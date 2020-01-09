#include	"Clustered.h"
#include	"Renderer.h"

static const constexpr float A = 1.0f * 0.5f * 0.5f;

XMFLOAT3 operator -(const XMFLOAT3& a, const XMFLOAT3& b)
{
	XMFLOAT3 f;
	f.x = a.x - b.x;
	f.y = a.y - b.y;
	f.z = a.z - b.z;

	return f;
}

XMFLOAT3 operator /(const XMFLOAT3& a, const XMFLOAT3& b)
{
	XMFLOAT3 f;
	f.x = a.x / b.x;
	f.y = a.y / b.y;
	f.z = a.z / b.z;

	return f;
}

XMFLOAT3 operator +(const XMFLOAT3& a, const float t)
{
	XMFLOAT3 f;
	f.x = a.x + t;
	f.y = a.y + t;
	f.z = a.z + t;

	return f;
}

XMFLOAT3 operator -(const XMFLOAT3& a, const float t)
{
	XMFLOAT3 f;
	f.x = a.x - t;
	f.y = a.y - t;
	f.z = a.z - t;

	return f;
}

XMFLOAT3 operator *(const XMFLOAT3& a, const XMFLOAT3& b)
{
	XMFLOAT3 f;
	f.x = a.x * b.x;
	f.y = a.y * b.y;
	f.z = a.z * b.z;

	return f;
}

XMFLOAT3 operator *(const XMFLOAT3& a, const float t)
{
	XMFLOAT3 f;
	f.x = a.x * t;
	f.y = a.y * t;
	f.z = a.z * t;

	return f;
}

XMFLOAT3 operator /(const XMFLOAT3& a, const float t)
{
	XMFLOAT3 f;
	f.x = a.x / t;
	f.y = a.y / t;
	f.z = a.z / t;

	return f;
}

XMFLOAT3 operator /(const float t, const XMFLOAT3& a)
{
	XMFLOAT3 f;
	f.x = t / a.x;
	f.y = t / a.y;
	f.z = t / a.z;

	return f;
}

bool CLUSTERED::Init()
{
	{
		//AABB_min = XMFLOAT3(-1024.00012f, -256.0f, -1024.0f);
		//AABB_max = XMFLOAT3(2048.0f, 256.0f, 2048.0f) + 1.0;

		AABB_min = XMFLOAT3(-10.0f, -10.0f, -10.0f);
		AABB_max = XMFLOAT3(10.0f, 10.0f, 10.0f) + 1.0;
	}

	{
		// Initialize all lights
		Light light[LIGHT_COUNT] = {};

		//light[0] =  { XMFLOAT3(576  * A, 96 * A,    0), 640.0f * A };
		//light[1] =  { XMFLOAT3(0    * A, 96 * A,  576), 640.0f * A };
		//light[2] =  { XMFLOAT3(-576 * A, 96 * A,    0), 640.0f * A };
		//light[3] =  { XMFLOAT3(0    * A, 96 * A, -576), 640.0f * A };
		//light[4] =  { XMFLOAT3(1792 * A, 96 * A,  320), 550.0f * A };
		//light[5] =  { XMFLOAT3(1792 * A, 96 * A, -320), 550.0f * A };
		//light[6] =  { XMFLOAT3(-192 * A, 96 * A, 1792), 550.0f * A };
		//light[7] =  { XMFLOAT3(-832 * A, 96 * A, 1792), 550.0f * A };
		//light[8] =  { XMFLOAT3(1280 * A, 32 * A,  192), 450.0f * A };
		//light[9] =  { XMFLOAT3(1280 * A, 32 * A, -192), 450.0f * A };
		//light[10] = { XMFLOAT3(-320 * A, 32 * A, 1280), 450.0f * A };
		//light[11] = { XMFLOAT3(-704 * A, 32 * A, 1280), 450.0f * A };
		//light[12] = { XMFLOAT3(960  * A, 32 * A,  640), 450.0f * A };
		//light[13] = { XMFLOAT3(960  * A, 32 * A, -640), 450.0f * A };
		//light[14] = { XMFLOAT3(640  * A, 32 * A, -960), 450.0f * A };
		//light[15] = { XMFLOAT3(-640 * A, 32 * A, -960), 450.0f * A };
		//light[16] = { XMFLOAT3(-960 * A, 32 * A,  640), 450.0f * A };
		//light[17] = { XMFLOAT3(-960 * A, 32 * A, -640), 450.0f * A };
		//light[18] = { XMFLOAT3(640  * A, 32 * A,  960), 450.0f * A };

		light[0] = { XMFLOAT3(-2.5, 1, 2.5),  2.0f };
		light[1] = { XMFLOAT3(0,    1, 0),    2.0f };
		light[2] = { XMFLOAT3(2.5,  1, 2.5),  2.0f };

		light[3] = { XMFLOAT3(-2.5, 1, 0),    2.0f };
		light[4] = { XMFLOAT3(0,    1, 0),    2.0f };
		light[5] = { XMFLOAT3(2.5,  1, 0),    2.0f };

		light[6] = { XMFLOAT3(-2.5, 1, -2.5), 2.0f };
		light[7] = { XMFLOAT3(0,    1, -2.5), 2.0f };
		light[8] = { XMFLOAT3(2.5,  1, -2.5), 2.0f };

		m_Lights.reserve(LIGHT_COUNT);

		for (char i = 0; i < LIGHT_COUNT; i++)
		{
			m_Lights.emplace_back(light[i]);
		}

	}

	{
		D3D11_TEXTURE3D_DESC desc;
		desc.Width = CLUSTER_X;
		desc.Height = CLUSTER_Y;
		desc.Depth = CLUSTER_Z;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R32_UINT;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		ID3D11Texture3D* tex = NULL;
		if (FAILED(CRenderer::GetDevice()->CreateTexture3D(&desc, NULL, &tex)))
		{
			return false;
		}

		Resource.reset(tex);


		// シェーダーリソースビュー設定
		{
			ID3D11ShaderResourceView* srv = nullptr;

			// シェーダーリソースビューの設定
			D3D11_SHADER_RESOURCE_VIEW_DESC d;
			ZeroMemory(&d, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			d.Format = DXGI_FORMAT_R32_UINT;
			d.ViewDimension = D3D_SRV_DIMENSION_TEXTURE3D;
			d.Texture3D.MostDetailedMip = 0;
			d.Texture3D.MipLevels = 1;

			HRESULT hr = CRenderer::GetDevice()->CreateShaderResourceView(tex, &d, &srv);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			ShaderResourceView.reset(srv);
		}
	}

	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT);

		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);

		ConstantBuffer.reset(buffer);

		XMFLOAT4 scale;
		XMFLOAT4 bias;

		{
			XMFLOAT3 s = XMFLOAT3(float(CLUSTER_X), float(CLUSTER_Y), float(CLUSTER_Z)) / (AABB_max - AABB_min);
			scale.x = s.x;
			scale.y = s.y;
			scale.z = s.z;
			scale.w = 0.f;

			s =  s - 1.0f;

			XMFLOAT3 b =  s * AABB_min;

			bias.x = b.x;
			bias.y = b.y;
			bias.z = b.z;
			bias.w = 0.f;
		}

		{
			CONSTANT constant;

			for (char i = 0; i < LIGHT_COUNT; i++)
			{
				constant.Lights[i] = m_Lights[i];
			}

			constant.Scale = scale;
			constant.Bias = bias;

			CRenderer::GetDeviceContext()->UpdateSubresource(ConstantBuffer.get(), 0, NULL, &constant, 0, 0);
		}
	}

	return true;
}

void CLUSTERED::Update()
{

}

void CLUSTERED::Draw()
{
	clusteredLightAssignment();

	auto b = ConstantBuffer.get();

	CRenderer::GetDeviceContext()->PSSetConstantBuffers(6, 1, &b);

	auto r = ShaderResourceView.get();

	CRenderer::GetDeviceContext()->PSSetShaderResources(2, 1, &r);
}

void CLUSTERED::Uninit()
{
	Resource.reset(nullptr);
	ConstantBuffer.reset(nullptr);
	ShaderResourceView.reset(nullptr);
}


void CLUSTERED::clusteredLightAssignment()
{
	UINT lights[CLUSTER_Z][CLUSTER_Y][CLUSTER_X] = { 0 };

	XMFLOAT3 scale = XMFLOAT3(float(CLUSTER_X), float(CLUSTER_Y), float(CLUSTER_Z)) / (AABB_max - AABB_min);
	XMFLOAT3 inv_scale = 1.0f / scale;

	for (UINT i = 0; i < LIGHT_COUNT; i++)
	{
		const Light& light = m_Lights[i];

		const XMFLOAT3 p = (light.Position - AABB_min);
		const XMFLOAT3 p_min = (p - light.Radius) * scale;
		const XMFLOAT3 p_max = (p + light.Radius) * scale;

		// Cluster for the center of the light
		const int px = (int)floorf(p.x * scale.x);
		const int py = (int)floorf(p.y * scale.y);
		const int pz = (int)floorf(p.z * scale.z);

		// Cluster bounds for the light
		const int x0 = max((UINT)floorf(p_min.x), (UINT)0);
		const int x1 = min((UINT)ceilf(p_max.x), CLUSTER_X);
		const int y0 = max((UINT)floorf(p_min.y), (UINT)0);
		const int y1 = min((UINT)ceilf(p_max.y), CLUSTER_Y);
		const int z0 = max((UINT)floorf(p_min.z), (UINT)0);
		const int z1 = min((UINT)ceilf(p_max.z), CLUSTER_Z);

		const float radius_sqr = light.Radius * light.Radius;
		const UINT mask = (1 << i);

		// Do AABB<->Sphere tests to figure out which clusters are actually intersected by the light
		for (int z = z0; z < z1; z++)
		{
			float dz = (pz == z) ? 0.0f : AABB_min.z + (pz < z ? z : z + 1) * inv_scale.z - light.Position.z;
			dz *= dz;

			for (int y = y0; y < y1; y++)
			{
				float dy = (py == y) ? 0.0f : AABB_min.y + (py < y ? y : y + 1) * inv_scale.y - light.Position.y;
				dy *= dy;
				dy += dz;

				for (int x = x0; x < x1; x++)
				{
					float dx = (px == x) ? 0.0f : AABB_min.x + (px < x ? x : x + 1) * inv_scale.x - light.Position.x;
					dx *= dx;
					dx += dy;

					if (dx < radius_sqr)
					{
						lights[z][y][x] |= mask;
					}
				}
			}
		}
	}

	// Upload the cluster data to a volume texture
	ID3D11Resource* tex = Resource.get();

	D3D11_MAPPED_SUBRESOURCE map;
	CRenderer::GetDeviceContext()->Map(tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	for (UINT z = 0; z < CLUSTER_Z; z++)
	{
		for (UINT y = 0; y < CLUSTER_Y; y++)
		{
			memcpy(((BYTE*)map.pData) + z * map.DepthPitch + y * map.RowPitch, lights[z][y], CLUSTER_X * sizeof(UINT));
		}
	}
	CRenderer::GetDeviceContext()->Unmap(tex, 0);
}