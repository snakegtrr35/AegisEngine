#include "Clustered.h"
#include "manager.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"
#include <io.h>

#include "Scene_Manager.h"

using namespace aegis;

static LIGHTS g_Light;

bool CLUSTERED::Init()
{
	HRESULT hr;
	CRenderer* render = CRenderer::getInstance();
	
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

			ClusterCS.reset(render->CreateComputeShader(buffer, fsize));

			delete[] buffer;
		}
	}

	// クラスターグリッドの作成
	{
		Texture3DDesc desc{};
		desc.Width = CLUSTERED_X;
		desc.Height = CLUSTERED_Y;
		desc.Depth = CLUSTERED_Z;
		desc.MipLevels = 1;
		desc.Format = Format::R32Uint;
		desc.Usage = Usage::Default;
		desc.BindFlags = (BindFlag)((uint32)BindFlag::UnorderedAccess | (uint32)BindFlag::ShaderResource);
		desc.CPUAccessFlags = CpuAccessFlag::None;

		ClusterTexture.reset(render->CreateTexture3D(desc, nullptr));

		{
			ShaderResourceViewDesc SRVDesc{};

			SRVDesc.Format = desc.Format;
			SRVDesc.ViewDimension = SrvDimension::Texture3D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;

			ClusterSRV.reset(render->CreateShaderResourceView(ClusterTexture.get(), SRVDesc));
		}

		{
			ClusterUAV.reset(render->CreateUnorderedAccessView(ClusterTexture.get(), nullptr));
		}
	}

	//　ライトリストの作成
	{
		Texture2DDesc desc{};
		desc.Width = CLUSTERED_X * CLUSTERED_Y * CLUSTERED_Z;
		desc.Height = CLUSTERED_X;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Format = Format::R32Uint;
		desc.Usage = Usage::Default;
		desc.BindFlags = (BindFlag)((uint32)BindFlag::UnorderedAccess | (uint32)BindFlag::ShaderResource);
		desc.CPUAccessFlags = CpuAccessFlag::None;

		LightListTexture.reset(render->CreateTexture2D(desc, nullptr));

		{
			ShaderResourceViewDesc SRVDesc = {};

			SRVDesc.Format = desc.Format;
			SRVDesc.ViewDimension = SrvDimension::Texture2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;

			LightListSRV.reset(render->CreateShaderResourceView(LightListTexture.get(), SRVDesc));
		}

		{
			LightListUAV.reset(render->CreateUnorderedAccessView(LightListTexture.get(), nullptr));
		}
	}

	// 定数バッファ作成
	{
		BufferDesc  desc{};
		desc.Usage = Usage::Default;
		desc.ByteWidth = sizeof(CLSTER_BUFFER);
		desc.StructureByteStride = sizeof(float);
		desc.BindFlags = BindFlag::Constantbuffer;
		desc.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};

		ClusterBuffer.reset(render->CreateBuffer(desc, sd));
	}

	// コンピュートシェーダー用の変数
	{
		BufferDesc  desc{};
		desc.Usage = Usage::Default;
		desc.ByteWidth = sizeof(CONSTATNT_BUFFER);
		desc.StructureByteStride = sizeof(float);
		desc.BindFlags = BindFlag::Constantbuffer;
		desc.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};

		ConstatntBuffer.reset(render->CreateBuffer(desc, sd));
	}

	m_Frustum.Init();
	m_Aabb.Init();

	m_Frustum.Aabb_Cale(m_Aabb);

	return true;
}

void CLUSTERED::Update()
{
	auto render = CRenderer::getInstance();

	Cale_Cluster(m_Max, m_Min);
	{
		CLSTER_BUFFER buffer = {};

		Vector3 a = Vector3(m_Max - m_Min);
		Vector3 scale = a / Vector3(float(CLUSTERED_X), float(CLUSTERED_Y), float(CLUSTERED_Z));

		buffer.Scale = scale;

		buffer.Bias = m_Min;

		render->UpdateSubresource(ClusterBuffer.get(), &buffer);
	}
}

void CLUSTERED::Draw()
{
	auto render = CRenderer::getInstance();

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
			uint32 ClearColor[4] = { 0, 0, 0, 0. };
			render->ClearUnorderedAccessView(ClusterUAV.get(), ClearColor);
			render->ClearUnorderedAccessView(LightListUAV.get(), ClearColor);
		}

		{
			render->CSSetShader(ClusterCS.get());

			// 定数バッファの設定
			{
				auto buffer = ConstatntBuffer.get();

				render->CSSetConstantBuffers(0, 1, &buffer);

				buffer = LIGHTS::Get_LightBuffer();

				render->CSSetConstantBuffers(1, 1, &buffer);
			}

			UnorderedAccessView* cluster[] = { ClusterUAV.get() };
			UnorderedAccessView* lightList[] = { LightListUAV.get() };

			render->CSSetUnorderedAccessViews(0, 1, cluster);
			render->CSSetUnorderedAccessViews(1, 1, lightList);

			render->Dispatch(CLUSTERED_X, CLUSTERED_Y, CLUSTERED_Z);

			render->CSSetUnorderedAccessViews(0, 1, nullptr);
			render->CSSetUnorderedAccessViews(1, 1, nullptr);
		}

		render->CSSetShader(nullptr);

		Buffer* buffer = nullptr;
		render->CSSetConstantBuffers(0, 1, &buffer);
		render->CSSetConstantBuffers(1, 1, &buffer);
	}

	aegis::ShaderResourceView* cluster[] = { ClusterSRV.get() };

	aegis::ShaderResourceView* lightList[] = { LightListSRV.get()};

	aegis::Buffer* buffer[] = { ClusterBuffer.get() };

	render->PSSetShaderResources(10, 1, cluster);
	render->PSSetShaderResources(11, 1, lightList);
	render->PSSetConstantBuffers(7, 1, buffer);
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