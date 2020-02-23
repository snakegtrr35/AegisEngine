#include	"Bounding_Aabb.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

#include	"Bounding_Frustum.h"
extern unique_ptr<BOUNDING_FRUSTUM> Frustum;

BOUNDING_AABB::~BOUNDING_AABB()
{
	Uninit();
}

void BOUNDING_AABB::Init()
{
	{
		//XMFLOAT3 pos = *Owner.lock()->Get_Position();

		Aabb = BoundingBox(XMFLOAT3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		//matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);
		matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		Aabb.Transform(Aabb, matrix);
	}

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		XMFLOAT3 corners[BoundingBox::CORNER_COUNT];
		VERTEX_3D Vertex[BoundingFrustum::CORNER_COUNT] = {};

		/*{
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
		}*/

		{
			auto frustum = Frustum->Get_Collition();

			frustum.GetCorners(corners);

			XMFLOAT3 pos[BoundingFrustum::CORNER_COUNT] = { corners[0] };

			XMFLOAT3 min, max;

			max.x = std::max({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
			max.y = std::max({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
			max.z = std::max({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

			min.x = std::min({ corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x });
			min.y = std::min({ corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y });
			min.z = std::min({ corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z });

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

			/*Vertex[0].Position = corners[7];
			Vertex[1].Position = corners[6];
			Vertex[2].Position = corners[4];
			Vertex[3].Position = corners[5];


			Vertex[4].Position = corners[3];
			Vertex[5].Position = corners[2];
			Vertex[6].Position = corners[0];
			Vertex[7].Position = corners[1];*/

			for (int i = 0; i < BoundingFrustum::CORNER_COUNT; i++)
			{
				Vertex[i].Position = pos[i];
			}
		}

		for (char i = 0; i < BoundingBox::CORNER_COUNT; i++)
		{
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
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

			pVertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer_BOX.get())
	{
		ID3D11Buffer* buffer;

		const WORD index[] = {
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

		bd.ByteWidth = sizeof(WORD) * IndexNum_Box;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		pIndexBuffer_BOX.reset(buffer);
	}
}

void BOUNDING_AABB::Draw()
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
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
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		CRenderer::SetIndexBuffer(pIndexBuffer_BOX.get());

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		CRenderer::GetDeviceContext()->DrawIndexed(IndexNum_Box, 0, 0);

		CRenderer::Set_Shader();
	}
}

void BOUNDING_AABB::Update(float delta_time)
{
	OverWrite();
}

void BOUNDING_AABB::Uninit()
{
	pVertexBuffer.reset(nullptr);
}

void BOUNDING_AABB::OverWrite()
{
	if (nullptr != pVertexBuffer.get())
	{
		Color = COLOR(0.f, 0.f, 1.0f, 1.0f);

		//MFLOAT3 pos = *Owner.lock()->Get_Position();

		Aabb = BoundingBox(XMFLOAT3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		//matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);
		matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		Aabb.Transform(Aabb, matrix);

		XMFLOAT3 corners[BoundingBox::CORNER_COUNT];
		VERTEX_3D Vertex[BoundingBox::CORNER_COUNT] = {};

		/*{
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
		}*/

		{
			auto frustum = Frustum->Get_Collition();

			frustum.GetCorners(corners);

			XMFLOAT3 pos[BoundingFrustum::CORNER_COUNT];

			XMFLOAT3 min, max;

			max.x = std::max( { corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x } );
			max.y = std::max( { corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y } );
			max.z = std::max( { corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z } );

			min.x = std::min( { corners[0].x, corners[1].x, corners[2].x, corners[3].x, corners[4].x, corners[5].x, corners[6].x, corners[7].x } );
			min.y = std::min( { corners[0].y, corners[1].y, corners[2].y, corners[3].y, corners[4].y, corners[5].y, corners[6].y, corners[7].y } );
			min.z = std::min( { corners[0].z, corners[1].z, corners[2].z, corners[3].z, corners[4].z, corners[5].z, corners[6].z, corners[7].z } );

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

			/*Vertex[0].Position = corners[7];
			Vertex[1].Position = corners[6];
			Vertex[2].Position = corners[4];
			Vertex[3].Position = corners[5];


			Vertex[4].Position = corners[3];
			Vertex[5].Position = corners[2];
			Vertex[6].Position = corners[0];
			Vertex[7].Position = corners[1];*/

			for (int i = 0; i < BoundingBox::CORNER_COUNT; i++)
			{
				Vertex[i].Position = pos[i];
			}
		}

		for (char i = 0; i < BoundingBox::CORNER_COUNT; i++)
		{
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(pVertexBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * BoundingBox::CORNER_COUNT);
			CRenderer::GetDeviceContext()->Unmap(pVertexBuffer.get(), 0);
		}
	}
}

void BOUNDING_AABB::Set_Radius(const XMFLOAT3& radius)
{
	Radius = radius;
}

void BOUNDING_AABB::Set_Radius(const XMFLOAT3* radius)
{
	Radius = *radius;
}

XMFLOAT3* BOUNDING_AABB::Get_Radius()
{
	return &Radius;
}

const BoundingBox& BOUNDING_AABB::Get_Collition()
{
	return Aabb;
}


#include	"imgui/imgui.h"

void BOUNDING_AABB::Draw_Inspector()
{
	auto str = (char*)u8"コリジョン(AABB)";

	ImGui::Text(str);

	COMPONENT::Draw_Inspector();

	float position[3] = { Position.x, Position.y, Position.z };
	float scale[3] = { Scaling.x, Scaling.y, Scaling.z };
	float radius[3] = { Radius.x, Radius.y, Radius.z };

	ImGui::DragFloat3("Position##AABB", position, 0.01f);
	ImGui::DragFloat3("Scaling", scale, 0.01f);
	ImGui::DragFloat3("Radius", radius, 0.01f, 0.01f, 1000.0f);

	Position = XMFLOAT3(position[0], position[1], position[2]);
	Scaling = XMFLOAT3(scale[0], scale[1], scale[2]);
	Radius = XMFLOAT3(radius[0], radius[1], radius[2]);

	OverWrite();
}