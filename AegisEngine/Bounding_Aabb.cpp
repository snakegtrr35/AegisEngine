#include	"Bounding_Aabb.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

using namespace Aegis;

BOUNDING_AABB::~BOUNDING_AABB()
{
	Uninit();
}

void BOUNDING_AABB::Init()
{
	CRenderer* render = CRenderer::getInstance();

	{
		Vector3 pos = Vector3();

		if (false == Owner.expired())
		{
			pos = *Owner.lock()->Get_Transform().Get_Position();
		}

		Aabb = BoundingBox(Vector3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Aabb.Transform(Aabb, matrix);
	}

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer)
	{
		Vector3 corners[BoundingBox::CORNER_COUNT];

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

		for (char i = 0; i < BoundingBox::CORNER_COUNT; i++)
		{
			Vertex[i].Normal = Vector3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].TexCoord = Vector2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(VERTEX_3D) * BoundingBox::CORNER_COUNT;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = Vertex;

			render->GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer_BOX)
	{
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

		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(WORD) * IndexNum_Box;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = index;

		render->GetDevice()->CreateBuffer(&bd, &sd, &pIndexBuffer_BOX);
	}
}

void BOUNDING_AABB::Draw()
{
	//if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		CRenderer* render = CRenderer::getInstance();

		// 3Dマトリックス設定
		{

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				render->Set_MatrixBuffer(XMMatrixIdentity(), camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
			}
			else
			{
				render->Set_MatrixBuffer(XMMatrixIdentity(), camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
			}
		}

		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(pVertexBuffer.Get());

		render->SetIndexBuffer(pIndexBuffer_BOX.Get());

		// トポロジの設定
		render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		render->DrawIndexed(IndexNum_Box, 0, 0);

		render->Set_Shader();

		// トポロジの設定
		render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

void BOUNDING_AABB::Update(float delta_time)
{
	OverWrite();
}

void BOUNDING_AABB::Uninit()
{
}

void BOUNDING_AABB::OverWrite()
{
	if (nullptr != pVertexBuffer)
	{
		Color = Default_Color;

		Vector3 pos = *Owner.lock()->Get_Transform().Get_Position();

		Aabb = BoundingBox(Vector3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Aabb.Transform(Aabb, matrix);

		VERTEX_3D Vertex[BoundingBox::CORNER_COUNT];
		Vector3 corners[BoundingBox::CORNER_COUNT];

		Aabb.GetCorners(corners);

		Vertex[0].Position = corners[7];

		Vertex[1].Position = corners[6];

		Vertex[2].Position = corners[4];

		Vertex[3].Position = corners[5];


		Vertex[4].Position = corners[3];

		Vertex[5].Position = corners[2];

		Vertex[6].Position = corners[0];

		Vertex[7].Position = corners[1];

		for (char i = 0; i < BoundingBox::CORNER_COUNT; i++)
		{
			Vertex[i].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].Normal = Vector3(1.0f, 0.0f, 0.0f);
			Vertex[i].TexCoord = Vector2(0.0f, 0.0f);
		}

		CRenderer* render = CRenderer::getInstance();

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * BoundingBox::CORNER_COUNT);
			render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
		}
	}
}

void BOUNDING_AABB::Set_Radius(const Vector3& radius)
{
	Radius = radius;
}

void BOUNDING_AABB::Set_Radius(const Vector3* radius)
{
	Radius = *radius;
}

Vector3* BOUNDING_AABB::Get_Radius()
{
	return &Radius;
}

const BoundingBox& BOUNDING_AABB::Get_Collition()
{
	return Aabb;
}

void BOUNDING_AABB::OverWrite(BoundingBox aabb)
{
	Aabb = aabb;

	Color = Default_Color;

	if (nullptr != pVertexBuffer)
	{

		VERTEX_3D Vertex[BoundingBox::CORNER_COUNT];
		Vector3 corners[BoundingBox::CORNER_COUNT];

		Aabb.GetCorners(corners);

		Vertex[0].Position = corners[7];

		Vertex[1].Position = corners[6];

		Vertex[2].Position = corners[4];

		Vertex[3].Position = corners[5];


		Vertex[4].Position = corners[3];

		Vertex[5].Position = corners[2];

		Vertex[6].Position = corners[0];

		Vertex[7].Position = corners[1];

		for (char i = 0; i < BoundingBox::CORNER_COUNT; i++)
		{
			Vertex[i].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].Normal = Vector3(1.0f, 0.0f, 0.0f);
			Vertex[i].TexCoord = Vector2(0.0f, 0.0f);
		}

		CRenderer* render = CRenderer::getInstance();

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * BoundingBox::CORNER_COUNT);
			render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
		}
	}
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
	ImGui::DragFloat3("Scaling##AABB", scale, 0.01f);
	ImGui::DragFloat3("Radius##AABB", radius, 0.01f, 0.01f, 1000.0f);

	Position = Vector3(position[0], position[1], position[2]);
	Scaling = Vector3(scale[0], scale[1], scale[2]);
	Radius = Vector3(radius[0], radius[1], radius[2]);

	OverWrite();
}