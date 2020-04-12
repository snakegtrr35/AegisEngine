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

	// 
	{
		ID3D11Texture3D* tex = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE3D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = CLUSTERED_X;
		desc.Height = CLUSTERED_Y;
		desc.Depth = CLUSTERED_Z;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R32G32_UINT;
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

		hr = device->CreateShaderResourceView(ClusteredTexture.get(), &SRVDesc, &srv);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
		ClusteredSRV.reset(srv);
	}

	return true;
}

void CLUSTERED::Update()
{
}

void CLUSTERED::Draw()
{
}

void CLUSTERED::Uninit()
{
	m_LightCullCS.reset(nullptr);
	ClusteredTexture.reset(nullptr);
	ClusteredSRV.reset(nullptr);
}

void CLUSTERED::Light_Culling()
{
}