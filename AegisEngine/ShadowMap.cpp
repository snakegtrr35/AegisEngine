#include	"ShadowMap.h"
#include	"Game_Object.h"
#include	"Renderer.h"

float SHADOW_MAP::WIDTH = 2048.0f;
float SHADOW_MAP::HEIGHT = 2048.0f;

static ID3D11DepthStencilState* m_DepthStateEnable;


SHADOW_MAP::SHADOW_MAP()
{
	LightPos = XMFLOAT3(10.0f, 10.0f, -10.0f);

	Enable = false;

	{
		// ビューマトリックス(光源から見る)
		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&LightPos), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 1.0f, 0.f, 0.f));
	}

	{
		// プロジェクションマトリックス
		//PlojectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f), WIDTH / HEIGHT, 1.0f, 100.0f);

		PlojectionMatrix = XMMatrixOrthographicLH(30.0f, 30.0f, 0.1f, 100.0f);
	}

	{
		// ビューポートの設定設定
		Viewport.left = 0;
		Viewport.top = 0;
		Viewport.right = WIDTH;
		Viewport.bottom = HEIGHT;

		// ビューポート設定
		DxViewport.Width = (float)(Viewport.right - Viewport.left);
		DxViewport.Height = (float)(Viewport.bottom - Viewport.top);
		DxViewport.MinDepth = 0.0f;
		DxViewport.MaxDepth = 1.0f;
		DxViewport.TopLeftX = (float)Viewport.left;
		DxViewport.TopLeftY = (float)Viewport.top;
	}

	Target = nullptr;
}

bool SHADOW_MAP::Init()
{
	HRESULT hr;

	// デプスステンシルビューの作成
	{
		ID3D11DepthStencilView* dsv = nullptr;
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R24G8_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = CRenderer::GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = 0;
			desc.Texture2D.MipSlice = 0;

			hr = CRenderer::GetDevice()->CreateDepthStencilView(pTex, &desc, &dsv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			DepthStencilView.reset(dsv);
		}

		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
			desc.Texture2D.MipSlice = 0;

			hr = CRenderer::GetDevice()->CreateDepthStencilView(pTex, &desc, &dsv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			DepthStencilView2.reset(dsv);
		}
    }

	// デプスバッファの作成
	{
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = CRenderer::GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// レンダーターゲットビュー設定
		{
			ID3D11RenderTargetView* pRtv;

			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = td.Format;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			hr = CRenderer::GetDevice()->CreateRenderTargetView(pTex, &desc, &pRtv);
			if (FAILED(hr))
			{
				return false;
			}

			RenderTargetView[0].reset(pRtv);
		}

		// シェーダーリソースビュー設定
		{
			ID3D11ShaderResourceView* srv = nullptr;

			// シェーダーリソースビューの設定
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = td.Format;
			desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.MipLevels = 1;

			hr = CRenderer::GetDevice()->CreateShaderResourceView(pTex, &desc, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
			ShaderResourceView[0].reset(srv);
		}
	}

	// アルベドバッファの作成
	{
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = CRenderer::GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// レンダーターゲットビュー設定
		{
			ID3D11RenderTargetView* pRtv;

			hr = CRenderer::GetDevice()->CreateRenderTargetView(pTex, nullptr, &pRtv);
			if (FAILED(hr))
			{
				return false;
			}

			RenderTargetView[1].reset(pRtv);
		}

		// シェーダーリソースビュー設定
		{
			ID3D11ShaderResourceView* srv = nullptr;

			hr = CRenderer::GetDevice()->CreateShaderResourceView(pTex, nullptr, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
			ShaderResourceView[1].reset(srv);
		}
	}

	// 法線バッファの作成
	{
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R11G11B10_FLOAT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = CRenderer::GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// レンダーターゲットビュー設定
		{
			ID3D11RenderTargetView* pRtv;

			hr = CRenderer::GetDevice()->CreateRenderTargetView(pTex, nullptr, &pRtv);
			if (FAILED(hr))
			{
				return false;
			}

			RenderTargetView[2].reset(pRtv);
		}

		// シェーダーリソースビュー設定
		{
			ID3D11ShaderResourceView* srv = nullptr;

			hr = CRenderer::GetDevice()->CreateShaderResourceView(pTex, nullptr, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
			ShaderResourceView[2].reset(srv);
		}
	}

	{
		// ブレンドステート設定
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		for (char i = 0; i < 2; i++)
		{
			blendDesc.RenderTarget[i].BlendEnable = TRUE;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		ID3D11BlendState* bs = NULL;
		CRenderer::GetDevice()->CreateBlendState(&blendDesc, &bs);

		BlendState.reset(bs);
	}

	// ラスタライズステートの設定
	{
		ID3D11RasterizerState* rs = nullptr;

		// ラスタライザステート設定
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;
		rd.DepthBias = 0.001f;
		rd.DepthBiasClamp = 0.01f;
		rd.SlopeScaledDepthBias = 0.001f;

		CRenderer::GetDevice()->CreateRasterizerState(&rd, &rs);

		RasterizerState.reset(rs);
	}

	// テクスチャ用のサンプラーステートの生成
	{
		ID3D11SamplerState* sampler = nullptr;

		D3D11_SAMPLER_DESC desc;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0;
		desc.MaxAnisotropy = 16;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		// サンプラーステートを生成
		HRESULT hr = CRenderer::GetDevice()->CreateSamplerState(&desc, &sampler);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		Texture_Sampler.reset(sampler);
	}

	// サンプラーステートの生成
	{
		ID3D11SamplerState* sampler = nullptr;

		D3D11_SAMPLER_DESC desc;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		desc.MaxAnisotropy = 1;
		desc.MipLODBias = 0;
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = 0.f;

		// サンプラーステートを生成
		HRESULT hr = CRenderer::GetDevice()->CreateSamplerState(&desc, &sampler);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		Sampler.reset(sampler);

		CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &sampler);
	}

	// 定数バッファ作成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(CONSTANT_SHADOW_MAP);
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		{
			hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &buffer);

			ShadowBuffer.reset(buffer);
		}

		{
			hBufferDesc.ByteWidth = sizeof(CONSTANT_LIGHT);

			hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			CRenderer::GetDeviceContext()->PSSetConstantBuffers(0, 1, &buffer);

			LightBuffer.reset(buffer);
		}
	}

	Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);
	Shadow.ProjectionMatrix = XMMatrixTranspose(PlojectionMatrix);

	// シャドウ生成パスの定数バッファを更新
	CRenderer::GetDeviceContext()->UpdateSubresource(ShadowBuffer.get(), 0, nullptr, &Shadow, 0, 0);


	{
		// 深度ステンシルステート設定
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = FALSE;

		CRenderer::GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート
	}

    return true;
}

void SHADOW_MAP::Begin()
{
	ID3D11DepthStencilView* pDSV = DepthStencilView.get();

	//ID3D11RenderTargetView* pRTV = RenderTargetView[0].get();
	ID3D11RenderTargetView* pRTV[3] = { RenderTargetView[0].get(), RenderTargetView[1].get(), RenderTargetView[2].get() };

    //CRenderer::GetDeviceContext()->OMSetRenderTargets( 1, &pRTV, pDSV );
	CRenderer::GetDeviceContext()->OMSetRenderTargets(3, pRTV, pDSV);

	D3D11_VIEWPORT V[3] = { DxViewport, DxViewport, DxViewport };

	CRenderer::GetDeviceContext()->RSSetViewports(3, V);

	// バックバッファクリア
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//CRenderer::GetDeviceContext()->ClearRenderTargetView(pRTV, ClearColor);
	CRenderer::GetDeviceContext()->ClearRenderTargetView( pRTV[0], ClearColor );
	CRenderer::GetDeviceContext()->ClearRenderTargetView( pRTV[1], ClearColor);
	CRenderer::GetDeviceContext()->ClearRenderTargetView( pRTV[2], ClearColor);
    CRenderer::GetDeviceContext()->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	Enable = true;

	CRenderer::GetDeviceContext()->RSSetState(RasterizerState.get());

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	CRenderer::GetDeviceContext()->OMSetBlendState(BlendState.get(), blendFactor, 0xffffffff);

	ID3D11SamplerState* s = Texture_Sampler.get();

	CRenderer::GetDeviceContext()->PSSetSamplers(0, 1, &s);

	Set_Light(LightPos);
}

void SHADOW_MAP::End()
{
	Enable = false;

	ID3D11ShaderResourceView* srv = ShaderResourceView[0].get();

	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &srv);

	ID3D11SamplerState* s = Sampler.get();

	CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &s);

	CRenderer::SetBlendState();
}

void SHADOW_MAP::Update()
{
	LIGHT* light = CRenderer::Get_Light();

	LightPos.x = light->Position.x;
	LightPos.y = light->Position.y;
	LightPos.z = light->Position.z;

	if (nullptr != Target)
	{
		XMFLOAT3* at = Target->Get_Position();


		XMFLOAT3 pos;
		pos.x = at->x + LightPos.x;
		pos.y = at->y + LightPos.y;
		pos.z = at->z + LightPos.z;

		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMVectorSet(at->x, at->y, at->z, 0.f), XMVectorSet(0.f, 1.0f, 0.f, 0.f));

		Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);

		// シャドウ生成パスの定数バッファを更新
		CRenderer::GetDeviceContext()->UpdateSubresource(ShadowBuffer.get(), 0, nullptr, &Shadow, 0, 0);
	}
}

void SHADOW_MAP::Uninit()
{
	RenderTargetView[0].reset(nullptr);
	RenderTargetView[1].reset(nullptr);
	RenderTargetView[2].reset(nullptr);

	BlendState.reset(nullptr);

	ShaderResourceView[0].reset(nullptr);
	ShaderResourceView[1].reset(nullptr);
	ShaderResourceView[2].reset(nullptr);

	DepthStencilView.reset(nullptr);

	RasterizerState.reset(nullptr);

	Sampler.reset(nullptr);
	Texture_Sampler.reset(nullptr);

	ShadowBuffer.reset(nullptr);
}

void SHADOW_MAP::Set_SamplerState()
{
	ID3D11SamplerState* samp = Sampler.get();

	CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &samp);
}

void SHADOW_MAP::Set_LightPos(const XMFLOAT3& pos)
{
	LightPos = pos;
}

void SHADOW_MAP::Set_Target(GAME_OBJECT* object)
{
	Target = object;
}

void SHADOW_MAP::Set_Light(const XMFLOAT3& pos)
{
	Light.Direction.x = -pos.x;
	Light.Direction.y = -pos.y;
	Light.Direction.z = -pos.z;
	Light.Direction.w = 0.f;

	CRenderer::GetDeviceContext()->UpdateSubresource(LightBuffer.get(), 0, nullptr, &Light, 0, 0);
}