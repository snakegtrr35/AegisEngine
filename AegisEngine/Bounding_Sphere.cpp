#include	"Bounding_Sphere.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

using namespace Aegis;

void BOUNDING_SHPERE::Init()
{
	CRenderer* render = CRenderer::getInstance();

	{
		Vector3 pos = *Owner.lock()->Get_Transform().Get_Position();

		Sphere = BoundingSphere(Vector3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix;

		matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Sphere.Transform(Sphere, matrix);
	}

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer)
	{
		const UINT cnt = 20 * max((UINT)1, (UINT)Radius);

		vector<VERTEX_3D> vertex;
		vertex.resize(cnt);

		const float angle = Math::PI2 / cnt;

		for (UINT i = 0; i < cnt; i++)
		{
			vertex[i].Position = Vector3(cosf(angle * i) * Radius, sinf(angle * i) * Radius, 0.0f);
			vertex[i].Normal = Vector3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			vertex[i].TexCoord = Vector2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(VERTEX_3D) * cnt;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = vertex.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer)
	{
		const UINT cnt = 20 * max((UINT)1, (UINT)Radius);

		IndexNum = cnt * 2;
		vector<WORD> index_array;
		index_array.reserve(IndexNum);

		for (UINT i = 0; i < IndexNum; i++)
		{
			index_array.emplace_back(0);
		}

		for (UINT i = 0; i < cnt; i++)
		{
			index_array[i * 2] = (WORD)i;
			index_array[i * 2 + 1] = (WORD)((i + 1) % cnt);
		}

		// インデックスバッファの設定
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = index_array.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pIndexBuffer);
		}
	}
}

void BOUNDING_SHPERE::Draw()
{
	CRenderer* render = CRenderer::getInstance();

	//if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(pVertexBuffer.Get());

		// 入力アセンブラにインデックスバッファを設定
		render->SetIndexBuffer(pIndexBuffer.Get());

		// トポロジの設定
		render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		Draw_Ring(Vector3(0.f, 0.f, 0.f));

		Draw_Ring(Vector3(0.f, 90.0f, 0.f));

		Draw_Ring(Vector3(90.0f, 0.f, 0.f));

		render->Set_Shader();

		// トポロジの設定
		render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

void BOUNDING_SHPERE::Update(float delta_time)
{
	OverWrite();

	{
		Vector3 pos = *Owner.lock()->Get_Transform().Get_Position();

		Sphere = BoundingSphere(Vector3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix;

		matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Sphere.Transform(Sphere, matrix);
	}
}

void BOUNDING_SHPERE::Uninit()
{
}

void BOUNDING_SHPERE::Set_Radius(const float radius)
{
	Radius = max(radius, 0.01f);

	//Create_Buffer();
}

const float BOUNDING_SHPERE::Get_Radius()
{
	return Radius;
}

const BoundingSphere& BOUNDING_SHPERE::Get_Collition()
{
	return Sphere;
}

void BOUNDING_SHPERE::Draw_Ring(const Vector3& rotation)
{
	CRenderer* render = CRenderer::getInstance();
	// 3Dマトリックス設定
	{
		Vector3 pos = *Owner.lock()->Get_Transform().Get_Position();

		XMMATRIX world= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
		world *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

			render->Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

			render->Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
		}
	}

	render->DrawIndexed(IndexNum, 0, 0);
}

void BOUNDING_SHPERE::OverWrite()
{
	CRenderer* render = CRenderer::getInstance();

	Color = Default_Color;

	// 頂点バッファの設定
	{
		const UINT cnt = 20 * max((UINT)1, (UINT)Radius);

		vector<VERTEX_3D> vertex;
		vertex.reserve(cnt);

		for (UINT i = 0; i < cnt; i++)
		{
			vertex.emplace_back(VERTEX_3D());
		}

		const float angle = Math::PI2 / cnt;

		for (UINT i = 0; i < cnt; i++)
		{
			vertex[i].Position = Vector3(cosf(angle * i) * Radius, sinf(angle * i) * Radius, 0.0f);
			vertex[i].Normal = Vector3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			vertex[i].TexCoord = Vector2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(VERTEX_3D) * cnt;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = vertex.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
		}
	}

	// インデックスバッファの設定
	{
		const UINT cnt = 20 * max((UINT)1, (UINT)Radius);

		IndexNum = cnt * 2;
		vector<WORD> index_array;
		index_array.reserve(IndexNum);

		for (UINT i = 0; i < IndexNum; i++)
		{
			index_array.emplace_back(0);
		}

		for (UINT i = 0; i < cnt; i++)
		{
			index_array[i * 2] = (WORD)i;
			index_array[i * 2 + 1] = (WORD)((i + 1) % cnt);
		}

		// インデックスバッファの設定
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = index_array.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pIndexBuffer);
		}
	}
}

#include	"imgui/imgui.h"

void BOUNDING_SHPERE::Draw_Inspector()
{
	ImGui::Text((char*)u8"コリジョン(球)");

	COMPONENT::Draw_Inspector();

	float position[3] = { Position.x, Position.y, Position.z };
	float scale[3] = { Scaling.x, Scaling.y, Scaling.z };

	ImGui::DragFloat3("Position##SPHERE", position, 0.01f);
	ImGui::DragFloat3("Scaling##SPHERE", scale, 0.01f);
	ImGui::DragFloat("Radius##SPHERE", &Radius, 0.01f, 0.01f, 1000.0f);

	Position = Vector3(position[0], position[1], position[2]);
	Scaling = Vector3(scale[0], scale[1], scale[2]);

	OverWrite();
}