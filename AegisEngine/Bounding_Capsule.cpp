#include	"Bounding_Capsule.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

using namespace aegis;

void BOUNDING_CAPSULE::Init()
{
	Radius = 3.0f;
	Height = 6.0f;

	Color = Default_Color;

	Init_Body();
	Init_Ring();

}

void BOUNDING_CAPSULE::Draw()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable()) return;

	//Vector3 pos(Position.x, Position.y, Position.z);
	//Vector3 rotation(Rotation.x, Rotation.y, Rotation.z);

	//CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	//Vector3 p(pos.x, pos.y, pos.z);

	{
		Vector3 pos = *Owner.lock()->Get_Transform().Get_Position();
		Vector3 rota = *Owner.lock()->Get_Transform().Get_Rotation();

		pos = pos + Position;
		rota = rota + Rotation;

		Draw_Body(pos, rota);
	}

	//Draw_Body(p, rotation);
	//Draw_Body(pos, Rotation);

	//CRenderer::Set_Shader();
}

void BOUNDING_CAPSULE::Update(float delta_time)
{
	//OverWrite();
}

void BOUNDING_CAPSULE::Uninit()
{
}

void BOUNDING_CAPSULE::OverWrite()
{
	Color = Default_Color;

	Init_Body();
	Init_Ring();
}

void BOUNDING_CAPSULE::Set_Radius(const float radius)
{
	Radius = std::max(radius, 0.01f);
}

const float BOUNDING_CAPSULE::Get_Radius()
{
	return Radius;
}

void BOUNDING_CAPSULE::Init_Body()
{
	CRenderer* render = CRenderer::getInstance();

	int vertex_num = std::max(int(10 * (UINT)Radius * 0.5), 8);

	// 頂点バッファの設定
	{
		vector<VERTEX_3D> Vertex;
		VERTEX_3D vertex;
		Vertex.reserve(vertex_num * 2 + 1);

		const float angle = Math::PI / (vertex_num - 1);

		// 上の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = Vector3(cosf(angle * i) * Radius, sinf(angle * i) * Radius + Height - Radius, 0.f);
			vertex.Normal = Vector3(0.f, 0.f, 0.f);
			vertex.Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = Vector2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 下の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = Vector3(cosf(angle * (i + vertex_num - 1)) * Radius, sinf(angle * (i + vertex_num - 1)) * Radius - Height + Radius, 0.f);
			vertex.Normal = Vector3(0.f, 0.f, 0.f);
			vertex.Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = Vector2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 最後の点
		vertex.Position = Vector3(cosf(0) * Radius, sinf(0) * Radius + Height - Radius, 0.f);
		vertex.Normal = Vector3(0.f, 0.f, 0.f);
		vertex.Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		vertex.TexCoord = Vector2(0.f, 0.f);

		Vertex.emplace_back(vertex);

		vertex_num *= 2;
		vertex_num += 1;

		// 頂点バッファの設定
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(VERTEX_3D) * vertex_num;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = Vertex.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
		}
	}

	// 頂点バッファの設定
	{
		vertex_num = std::max(int(10 * (UINT)Radius * 0.5), 8);

		vector<VERTEX_3D> Vertex;
		VERTEX_3D vertex;
		Vertex.reserve(vertex_num * 2 + 1);

		const float angle = Math::PI / (vertex_num - 1);

		// 上の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = Vector3(0.f, sinf(angle * i) * Radius + Height - Radius, cosf(angle * i) * Radius);
			vertex.Normal = Vector3(0.f, 0.f, 0.f);
			vertex.Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = Vector2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 下の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = Vector3(0.f, sinf(angle * (i + vertex_num - 1)) * Radius - Height + Radius, cosf(angle * (i + vertex_num - 1)) * Radius);
			vertex.Normal = Vector3(0.f, 0.f, 0.f);
			vertex.Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = Vector2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 最後の点
		vertex.Position = Vector3(0.f, sinf(0) * Radius + Height - Radius, cosf(0) * Radius);
		vertex.Normal = Vector3(0.f, 0.f, 0.f);
		vertex.Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
		vertex.TexCoord = Vector2(0.f, 0.f);

		Vertex.emplace_back(vertex);

		vertex_num *= 2;
		vertex_num += 1;

		// 頂点バッファの設定
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(VERTEX_3D) * vertex_num;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = Vertex.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer2);
		}
	}

	// インデックスバッファの設定
	{
		IndexNum = vertex_num * 2;
		vector<WORD> Index;
		Index.reserve(IndexNum);

		for (int i = 0; i < vertex_num; i++)
		{
			Index.emplace_back(i);
			Index.emplace_back((i + 1) % vertex_num);
		}

		// インデックスバッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = Index.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pIndexBuffer);
		}
	}
}

void BOUNDING_CAPSULE::Init_Ring()
{
	CRenderer* render = CRenderer::getInstance();

	// 頂点バッファの設定
	{
		const int cnt = 10 * (UINT)Radius;

		vector<VERTEX_3D> Vertex;
		VERTEX_3D vertex;
		Vertex.reserve(cnt);

		const float angle = Math::PI2 / cnt;

		for (int i = 0; i < cnt; i++)
		{
			vertex.Position = Vector3(cosf(angle * i) * Radius, 0.f, sinf(angle * i) * Radius);
			vertex.Normal = Vector3(0.f, 0.f, 0.f);
			vertex.Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = Vector2(0.f, 0.f);

			Vertex.emplace_back(vertex);
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
			sd.pSysMem = Vertex.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer_Ring);
		}
	}

	// インデックスバッファの設定
	{
		const int cnt = 10 * (UINT)Radius;

		IndexNum_Ring = cnt * 2;
		vector<WORD> Index;
		Index.reserve(IndexNum_Ring);

		for (int i = 0; i < cnt; i++)
		{
			Index.emplace_back(i);
			Index.emplace_back((i + 1) % cnt);
		}

		// インデックスバッファの設定
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(WORD) * IndexNum_Ring;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = Index.data();

			render->GetDevice()->CreateBuffer(&bd, &sd, &pIndexBuffer_Ring);
		}

	}
}

void BOUNDING_CAPSULE::Draw_Body(const Vector3& position, const Vector3& rotation)
{
	CRenderer* render = CRenderer::getInstance();

	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	// トポロジの設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 本体の描画
	{
		// 入力アセンブラにインデックスバッファを設定
		render->SetIndexBuffer(pIndexBuffer.Get());

		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(pVertexBuffer.Get());

		Draw_Semicircle(position, rotation);

		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(pVertexBuffer2.Get());

		Draw_Semicircle(position, rotation);
	}

	// 上下のリングの描画
	{
		Draw_Ring(position, rotation);
	}

	render->Set_Shader();

	// トポロジの設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void BOUNDING_CAPSULE::Draw_Semicircle(const Vector3& position, const Vector3& rotation)
{
	CRenderer* render = render->getInstance();

	const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
	const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMMATRIX world /*= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z)*/ = XMMatrixIdentity();
	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y - 10.0f), XMConvertToRadians(rotation.z));
	world *= XMMatrixTranslation(position.x, position.y, position.z);

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

	render->DrawIndexed(IndexNum, 0, 0);
}

void BOUNDING_CAPSULE::Draw_Ring(const Vector3& position, const Vector3& rotation)
{
	CRenderer* render = CRenderer::getInstance();

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer_Ring.Get());

	// 入力アセンブラにインデックスバッファを設定
	render->SetIndexBuffer(pIndexBuffer_Ring.Get());

	// 3Dマトリックス設定
	{
		XMMATRIX world /*= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z)*/ = XMMatrixIdentity();
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y - 10.0f), XMConvertToRadians(rotation.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

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

	render->DrawIndexed(IndexNum_Ring, 0, 0);
}

#include	"imgui/imgui.h"

void BOUNDING_CAPSULE::Draw_Inspector()
{
	auto str = (char*)u8"コリジョン(カプセル)";

	ImGui::Text(str);

	COMPONENT::Draw_Inspector();

	float position[3] = { Position.x, Position.y, Position.z };
	float scale[3] = { Scaling.x, Scaling.y, Scaling.z };
	float Rotate[3] = { Rotation.x, Rotation.y, Rotation.z };

	ImGui::DragFloat3("Position##CAPSULE", position, 0.01f);
	ImGui::DragFloat3("Scaling##CAPSULE", scale, 0.01f);
	ImGui::DragFloat3("Rotate##CAPSULE", Rotate, 0.1f);
	ImGui::DragFloat("Radius##CAPSULE", &Radius, 0.01f, 0.01f, 1000.0f);
	ImGui::DragFloat("Height##CAPSULE", &Height, 0.01f, Radius, 1000.0f);

	Position = Vector3(position[0], position[1], position[2]);
	Scaling = Vector3(scale[0], scale[1], scale[2]);
	Rotation = Vector3(Rotate[0], Rotate[1], Rotate[2]);

	if (Height < Radius) Height = Radius;

	OverWrite();
}