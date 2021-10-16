#include	"Bounding_Capsule.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

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

	//XMFLOAT3 pos(Position.x, Position.y, Position.z);
	//XMFLOAT3 rotation(Rotation.x, Rotation.y, Rotation.z);

	//CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	//XMFLOAT3 p(pos.x, pos.y, pos.z);

	{
		XMFLOAT3 pos = *Owner.lock()->Get_Transform().Get_Position();
		XMFLOAT3 rota = *Owner.lock()->Get_Transform().Get_Rotation();

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
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);

	pVertexBuffer2.reset(nullptr);

	pVertexBuffer_Ring.reset(nullptr);
	pIndexBuffer_Ring.reset(nullptr);
}

void BOUNDING_CAPSULE::OverWrite()
{
	Color = Default_Color;

	Init_Body();
	Init_Ring();
}

void BOUNDING_CAPSULE::Set_Radius(const float radius)
{
	Radius = max(radius, 0.01f);
}

const float BOUNDING_CAPSULE::Get_Radius()
{
	return Radius;
}

void BOUNDING_CAPSULE::Init_Body()
{
	int vertex_num = max(int(10 * (UINT)Radius * 0.5), 8);

	// 頂点バッファの設定
	{
		vector<VERTEX_3D> Vertex;
		VERTEX_3D vertex;
		Vertex.reserve(vertex_num * 2 + 1);

		const float angle = XM_PI / (vertex_num - 1);

		// 上の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = XMFLOAT3(cosf(angle * i) * Radius, sinf(angle * i) * Radius + Height - Radius, 0.f);
			vertex.Normal = XMFLOAT3(0.f, 0.f, 0.f);
			vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = XMFLOAT2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 下の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = XMFLOAT3(cosf(angle * (i + vertex_num - 1)) * Radius, sinf(angle * (i + vertex_num - 1)) * Radius - Height + Radius, 0.f);
			vertex.Normal = XMFLOAT3(0.f, 0.f, 0.f);
			vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = XMFLOAT2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 最後の点
		vertex.Position = XMFLOAT3(cosf(0) * Radius, sinf(0) * Radius + Height - Radius, 0.f);
		vertex.Normal = XMFLOAT3(0.f, 0.f, 0.f);
		vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		vertex.TexCoord = XMFLOAT2(0.f, 0.f);

		Vertex.emplace_back(vertex);

		vertex_num *= 2;
		vertex_num += 1;

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * vertex_num;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}
	}

	// 頂点バッファの設定
	{
		vertex_num = max(int(10 * (UINT)Radius * 0.5), 8);

		vector<VERTEX_3D> Vertex;
		VERTEX_3D vertex;
		Vertex.reserve(vertex_num * 2 + 1);

		const float angle = XM_PI / (vertex_num - 1);

		// 上の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = XMFLOAT3(0.f, sinf(angle * i) * Radius + Height - Radius, cosf(angle * i) * Radius);
			vertex.Normal = XMFLOAT3(0.f, 0.f, 0.f);
			vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = XMFLOAT2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 下の半円
		for (int i = 0; i < vertex_num; i++)
		{
			vertex.Position = XMFLOAT3(0.f, sinf(angle * (i + vertex_num - 1)) * Radius - Height + Radius, cosf(angle * (i + vertex_num - 1)) * Radius);
			vertex.Normal = XMFLOAT3(0.f, 0.f, 0.f);
			vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = XMFLOAT2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 最後の点
		vertex.Position = XMFLOAT3(0.f, sinf(0) * Radius + Height - Radius, cosf(0) * Radius);
		vertex.Normal = XMFLOAT3(0.f, 0.f, 0.f);
		vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		vertex.TexCoord = XMFLOAT2(0.f, 0.f);

		Vertex.emplace_back(vertex);

		vertex_num *= 2;
		vertex_num += 1;

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * vertex_num;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer2.reset(buffer);
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
			sd.pSysMem = Index.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer.reset(buffer);
		}
	}
}

void BOUNDING_CAPSULE::Init_Ring()
{
	// 頂点バッファの設定
	{
		const int cnt = 10 * (UINT)Radius;

		vector<VERTEX_3D> Vertex;
		VERTEX_3D vertex;
		Vertex.reserve(cnt);

		const float angle = XM_2PI / cnt;

		for (int i = 0; i < cnt; i++)
		{
			vertex.Position = XMFLOAT3(cosf(angle * i) * Radius, 0.f, sinf(angle * i) * Radius);
			vertex.Normal = XMFLOAT3(0.f, 0.f, 0.f);
			vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = XMFLOAT2(0.f, 0.f);

			Vertex.emplace_back(vertex);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * cnt;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer_Ring.reset(buffer);
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
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(WORD) * IndexNum_Ring;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = Index.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer_Ring.reset(buffer);
		}

	}
}

void BOUNDING_CAPSULE::Draw_Body(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 本体の描画
	{
		// 入力アセンブラにインデックスバッファを設定
		CRenderer::SetIndexBuffer(pIndexBuffer.get());

		// 入力アセンブラに頂点バッファを設定
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		Draw_Semicircle(position, rotation);

		// 入力アセンブラに頂点バッファを設定
		CRenderer::SetVertexBuffers(pVertexBuffer2.get());

		Draw_Semicircle(position, rotation);
	}

	// 上下のリングの描画
	{
		Draw_Ring(position, rotation);
	}

	CRenderer::Set_Shader();
}

void BOUNDING_CAPSULE::Draw_Semicircle(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
	const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMMATRIX world /*= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z)*/ = XMMatrixIdentity();
	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y - 10.0f), XMConvertToRadians(rotation.z));
	world *= XMMatrixTranslation(position.x, position.y, position.z);

	if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
	{
		CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
	}
	else
	{
		CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
	}

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

void BOUNDING_CAPSULE::Draw_Ring(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	// 入力アセンブラに頂点バッファを設定
	CRenderer::SetVertexBuffers(pVertexBuffer_Ring.get());

	// 入力アセンブラにインデックスバッファを設定
	CRenderer::SetIndexBuffer(pIndexBuffer_Ring.get());

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 3Dマトリックス設定
	{
		XMMATRIX world /*= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z)*/ = XMMatrixIdentity();
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y - 10.0f), XMConvertToRadians(rotation.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

			CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

			CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
		}
	}

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum_Ring, 0, 0);
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

	Position = XMFLOAT3(position[0], position[1], position[2]);
	Scaling = XMFLOAT3(scale[0], scale[1], scale[2]);
	Rotation = XMFLOAT3(Rotate[0], Rotate[1], Rotate[2]);

	if (Height < Radius) Height = Radius;

	OverWrite();
}