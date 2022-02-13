#include	"Clustered.h"
#include	"manager.h"
#include	"Scene.h"
#include	"Light.h"
#include	"Renderer.h"
#include	<io.h>

#include	"Scene_Manager.h"

using namespace aegis;

static LIGHTS g_Light;

bool CLUSTERED::Init()
{
	HRESULT hr;
	auto device = CRenderer::getInstance()->GetDevice();
	
	// コンピュートシェーダーの生成
	{
		// ライトカリングシェーダの生成
		{
			FILE* file;
			long int fsize;

			file = fopen("ClusterCS.cso", "rb");
			fsize = _filelength(_fileno(file));
			BYTE* buffer = new BYTE[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = device->CreateComputeShader(buffer, fsize, NULL, &ClusterCS);

			delete[] buffer;

			assert(SUCCEEDED(hr));
		}
	}

	// クラスターグリッドの作成
	{
		D3D11_TEXTURE3D_DESC desc{};

		desc.Width = CLUSTERED_X;
		desc.Height = CLUSTERED_Y;
		desc.Depth = CLUSTERED_Z;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R32_UINT;

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		hr = device->CreateTexture3D(&desc, nullptr, &ClusterTexture);
		assert(SUCCEEDED(hr));

		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

			SRVDesc.Format = desc.Format;
			SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;

			hr = device->CreateShaderResourceView(ClusterTexture.Get(), &SRVDesc, &ClusterSRV);
			assert(SUCCEEDED(hr));
		}

		{
			hr = device->CreateUnorderedAccessView(ClusterTexture.Get(), nullptr, &ClusterUAV);
			assert(SUCCEEDED(hr));
		}
	}

	//　ライトリストの作成
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = CLUSTERED_X * CLUSTERED_Y * CLUSTERED_Z;
		desc.Height = CLUSTERED_X;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Format = DXGI_FORMAT_R32_UINT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		hr = device->CreateTexture2D(&desc, nullptr, &Light_List_Texture);
		assert(SUCCEEDED(hr));

		{
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

			SRVDesc.Format = desc.Format;
			SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;

			hr = device->CreateShaderResourceView(Light_List_Texture.Get(), &SRVDesc, &Light_List_SRV);
			assert(SUCCEEDED(hr));
		}

		{
			hr = device->CreateUnorderedAccessView(Light_List_Texture.Get(), nullptr, &Light_List_UAV);
			assert(SUCCEEDED(hr));
		}
	}

	// 定数バッファ作成
	{
		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(float);
		desc.ByteWidth = sizeof(CLSTER_BUFFER);

		hr = device->CreateBuffer(&desc, nullptr, &ClusterBuffer);
		assert(SUCCEEDED(hr));
	}

	// コンピュートシェーダー用の変数
	{
		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(float);
		desc.ByteWidth = sizeof(CONSTATNT_BUFFER);

		hr = device->CreateBuffer(&desc, nullptr, &ConstatntBuffer);
		assert(SUCCEEDED(hr));
	}

	m_Frustum.Init();
	m_Aabb.Init();

	m_Frustum.Aabb_Cale(m_Aabb);

	return true;
}

void CLUSTERED::Update()
{
	CRenderer* render = CRenderer::getInstance();
	auto device_context = render->GetDeviceContext();

	Cale_Cluster(m_Max, m_Min);
	{
		CLSTER_BUFFER buffer = {};

		Vector3 a = Vector3(m_Max - m_Min);
		Vector3 scale = a / Vector3(float(CLUSTERED_X), float(CLUSTERED_Y), float(CLUSTERED_Z));

		buffer.Scale = scale;

		buffer.Bias = m_Min;

		render->GetDeviceContext()->UpdateSubresource(ClusterBuffer.Get(), 0, nullptr, &buffer, 0, 0);
	}
}

void CLUSTERED::Draw()
{
	auto device_context = CRenderer::getInstance()->GetDeviceContext();

	/*{
		Cale_Cluster(m_Max, m_Min);
		{
			CLSTER_BUFFER buffer = {};

			Vector3 a = Vector3(m_Max - m_Min);
			Vector3 scale = a / Vector3(float(CLUSTERED_X), float(CLUSTERED_Y), float(CLUSTERED_Z));

			buffer.Scale = scale;

			buffer.Bias = m_Min;

			CRenderer::GetDeviceContext()->UpdateSubresource(ClusterBuffer.get(), 0, nullptr, &buffer, 0, 0);
		}
	}*/

	{
		{
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			device_context->ClearUnorderedAccessViewFloat(ClusterUAV.Get(), ClearColor);
			device_context->ClearUnorderedAccessViewFloat(Light_List_UAV.Get(), ClearColor);
		}

		{
			device_context->CSSetShader(ClusterCS.Get(), nullptr, NULL);

			// 定数バッファの設定
			{
				auto buffer = ConstatntBuffer.Get();

				device_context->CSSetConstantBuffers(0, 1, &buffer);

				buffer = LIGHTS::Get_LightBuffer();

				device_context->CSSetConstantBuffers(1, 1, &buffer);
			}

			device_context->CSSetUnorderedAccessViews(0, 1, ClusterUAV.GetAddressOf(), nullptr);
			device_context->CSSetUnorderedAccessViews(1, 1, ClusterUAV.GetAddressOf(), nullptr);

			device_context->Dispatch(CLUSTERED_X, CLUSTERED_Y, CLUSTERED_Z);

			ID3D11UnorderedAccessView* uav[2] = { nullptr, nullptr };
			device_context->CSSetUnorderedAccessViews(0, 2, uav, nullptr);
		}

		device_context->CSSetShader(nullptr, nullptr, NULL);

		ID3D11Buffer* buffer = nullptr;
		device_context->CSSetConstantBuffers(0, 1, &buffer);
		device_context->CSSetConstantBuffers(1, 1, &buffer);
	}

	device_context->PSSetShaderResources(10, 1, ClusterSRV.GetAddressOf());
	device_context->PSSetShaderResources(11, 1, Light_List_SRV.GetAddressOf());
	device_context->PSSetConstantBuffers(7, 1, ClusterBuffer.GetAddressOf());
}

void CLUSTERED::Uninit()
{
}

void CLUSTERED::Light_Culling()
{
}

void CLUSTERED::Cale_Cluster(Vector3& max, Vector3& min)
{
	m_Frustum.Update(0.f);

	BoundingFrustum Frustum = m_Frustum.Get_Collition();

	vector<Vector3> points = {};
	points.resize(8);

	Frustum.GetCorners(points.data());

	float max_x, max_y, max_z;
	float min_x, min_y, min_z;

	for (int i = 0; i < points.size(); i++)
	{
		// 最大値
		if (max.x <= points[i].x || 0 == i)
		{
			max.x = points[i].x;
		}

		if (max.y <= points[i].y || 0 == i)
		{
			max.y = points[i].y;
		}

		if (max.z <= points[i].z || 0 == i)
		{
			max.z = points[i].z;
		}

		// 最小値
		if (points[i].x <= min.x || 0 == i)
		{
			min.x = points[i].x;
		}

		if (points[i].y <= min.y || 0 == i)
		{
			min.y = points[i].y;
		}

		if (points[i].z <= min.z || 0 == i)
		{
			min.z = points[i].z;
		}
	}
}