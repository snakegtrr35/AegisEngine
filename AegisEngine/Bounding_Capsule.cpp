#include	"Bounding_Capsule.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

void BOUNDING_CAPSULE::Init()
{
	Radius = 1.0f;
	Height = 2.0f;

	Color = Default_Color;

	Init_Body();
	Init_Line();
	Init_Ring();

}

void BOUNDING_CAPSULE::Draw()
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		XMFLOAT3 pos(5.0f, 0.0f, 0.0f);
		XMFLOAT3 rotation(0.0f, 0.f, 0.f);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		XMFLOAT3 p(pos.x, pos.y, pos.z);

		Draw_Body(p, rotation);

		rotation.y += 90.0f;

		Draw_Body(p, rotation);

		CRenderer::Set_Shader();
	}
}

void BOUNDING_CAPSULE::Update(float delta_time)
{
}

void BOUNDING_CAPSULE::Uninit()
{
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);

	pVertexBuffer_Line.reset(nullptr);
	pIndexBuffer_Line.reset(nullptr);

	pVertexBuffer_Ring.reset(nullptr);
	pIndexBuffer_Ring.reset(nullptr);
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
	VertexNum = max(int(10 * (UINT)Radius * 0.5), 8);

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		vector<VERTEX_3D> Vertex;
		VERTEX_3D vertex;
		Vertex.reserve(VertexNum * 2 + 1);

		const float angle = XM_PI / (VertexNum - 1);

		// 上の半円
		for (int i = 0; i < VertexNum; i++)
		{
			vertex.Position = XMFLOAT3(cosf(angle * i) * Radius, sinf(angle * i) * Radius + Height - Radius, 0.0f);
			vertex.Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);

			Vertex.emplace_back(vertex);
		}

		// 下の半円
		for (int i = 0; i < VertexNum; i++)
		{
			vertex.Position = XMFLOAT3(cosf(angle * (i + VertexNum - 1)) * Radius, sinf(angle * (i + VertexNum - 1)) * Radius - Height + Radius, 0.0f);
			vertex.Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);

			Vertex.emplace_back(vertex);
		}

		// 最後の点
		vertex.Position = XMFLOAT3(cosf(0) * Radius, sinf(0) * Radius + Height - Radius, 0.0f);
		vertex.Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex.Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex.emplace_back(vertex);

		VertexNum *= 2;
		VertexNum += 1;

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
			sd.pSysMem = Vertex.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer.get())
	{
		IndexNum = VertexNum * 2;
		vector<WORD> Index;
		Index.reserve(IndexNum);

		for (int i = 0; i < VertexNum; i++)
		{
			Index.emplace_back(i);
			Index.emplace_back((i + 1) % VertexNum);
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

void BOUNDING_CAPSULE::Init_Line()
{
	// 頂点バッファの設定
	if (nullptr == pVertexBuffer_Line.get())
	{
		VERTEX_3D vertex[6];

		vertex[0].Position = XMFLOAT3(Radius, -Height * 0.5f, 0.f);
		vertex[1].Position = XMFLOAT3(Radius, Height * 0.5f, 0.f);

		vertex[2].Position = XMFLOAT3(Radius, Height * 0.5f, 0.f);
		vertex[3].Position = XMFLOAT3(-Radius, -Height * 0.5f, 0.f);

		vertex[4].Position = XMFLOAT3(-Radius, -Height * 0.5f, 0.f);
		vertex[5].Position = XMFLOAT3(-Radius, Height * 0.5f, 0.f);

		for (int i = 0; i < 6; i++)
		{
			vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * 6;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer_Line.reset(buffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer_Line.get())
	{
		WORD index_array[] = {
			0, 1,
			1, 2,
			3, 4,
			4, 5
		};


		// インデックスバッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(WORD) * 8;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index_array;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer_Line.reset(buffer);
		}
	}
}

void BOUNDING_CAPSULE::Init_Ring()
{

}

void BOUNDING_CAPSULE::Draw_Semicircle(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	// 入力アセンブラに頂点バッファを設定
	CRenderer::SetVertexBuffers(pVertexBuffer.get());

	// 入力アセンブラにインデックスバッファを設定
	CRenderer::SetIndexBuffer(pIndexBuffer.get());

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMMATRIX world = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
	world *= XMMatrixTranslation(position.x, position.y, position.z);

	if (nullptr != camera01)
	{
		CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
	}
	else
	{
		CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
	}

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

void BOUNDING_CAPSULE::Draw_Body(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	XMFLOAT3 r = rotation;
	r.z += 180.0f;

	Draw_Semicircle(position, rotation);

	/*XMFLOAT3 p = position;
	p.y -= Height;

	Draw_Semicircle(p, r);

	p = position;
	p.y -= Height * 0.5f;

	Draw_Line(p, rotation);*/
}

void BOUNDING_CAPSULE::Draw_Line(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	// 入力アセンブラに頂点バッファを設定
	CRenderer::SetVertexBuffers(pVertexBuffer_Line.get());

	// 入力アセンブラにインデックスバッファを設定
	CRenderer::SetIndexBuffer(pIndexBuffer_Line.get());

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMMATRIX world = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
	world *= XMMatrixTranslation(position.x, position.y, position.z);

	if (nullptr != camera01)
	{
		CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
	}
	else
	{
		CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
	}

	CRenderer::GetDeviceContext()->DrawIndexed(8, 0, 0);
}

void BOUNDING_CAPSULE::Draw_Ring(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
}