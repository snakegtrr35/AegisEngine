#include	"Bounding_Obb.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"


BOUNDING_OBB::~BOUNDING_OBB()
{
	Uninit();
}

void BOUNDING_OBB::Init()
{
	{
		XMFLOAT3 pos = *Owner.lock()->Get_Position();

		Obb = BoundingOrientedBox(XMFLOAT3(0.f, 0.f, 0.f), Radius, Quaternion);

		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Obb.Transform(Obb, matrix);
	}

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		VERTEX_3D Vertex[BoundingBox::CORNER_COUNT];
		XMFLOAT3 corners[BoundingBox::CORNER_COUNT];

		Obb.GetCorners(corners);

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

void BOUNDING_OBB::Draw()
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定.
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		CRenderer::SetIndexBuffer(pIndexBuffer_BOX.get());

		// 3Dマトリックス設定
		{
			XMFLOAT3 pos = *Owner.lock()->Get_Position();

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				CRenderer::Set_MatrixBuffer(XMMatrixIdentity(), camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

				CRenderer::Set_Shader();
			}
			else
			{
				CRenderer::Set_MatrixBuffer(XMMatrixIdentity(), camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

				CRenderer::Set_Shader();
			}
		}

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		CRenderer::GetDeviceContext()->DrawIndexed(IndexNum_Box, 0, 0);

		CRenderer::Set_Shader();
	}
}

void BOUNDING_OBB::Update(float delta_time)
{
	OverWrite();
}

void BOUNDING_OBB::Uninit()
{
}

void BOUNDING_OBB::OverWrite()
{
	{
		XMFLOAT3 pos = *Owner.lock()->Get_Position();

		Obb = BoundingOrientedBox(XMFLOAT3(0.f, 0.f, 0.f), Radius, Quaternion);

		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Obb.Transform(Obb, matrix);
	}

	if (nullptr != pVertexBuffer.get())
	{
		Color = Default_Color;

		VERTEX_3D Vertex[BoundingBox::CORNER_COUNT];
		XMFLOAT3 corners[BoundingBox::CORNER_COUNT];

		Obb.GetCorners(corners);

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
			Vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
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

const BoundingOrientedBox& BOUNDING_OBB::Get_Collition()
{
	return Obb;
}

#include	"imgui/imgui.h"

void BOUNDING_OBB::Draw_Inspector()
{
	ImGui::Text((char*)u8"コリジョン(OBB)");

	COMPONENT::Draw_Inspector();

	float position[3] = { Position.x, Position.y, Position.z };
	float rotation[3] = { Rotation.x, Rotation.y, Rotation.z };
	float scale[3] = { Scaling.x, Scaling.y, Scaling.z };
	float radius[3] = { Radius.x, Radius.y, Radius.z };

	ImGui::DragFloat3("Position##OBB", position, 0.01f);
	ImGui::DragFloat3("Rotate", rotation, 0.1f);
	ImGui::DragFloat3("Scaling", scale, 0.01f);
	ImGui::DragFloat3("Radius", radius, 0.01f, 0.01f, 1000.0f);

	Position = XMFLOAT3(position[0], position[1], position[2]);
	Rotation = XMFLOAT3(rotation[0], rotation[1], rotation[2]);
	Scaling = XMFLOAT3(scale[0], scale[1], scale[2]);
	Radius = XMFLOAT3(radius[0], radius[1], radius[2]);

	{
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));

		XMStoreFloat4(&Quaternion, quat);
	}

	OverWrite();
}