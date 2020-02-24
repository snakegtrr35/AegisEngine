#include	"Clustered.h"

#include	"manager.h"
#include	"Scene_Manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"
#include	"Renderer.h"
#include	"camera.h"
#include	"Debug_Camera.h"

CLUSTERED::CLUSTERED() : Position(XMFLOAT3(0.f, 0.f, 0.f)), Rotation(XMFLOAT3(0.f, 0.f, 0.f))
{
}

bool CLUSTERED::Init()
{
	HRESULT hr;
	auto device = CRenderer::GetDevice();

	{
		Init_Frustum();
		Init_Aabb();

		// インデックスバッファの設定
		if (nullptr == pIndexBuffer.get())
		{
			const WORD IndexNum = 24;

			ID3D11Buffer* buffer;

			const WORD index[IndexNum] = {
			0, 1,
			1, 3,
			3, 2,
			2, 0,

			0, 4,

			4, 5,
			5, 7,
			7, 6,
			6, 4,

			5, 1,
			3, 7,
			6, 2
			};

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer.reset(buffer);
		}
	}


	
	/*// Light index list in texture buffer
	{
		{
			D3D11_TEXTURE1D_DESC desc;
			desc.Width = 16384;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_R16_UINT;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.ArraySize = 1;

			array<UINT, 16384> array;
			{

				for (UINT i = 0; i < array.size(); i++)
				{
					array[i] = (UINT)(std::pow((UINT)2, i));
				}

			}

			ID3D11Texture1D* tex = nullptr;
			hr = device->CreateTexture1D(&desc, nullptr, &tex);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			Light_Index_List.reset(tex);
		}

		{
			D3D11_TEXTURE1D_DESC desc1d;
			((ID3D11Texture1D*)Light_Index_List.get())->GetDesc(&desc1d);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

			srvDesc.Format = desc1d.Format;
			if (desc1d.ArraySize > 1)
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture1DArray.FirstArraySlice = 0;
				srvDesc.Texture1DArray.ArraySize = desc1d.ArraySize;
				srvDesc.Texture1DArray.MostDetailedMip = 0;
				srvDesc.Texture1DArray.MipLevels = 1;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				srvDesc.Texture1D.MostDetailedMip = 0;
				srvDesc.Texture1D.MipLevels = desc1d.MipLevels;
			}

			ID3D11ShaderResourceView* srv;
			hr = device->CreateShaderResourceView(Light_Index_List.get(), &srvDesc, &srv);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			Light_Index_ListSRV.reset(srv);
		}
	}*/

	// Cluster “pointers” in 3D texture
	{
		{
			D3D11_TEXTURE3D_DESC desc;
			desc.Width = CLUSTER_X;
			desc.Height = CLUSTER_Y;
			desc.Depth = CLUSTER_Z;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;

			ID3D11Texture3D* tex = nullptr;
			hr = device->CreateTexture3D(&desc, nullptr, &tex);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			ClusteredTexture.reset(tex);
		}

		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

			D3D11_TEXTURE3D_DESC desc3d;
			((ID3D11Texture3D*)ClusteredTexture.get())->GetDesc(&desc3d);

			srvDesc.Format = desc3d.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			srvDesc.Texture3D.MostDetailedMip = 0;
			srvDesc.Texture3D.MipLevels = desc3d.MipLevels;

			ID3D11ShaderResourceView* srv;
			hr = device->CreateShaderResourceView(ClusteredTexture.get(), &srvDesc, &srv);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			ClusteredSRV.reset(srv);
		}
	}

	return true;
}

void CLUSTERED::Update()
{
	OverWrite_Frustum();
	OverWrite_Aabb();

	/*{
		

		// バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(Light_Index_List.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			//memcpy(msr.pData, Vertex, sizeof(WORD) * 4);
			CRenderer::GetDeviceContext()->Unmap(Light_Index_List.get(), 0);
		}
	}*/

	{
		XMUINT4 clustered[CLUSTER_Z][CLUSTER_Y][CLUSTER_X] = { XMUINT4(0, 0, 0, 0) };

		auto lights = SCENE::Get_Light_Manager();
		const auto light_list = lights->Get_Lights();



		XMFLOAT3 size;
		XMFLOAT3 min, max;

		XMFLOAT3 corners[BoundingBox::CORNER_COUNT];
		Aabb.GetCorners(corners);

		max.x = std::max({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
		max.y = std::max({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
		max.z = std::max({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

		min.x = std::min({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
		min.y = std::min({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
		min.z = std::min({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

		size.x = (max.x - min.x) / CLUSTER_X;
		size.y = (max.y - min.y) / CLUSTER_Y;
		size.z = (max.z - min.z) / CLUSTER_Z;

		for (UINT i = 0; i < light_list->size(); i++)
		{
			const LIGHT_BUFFER light = light_list->at(i);

			const UINT mask = ( 1 << (i % UINT(32)) );

			if (light.Enable)
			{
				for (UINT z = 0; z < CLUSTER_Z; z++)
				{
					//

					for (UINT y = 0; y < CLUSTER_Y; y++)
					{
						//

						for (UINT x = 0; x < CLUSTER_X; x++)
						{
							//
							if (96 <= i)
							{
								//if (dx < radius_sqr)
								{
									clustered[z][y][x].w |= mask;
								}
								continue;
							}

							if (64 <= i)
							{
								//if (dx < radius_sqr)
								{
									clustered[z][y][x].z |= mask;
								}
								continue;
							}

							if (32 <= i)
							{
								//if (dx < radius_sqr)
								{
									clustered[z][y][x].y |= mask;
								}
								continue;
							}

							{
								//if (dx < radius_sqr)
								{
									clustered[z][y][x].x |= mask;
								}
								continue;
							}
						}
					}
				}
			}
		}

		// バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(ClusteredTexture.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			for (UINT z = 0; z < CLUSTER_Z; z++)
			{
				for (UINT y = 0; y < CLUSTER_Y; y++)
				{
					memcpy(static_cast<XMUINT4*>(msr.pData) + z * msr.DepthPitch + y * msr.RowPitch, clustered[z][y], sizeof(XMUINT4) * CLUSTER_X);
				}
			}
			CRenderer::GetDeviceContext()->Unmap(ClusteredTexture.get(), 0);
		}
	}
}

void CLUSTERED::Draw()
{
}

void CLUSTERED::Uninit()
{	
	ClusteredTexture.reset(nullptr);
	ClusteredSRV.reset(nullptr);

	Light_Index_List.reset(nullptr);
	Light_Index_ListSRV.reset(nullptr);
}

void CLUSTERED::Init_Aabb()
{
	XMFLOAT3 min, max;

	// 頂点バッファの設定
	{
		COLOR color = COLOR(0.f, 0.f, 1.0f, 1.f);
		XMFLOAT3 corners[BoundingBox::CORNER_COUNT];
		VERTEX_3D Vertex[BoundingFrustum::CORNER_COUNT] = {};

		Frustum.GetCorners(corners);

		XMFLOAT3 pos[BoundingFrustum::CORNER_COUNT] = { corners[0] };

		max.x = std::max({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
		max.y = std::max({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
		max.z = std::max({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

		min.x = std::min({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
		min.y = std::min({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
		min.z = std::min({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

		{
			Aabb = BoundingBox(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3((max.x - min.x) * 0.5f, (max.y - min.y) * 0.5f, (max.z - min.z) * 0.5f));

			XMMATRIX matrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
			matrix *= XMMatrixTranslation((max.x + min.x) * 0.5f, (max.y + min.y) * 0.5f, (max.z + min.z) * 0.5f);

			Aabb.Transform(Aabb, matrix);
		}

		{
			Aabb.GetCorners(corners);

			VERTEX_3D Vertex[BoundingBox::CORNER_COUNT];

			Vertex[0].Position = corners[7];
			Vertex[1].Position = corners[6];
			Vertex[2].Position = corners[4];
			Vertex[3].Position = corners[5];


			Vertex[4].Position = corners[3];
			Vertex[5].Position = corners[2];
			Vertex[6].Position = corners[0];
			Vertex[7].Position = corners[1];
		}

		for (char i = 0; i < BoundingBox::CORNER_COUNT; i++)
		{
			Vertex[i].Position = pos[i];
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(color.r, color.g, color.b, color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * BoundingBox::CORNER_COUNT;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer_BOX.reset(buffer);
		}
	}
}

void CLUSTERED::Init_Frustum()
{
	{
		BoundingFrustum::CreateFromMatrix(Frustum, XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f + 35.0f), float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 30.0f));
		Frustum.Origin.z = 0.0f;
		{
			XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
			matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

			Frustum.Transform(Frustum, matrix);
		}
	}

	// 頂点バッファの設定
	{
		const WORD VertexNum = 8;
		COLOR color = COLOR(1.f, 0.f, 0.f, 1.f);
		VERTEX_3D Vertex[VertexNum];

		XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
		Frustum.GetCorners(corners);

		Vertex[0].Position = corners[1];
		Vertex[1].Position = corners[0];
		Vertex[2].Position = corners[2];
		Vertex[3].Position = corners[3];
		Vertex[4].Position = corners[5];
		Vertex[5].Position = corners[4];
		Vertex[6].Position = corners[6];
		Vertex[7].Position = corners[7];

		for (char i = 0; i < VertexNum; i++)
		{
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(color.r, color.g, color.b, color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer_Frustum.reset(buffer);
		}
	}
}

void CLUSTERED::Draw_Aabb()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable()) return;

	// 3Dマトリックス設定
	{

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			CRenderer::Set_MatrixBuffer(XMMatrixIdentity(), camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(XMMatrixIdentity(), camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	// 入力アセンブラに頂点バッファを設定
	CRenderer::SetVertexBuffers(pVertexBuffer_BOX.get());

	CRenderer::SetIndexBuffer(pIndexBuffer.get());

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	CRenderer::GetDeviceContext()->DrawIndexed(24, 0, 0);

	CRenderer::Set_Shader();
}

void CLUSTERED::Draw_Frustum()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable()) return;

	// 3Dマトリックス設定
	{
		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			CRenderer::Set_MatrixBuffer(XMMatrixIdentity(), camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

			CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(XMMatrixIdentity(), camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

			CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
		}
	}

	// 頂点バッファ設定
	CRenderer::SetVertexBuffers(pVertexBuffer_Frustum.get());

	// インデックスバッファ設定
	CRenderer::SetIndexBuffer(pIndexBuffer.get());

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	CRenderer::GetDeviceContext()->DrawIndexed(24, 0, 0);

	CRenderer::Set_Shader();
}

void CLUSTERED::OverWrite_Aabb()
{
	{
		COLOR color = COLOR(0.f, 0.f, 1.0f, 1.0f);
		XMFLOAT3 corners[BoundingBox::CORNER_COUNT];
		VERTEX_3D Vertex[BoundingBox::CORNER_COUNT] = {};

		Frustum.GetCorners(corners);

		XMFLOAT3 pos[BoundingFrustum::CORNER_COUNT];
		XMFLOAT3 min, max;

		max.x = std::max({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
		max.y = std::max({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
		max.z = std::max({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

		min.x = std::min({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
		min.y = std::min({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
		min.z = std::min({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

		{
			Aabb = BoundingBox(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3((max.x - min.x) * 0.5f, (max.y - min.y) * 0.5f, (max.z - min.z) * 0.5f));

			XMMATRIX matrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
			matrix *= XMMatrixTranslation((max.x + min.x) * 0.5f, (max.y + min.y) * 0.5f, (max.z + min.z) * 0.5f);

			Aabb.Transform(Aabb, matrix);
		}

		{
			{
				pos[0].z = min.z;
				pos[1].z = min.z;
				pos[2].z = min.z;
				pos[3].z = min.z;

				pos[4].z = max.z;
				pos[5].z = max.z;
				pos[6].z = max.z;
				pos[7].z = max.z;
			}
			{
				pos[0].x = min.x;
				pos[1].x = max.x;
				pos[2].x = min.x;
				pos[3].x = max.x;

				pos[4].x = min.x;
				pos[5].x = max.x;
				pos[6].x = min.x;
				pos[7].x = max.x;
			}
			{
				pos[0].y = min.y;
				pos[1].y = min.y;
				pos[2].y = max.y;
				pos[3].y = max.y;

				pos[4].y = min.y;
				pos[5].y = min.y;
				pos[6].y = max.y;
				pos[7].y = max.y;
			}
		}

		for (char i = 0; i < BoundingBox::CORNER_COUNT; i++)
		{
			Vertex[i].Position = pos[i];
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(color.r, color.g, color.b, color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(pVertexBuffer_BOX.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * BoundingBox::CORNER_COUNT);
			CRenderer::GetDeviceContext()->Unmap(pVertexBuffer_BOX.get(), 0);
		}
	}
}

void CLUSTERED::OverWrite_Frustum()
{
	// 頂点バッファの設定
	{
		const WORD VertexNum = 8;

		VERTEX_3D Vertex[VertexNum];
		COLOR color = COLOR(1.f, 0.f, 0.f, 1.f);
		XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
		Frustum.GetCorners(corners);

		Vertex[0].Position = corners[1];
		Vertex[1].Position = corners[0];
		Vertex[2].Position = corners[2];
		Vertex[3].Position = corners[3];

		Vertex[4].Position = corners[5];
		Vertex[5].Position = corners[4];
		Vertex[6].Position = corners[6];
		Vertex[7].Position = corners[7];

		for (char i = 0; i < VertexNum; i++)
		{
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(color.r, color.g, color.b, color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer_Frustum.reset(buffer);
		}
	}
}