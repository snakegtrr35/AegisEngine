#include	"ForwardPlus.h"
#include	"Light.h"
#include	"Renderer.h"
#include	<io.h>

#include	"Scene.h"

static LIGHTS g_Light;

bool FORWARDLUS::Init()
{
	HRESULT hr;
	auto device = CRenderer::GetDevice();
	
	// 頂点シェーダ生成
	{
		ID3D11VertexShader* vertex_shader = nullptr;

		FILE* file;
		long int fsize;

		file = fopen("VertexShader_Depth_Pre_Pass.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		hr = device->CreateVertexShader(buffer, fsize, NULL, &vertex_shader);

		delete[] buffer;

		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		m_VertexShader.reset(vertex_shader);
	}

	// コンピュートシェーダーの生成
	{
		ID3D11ComputeShader* pCS = nullptr;

		// ライトカリングシェーダの生成
		{
			FILE* file;
			long int fsize;

			file = fopen("TilingForward.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = device->CreateComputeShader(buffer, fsize, NULL, &pCS);

			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			m_pLightCullCS.reset(pCS);
		}

		// Zpre pass シェーダの生成
		{
			FILE* file;
			long int fsize;

			file = fopen("ParallelReduction.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = device->CreateComputeShader(buffer, fsize, NULL, &pCS);

			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			m_pLightCullCS.reset(pCS);
		}
	}

	{
		ID3D11DepthStencilState* pDSS = nullptr;

		// Default depth-stencil state, except with inverted DepthFunc 
		// (because we are using inverted 32-bit float depth for better precision)
		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
		ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		DepthStencilDesc.DepthEnable = TRUE;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;  // we are using inverted 32-bit float depth for better precision
		DepthStencilDesc.StencilEnable = FALSE;
		DepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		// Disable depth test write
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		hr = device->CreateDepthStencilState(&DepthStencilDesc, &pDSS);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		m_pDepthStencilState.reset(pDSS);
	}

	{
		DXGI_SAMPLE_DESC sample_dasc;
		sample_dasc.Count = 1;

		bool flag = CreateDepthStencilSurface(&m_DepthStencilBuffer.m_pDepthStencilTexture, &m_DepthStencilBuffer.m_pDepthStencilSRV, &m_DepthStencilBuffer.m_pDepthStencilView,
			DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT, sample_dasc.Count);

		if (false == flag)
		{
			return false;
		}
	}

	{
		auto texture = m_pDepthBoundsTexture.get();
		auto srv = m_pDepthBoundsSRV.get();
		auto uav = m_pDepthBoundsUAV.get();

		bool flag = CreateSurface(&texture, &srv, nullptr, &uav, DXGI_FORMAT_R32G32B32A32_FLOAT, GetNumTilesX(), GetNumTilesY(), 1);
	}

	{
		unsigned uNumTiles = GetNumTilesX()*GetNumTilesY();
		unsigned uMaxNumElementsPerTile = GetMaxNumElementsPerTile();

		ID3D11Buffer* buffer = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11UnorderedAccessView* uav = nullptr;

		D3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(BufferDesc));
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = 2 * uMaxNumElementsPerTile * uNumTiles;
		BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

		hr = device->CreateBuffer(&BufferDesc, NULL, &buffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		m_pLightIndexBuffer.reset(buffer);

		hr = device->CreateBuffer(&BufferDesc, NULL, &buffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		m_pSpotIndexBuffer.reset(buffer);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = DXGI_FORMAT_R16_UINT;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		SRVDesc.Buffer.ElementOffset = 0;
		SRVDesc.Buffer.ElementWidth = uMaxNumElementsPerTile * uNumTiles;

		hr = device->CreateShaderResourceView(m_pLightIndexBuffer.get(), &SRVDesc, &srv);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		m_pLightIndexBufferSRV.reset(srv);

		hr = device->CreateShaderResourceView(m_pSpotIndexBuffer.get(), &SRVDesc, &srv);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		m_pSpotIndexBufferSRV.reset(srv);

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
		ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		UAVDesc.Format = DXGI_FORMAT_R16_UINT;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.NumElements = uMaxNumElementsPerTile * uNumTiles;

		hr = device->CreateUnorderedAccessView(m_pLightIndexBuffer.get(), &UAVDesc, &uav);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		m_pLightIndexBufferUAV.reset(uav);

		hr = device->CreateUnorderedAccessView(m_pSpotIndexBuffer.get(), &UAVDesc, &uav);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		m_pSpotIndexBufferUAV.reset(uav);
	}

	return true;
}

void FORWARDLUS::Update()
{

}

void FORWARDLUS::Draw()
{
}

void FORWARDLUS::Uninit()
{	
	m_VertexShader.reset(nullptr);
	m_pDepthBoundsCS.reset(nullptr);
	m_pLightCullCS.reset(nullptr);

	m_pDepthStencilView.reset(nullptr);
	m_DepthStateEnable.reset(nullptr);
	m_pDepthStencilState.reset(nullptr);
}

void FORWARDLUS::Depth_Pre_Pass(SCENE_MANAGER* scene_manager)
{
	ID3D11RenderTargetView* pNULLRTV = nullptr;
	ID3D11DepthStencilView* pNULLDSV = nullptr;
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	ID3D11UnorderedAccessView* pNULLUAV = nullptr;
	ID3D11SamplerState* pNULLSampler = nullptr;

	auto device_context = CRenderer::GetDeviceContext();

	// Depth pre-pass
	{
		// Depth pre-pass (to eliminate pixel overdraw during forward rendering)
		device_context->OMSetRenderTargets(1, &pNULLRTV, m_pDepthStencilView.get());  // null color buffer
		device_context->OMSetDepthStencilState(m_DepthStateEnable.get(), NULL);  // we are using inverted 32-bit float depth for better precision
		//device_context->IASetInputLayout(m_pLayoutPositionOnly11);
		device_context->VSSetShader(m_VertexShader.get(), nullptr, 0);
		device_context->PSSetShader(nullptr, nullptr, 0);  // null pixel shader
		device_context->PSSetShaderResources(0, 1, &pNULLSRV);
		device_context->PSSetShaderResources(1, 1, &pNULLSRV);
		device_context->PSSetSamplers(0, 1, &pNULLSampler);

		// Draw the main scene
		scene_manager->Draw_DPP();

		// Restore to default
		//CRenderer::GetDeviceContext()->RSSetState(nullptr);
		//CRenderer::GetDeviceContext()->OMSetBlendState(m_pBlendStateOpaque, BlendFactor, 0xffffffff);
	}
}

void FORWARDLUS::Light_Culling()
{
	ID3D11RenderTargetView* pNULLRTV = nullptr;
	ID3D11DepthStencilView* pNULLDSV = nullptr;
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	ID3D11UnorderedAccessView* pNULLUAV = nullptr;
	ID3D11SamplerState* pNULLSampler = nullptr;

	ID3D11ShaderResourceView* pDepthSRV = m_DepthStencilBuffer.m_pDepthStencilSRV;

	auto device_context = CRenderer::GetDeviceContext();

	//Light culling
	{
		device_context->OMSetRenderTargets(1, &pNULLRTV, pNULLDSV);  // null color buffer and depth-stencil
		device_context->OMSetDepthStencilState(m_pDepthStencilState.get(), 0x00);
		device_context->VSSetShader(nullptr, nullptr, 0);  // null vertex shader
		device_context->PSSetShader(nullptr, nullptr, 0);  // null pixel shader
		device_context->PSSetShaderResources(0, 1, &pNULLSRV);
		device_context->PSSetShaderResources(1, 1, &pNULLSRV);
		device_context->PSSetSamplers(0, 1, &pNULLSampler);

		// Calculate per-tile depth bounds on the GPU, using a Compute Shader
		{
			auto uav = m_pDepthBoundsUAV.get();

			device_context->CSSetShader(m_pDepthBoundsCS.get(), nullptr, 0);
			device_context->CSSetShaderResources(0, 1, &pDepthSRV);
			device_context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
			device_context->Dispatch(GetNumTilesX(), GetNumTilesY(), 1);
			device_context->CSSetUnorderedAccessViews(0, 1, &pNULLUAV, nullptr);
		}

		// Cull lights on the GPU, using a Compute Shader
		{
			ID3D11ShaderResourceView* srv = nullptr;
			auto uav = m_pLightIndexBufferUAV.get();

			device_context->CSSetShader(m_pLightCullCS.get(), nullptr, 0);

			srv = g_Light.GetPointLightBufferCenterAndRadiusSRVParam();
			device_context->CSSetShaderResources(0, 1, &srv);

			srv = g_Light.GetSpotLightBufferCenterAndRadiusSRVParam();
			device_context->CSSetShaderResources(1, 1, &srv);

			srv = m_pDepthBoundsSRV.get();
			device_context->CSSetShaderResources(2, 1, &srv);


			device_context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

			uav = m_pSpotIndexBufferUAV.get();
			device_context->CSSetUnorderedAccessViews(1, 1, &uav, nullptr);
			device_context->Dispatch(GetNumTilesX(), GetNumTilesY(), 1);
		}

		device_context->CSSetShader(nullptr, nullptr, 0);
		device_context->CSSetShaderResources(0, 1, &pNULLSRV);
		device_context->CSSetShaderResources(1, 1, &pNULLSRV);
		device_context->CSSetShaderResources(2, 1, &pNULLSRV);
		device_context->CSSetUnorderedAccessViews(0, 1, &pNULLUAV, nullptr);
		device_context->CSSetUnorderedAccessViews(1, 1, &pNULLUAV, nullptr);
	}
}

bool FORWARDLUS::CreateDepthStencilSurface(ID3D11Texture2D** ppDepthStencilTexture, ID3D11ShaderResourceView** ppDepthStencilSRV,
	ID3D11DepthStencilView** ppDepthStencilView, DXGI_FORMAT DSFormat, DXGI_FORMAT SRVFormat,
	unsigned int uWidth, unsigned int uHeight, unsigned int uSampleCount)
{
	auto device = CRenderer::GetDevice();
	HRESULT hr = S_OK;
	DXGI_FORMAT TextureFormat;

	if (ppDepthStencilTexture)
	{
		switch (DSFormat)
		{
		case DXGI_FORMAT_D32_FLOAT:
			TextureFormat = DXGI_FORMAT_R32_TYPELESS;
			break;

		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			TextureFormat = DXGI_FORMAT_R24G8_TYPELESS;
			break;

		case DXGI_FORMAT_D16_UNORM:
			TextureFormat = DXGI_FORMAT_R16_TYPELESS;
			break;

		default:
			TextureFormat = DXGI_FORMAT_UNKNOWN;
			break;
		}
		assert(TextureFormat != DXGI_FORMAT_UNKNOWN);

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = uWidth;
		descDepth.Height = uHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = TextureFormat;
		descDepth.SampleDesc.Count = uSampleCount;
		descDepth.SampleDesc.Quality = 0;//( uSampleCount > 1 ) ? ( D3D10_STANDARD_MULTISAMPLE_PATTERN ) : ( 0 );
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		if (NULL != ppDepthStencilSRV)
		{
			if ((descDepth.SampleDesc.Count == 1))
			{
				descDepth.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
			}
		}

		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = device->CreateTexture2D(&descDepth, NULL, ppDepthStencilTexture);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		if (NULL != ppDepthStencilView)
		{
			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			descDSV.Format = DSFormat;
			if (descDepth.SampleDesc.Count > 1)
			{
				descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			}
			descDSV.Texture2D.MipSlice = 0;
			descDSV.Flags = 0;

			hr = device->CreateDepthStencilView((ID3D11Resource*)*ppDepthStencilTexture, &descDSV, ppDepthStencilView);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		if (NULL != ppDepthStencilSRV)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRDesc;
			SRDesc.Format = SRVFormat;
			SRDesc.ViewDimension = (uSampleCount > 1) ? (D3D11_SRV_DIMENSION_TEXTURE2DMS) : (D3D11_SRV_DIMENSION_TEXTURE2D);
			SRDesc.Texture2D.MostDetailedMip = 0;
			SRDesc.Texture2D.MipLevels = 1;

			hr = device->CreateShaderResourceView((ID3D11Resource*)*ppDepthStencilTexture, &SRDesc, ppDepthStencilSRV);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
	}

	return true;
}

bool FORWARDLUS::CreateSurface(ID3D11Texture2D** ppTexture, ID3D11ShaderResourceView** ppTextureSRV,
	ID3D11RenderTargetView** ppTextureRTV, ID3D11UnorderedAccessView** ppTextureUAV,
	DXGI_FORMAT Format, unsigned int uWidth, unsigned int uHeight, unsigned int uSampleCount)
{
	auto device = CRenderer::GetDevice();
	HRESULT hr = S_OK;

	if (ppTexture)
	{
		D3D11_TEXTURE2D_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));

		if (NULL == *ppTexture)
		{
			Desc.Width = uWidth;
			Desc.Height = uHeight;
			Desc.MipLevels = 1;
			Desc.ArraySize = 1;
			Desc.Format = Format;
			Desc.SampleDesc.Count = uSampleCount;
			Desc.SampleDesc.Quality = 0;//( uSampleCount > 1 ) ? ( D3D10_STANDARD_MULTISAMPLE_PATTERN ) : ( 0 );
			Desc.Usage = D3D11_USAGE_DEFAULT;
			Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			if (ppTextureUAV)
			{
				Desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
			}
			hr = device->CreateTexture2D(&Desc, NULL, ppTexture);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		if (ppTextureSRV)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			SRVDesc.Format = Format;
			SRVDesc.ViewDimension = (uSampleCount > 1) ? (D3D11_SRV_DIMENSION_TEXTURE2DMS) : (D3D11_SRV_DIMENSION_TEXTURE2D);
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;
			hr = device->CreateShaderResourceView(*ppTexture, &SRVDesc, ppTextureSRV);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		if (ppTextureRTV)
		{
			D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
			ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
			RTVDesc.Format = Format;
			RTVDesc.ViewDimension = (uSampleCount > 1) ? (D3D11_RTV_DIMENSION_TEXTURE2DMS) : (D3D11_RTV_DIMENSION_TEXTURE2D);
			RTVDesc.Texture2D.MipSlice = 0;
			hr = device->CreateRenderTargetView(*ppTexture, &RTVDesc, ppTextureRTV);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		if (ppTextureUAV)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
			ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
			UAVDesc.Format = Format;
			UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			UAVDesc.Buffer.FirstElement = 0;
			UAVDesc.Buffer.NumElements = Desc.Width * Desc.Height;
			hr = device->CreateUnorderedAccessView(*ppTexture, &UAVDesc, ppTextureUAV);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
	}

	return true;
}

WORD FORWARDLUS::GetNumTilesX()
{
	return (WORD)((SCREEN_WIDTH + TILE_RES - 1) / (float)TILE_RES);
}

WORD FORWARDLUS::GetNumTilesY()
{
	return (WORD)((SCREEN_HEIGHT + TILE_RES - 1) / (float)TILE_RES);
}

WORD FORWARDLUS::GetMaxNumElementsPerTile()
{
	// max num lights times 2 (because the halfZ method has two lists per tile, list A and B),
	// plus two more to store the 32-bit halfZ, plus one more for the light count of list A,
	// plus one more for the light count of list B
	return (2 * GetMaxNumLightsPerTile() + 4);
}

WORD FORWARDLUS::GetMaxNumLightsPerTile()
{
	const unsigned kAdjustmentMultipier = 16;

	// adjust max lights per tile down as height increases
	return WORD(MAX_NUM_LIGHTS_PER_TILE - (kAdjustmentMultipier * (SCREEN_HEIGHT / 120)));
}