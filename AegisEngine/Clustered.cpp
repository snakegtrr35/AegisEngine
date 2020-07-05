#include	"Clustered.h"
#include	"manager.h"
#include	"Scene.h"
#include	"Light.h"
#include	"Renderer.h"
#include	<io.h>

#include	"Scene_Manager.h"

static LIGHTS g_Light;

bool CLUSTERED::Init()
{
	HRESULT hr;
	auto device = CRenderer::GetDevice();
	
	//// コンピュートシェーダーの生成
	//{
	//	ID3D11ComputeShader* pCS = nullptr;

	//	// ライトカリングシェーダの生成
	//	{
	//		FILE* file;
	//		long int fsize;

	//		file = fopen("ParallelReduction.cso", "rb");
	//		fsize = _filelength(_fileno(file));
	//		BYTE* buffer = new BYTE[fsize];
	//		fread(buffer, fsize, 1, file);
	//		fclose(file);

	//		hr = device->CreateComputeShader(buffer, fsize, NULL, &pCS);

	//		delete[] buffer;

	//		if (FAILED(hr))
	//		{
	//			FAILDE_ASSERT;
	//			return false;
	//		}

	//		m_LightCullCS.reset(pCS);
	//	}
	//}

	// クラスターグリッドの作成
	{
		ID3D11Texture3D* tex = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE3D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = CLUSTERED_X;
		desc.Height = CLUSTERED_Y;
		desc.Depth = CLUSTERED_Z;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R32_UINT;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = device->CreateTexture3D(&desc, nullptr, &tex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		ClusteredTexture.reset(tex);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE3D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(ClusteredTexture.get(), &SRVDesc, &srv);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		ClusteredSRV.reset(srv);
	}

	//　ライトリストの作成
	{
		ID3D11Texture2D* tex = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = CLUSTERED_X * CLUSTERED_Y * CLUSTERED_Z;
		desc.Height = CLUSTERED_X ;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Format = DXGI_FORMAT_R32_UINT;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = device->CreateTexture2D(&desc, nullptr, &tex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		Light_List_Texture.reset(tex);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(Light_List_Texture.get(), &SRVDesc, &srv);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		Light_List_SRV.reset(srv);
	}

	// 定数バッファ作成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(float);
		desc.ByteWidth = sizeof(CLSTER_BUFFER);

	hr = device->CreateBuffer(&desc, nullptr, &buffer);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

		ClusterBuffer.reset(buffer);
	}

	// コンピュートシェーダー用の変数
	/*{
		// クラスターグリッド
		{
			ID3D11Texture3D* m_tex = nullptr;

			D3D11_TEXTURE3D_DESC textureDesc = {};
			textureDesc.Width = CLUSTERED_X;
			textureDesc.Height = CLUSTERED_Y;
			textureDesc.Depth = CLUSTERED_Z;
			textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R32_UINT;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			textureDesc.CPUAccessFlags = 0;

			hr = device->CreateTexture3D(&textureDesc, 0, &m_tex);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			ID3D11UnorderedAccessView* uav = nullptr;
			unique_ptr<ID3D11UnorderedAccessView, Release> g_pBlurUAView0;

			D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV = {};
			DescUAV.Format = DXGI_FORMAT_UNKNOWN;
			DescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
			DescUAV.Texture3D.MipSlice = 0;
			DescUAV.Texture3D.FirstWSlice = 0;
			DescUAV.Texture3D.WSize = CLUSTERED_Z;

			hr = device->CreateUnorderedAccessView(m_tex, &DescUAV, &uav);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			g_pBlurUAView0.reset(uav);

			ID3D11ShaderResourceView* srv = nullptr;
			unique_ptr<ID3D11ShaderResourceView, Release> g_pBlurRV0;

			D3D11_SHADER_RESOURCE_VIEW_DESC DescRV2 = {};
			DescRV2.Format = DXGI_FORMAT_R32_UINT;
			DescRV2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			DescRV2.Texture3D.MostDetailedMip = 0;
			DescRV2.Texture3D.MipLevels = 1;
			hr = device->CreateShaderResourceView(m_tex, &DescRV2, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			g_pBlurRV0.reset(srv);
		}

		// ライトリスト
		{
			ID3D11Texture2D* m_tex = nullptr;

			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = CLUSTERED_X * CLUSTERED_Y * CLUSTERED_Z;
			textureDesc.Height = CLUSTERED_X;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Format = DXGI_FORMAT_R32_UINT;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			textureDesc.CPUAccessFlags = 0;

			hr = device->CreateTexture2D(&textureDesc, 0, &m_tex);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			ID3D11UnorderedAccessView* uav = nullptr;
			unique_ptr<ID3D11UnorderedAccessView, Release> g_pBlurUAView0;

			D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV = {};
			DescUAV.Format = DXGI_FORMAT_UNKNOWN;
			DescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			DescUAV.Texture2D.MipSlice = 0;

			hr = device->CreateUnorderedAccessView(m_tex, &DescUAV, &uav);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			g_pBlurUAView0.reset(uav);

			ID3D11ShaderResourceView* srv = nullptr;
			unique_ptr<ID3D11ShaderResourceView, Release> g_pBlurRV0;

			D3D11_SHADER_RESOURCE_VIEW_DESC DescRV2 = {};
			DescRV2.Format = DXGI_FORMAT_R32_UINT;
			DescRV2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			DescRV2.Texture2D.MostDetailedMip = 0;
			DescRV2.Texture2D.MipLevels = 1;
			hr = device->CreateShaderResourceView(m_tex, &DescRV2, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			g_pBlurRV0.reset(srv);
		}
	}*/

	return true;
}

void CLUSTERED::Update()
{
	auto device_context = CRenderer::GetDeviceContext();

	unique_ptr<LIGHT_LIST> light_list = make_unique<LIGHT_LIST>();

	{
		unique_ptr<CLSTER> cluster = make_unique<CLSTER>();

		const auto* lights = CManager::Get_Instance()->Get_Scene()->Get_Light_Manager()->Get_Lights();

		//UINT cnt = 0;

		/*for (const auto& light : *lights)*/
		for (UINT cnt = 0; cnt < 512;)
		{
			for (int z = 0; z < CLUSTERED_Z; z++)
			{
				for (int y = 0; y < CLUSTERED_Y; y++)
				{
					for (int x = 0; x < CLUSTERED_X; x++)
					{
						// 当たり判定
						{
							const UINT quo = cnt / 32;
							cluster->at(z).at(y).at(x) |= (1 << quo);	// ライト番号 / 32 の商の値のビットを立てる

							const UINT rem = cnt % 32;
							light_list->at(z * CLUSTERED_X * CLUSTERED_Y + y * CLUSTERED_X + x).at(quo) |= (1 << rem);	// ライト番号 / 32 の余りの値のビットを立てる
						}
					}
				}
			}
			cnt++;
		}

		D3D11_MAPPED_SUBRESOURCE msr;
		device_context->Map(ClusteredTexture.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		for (UINT z = 0; z < CLUSTERED_Z; z++)
		{
			for (UINT y = 0; y < CLUSTERED_Y; y++)
			{
				memcpy(reinterpret_cast<BYTE*>(msr.pData) + z * msr.DepthPitch + y * msr.RowPitch, cluster->at(z).at(y).data(), CLUSTERED_X * sizeof(UINT));
			}
		}

		device_context->Unmap(ClusteredTexture.get(), 0);

		cluster.reset(nullptr);
	}

	{
		D3D11_MAPPED_SUBRESOURCE msr;
		device_context->Map(ClusteredTexture.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		for (UINT y = 0; y < CLUSTERED_Y; y++)
		{
			memcpy(reinterpret_cast<BYTE*>(msr.pData) + y * msr.RowPitch, light_list->at(y).data(), CLUSTERED_X * CLUSTERED_Y * CLUSTERED_Z * sizeof(UINT));
		}

		device_context->Unmap(ClusteredTexture.get(), 0);
	}

	light_list.reset(nullptr);
}

void CLUSTERED::Draw()
{
	auto tex = ClusteredSRV.get();

	CRenderer::GetDeviceContext()->PSSetShaderResources(10, 1, &tex);

	CLSTER_BUFFER cluster_buffer = CLSTER_BUFFER(XMFLOAT3(), XMFLOAT3());

	CRenderer::GetDeviceContext()->UpdateSubresource(ClusterBuffer.get(), 0, nullptr, &cluster_buffer, 0, 0);
}

void CLUSTERED::Uninit()
{
	//m_LightCullCS.reset(nullptr);

	ClusteredTexture.reset(nullptr);
	ClusteredSRV.reset(nullptr);

	Light_List_Texture.reset(nullptr);
	Light_List_SRV.reset(nullptr);

	ClusterBuffer.reset(nullptr);
}

void CLUSTERED::Light_Culling()
{
}