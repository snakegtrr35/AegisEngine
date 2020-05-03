#include	"Clustered.h"
#include	"Light.h"
#include	"Renderer.h"
#include	<io.h>

#include	"Scene_Manager.h"

static LIGHTS g_Light;

bool CLUSTERED::Init()
{
	HRESULT hr;
	auto device = CRenderer::GetDevice();
	
	// コンピュートシェーダーの生成
	{
		ID3D11ComputeShader* pCS = nullptr;

		// ライトカリングシェーダの生成
		{
			FILE* file;
			long int fsize;

			file = fopen("ParallelReduction.cso", "rb");
			fsize = _filelength(_fileno(file));
			BYTE* buffer = new BYTE[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = device->CreateComputeShader(buffer, fsize, NULL, &pCS);

			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			m_LightCullCS.reset(pCS);
		}
	}

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
		desc.Height = CLUSTERED_X;
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

		ClusterBuffer.reset(buffer);
	}

	return true;
}

void CLUSTERED::Update()
{
	auto device_context = CRenderer::GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE msr;

	device_context->Map(ClusteredTexture.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	CLSTER* cluster = reinterpret_cast<CLSTER*>(msr.pData);

	UINT cnt = 0;

	for (int z = 0; z < CLUSTERED_Z; z++)
	{
		for (int y = 0; y < CLUSTERED_Y; y++)
		{
			for (int x = 0; x < CLUSTERED_X; x++, cnt++)
			{
				cluster->at(z).at(y).at(x) = XMINT2(x, cnt);	// テスト
			}
		}
	}

	device_context->Unmap(ClusteredTexture.get(), 0);
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
	m_LightCullCS.reset(nullptr);

	ClusteredTexture.reset(nullptr);
	ClusteredSRV.reset(nullptr);

	Light_List_Texture.reset(nullptr);
	Light_List_SRV.reset(nullptr);

	ClusterBuffer.reset(nullptr);
}

void CLUSTERED::Light_Culling()
{
}