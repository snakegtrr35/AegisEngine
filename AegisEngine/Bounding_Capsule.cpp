#include	"Bounding_Capsule.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

void BOUNDING_CAPSULE::Init()
{
	Init_Body();
	Init_Line();
	Init_Ring();
}

void BOUNDING_CAPSULE::Draw()
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		XMFLOAT3 pos(0.f, 2.5f, 0.f);
		XMFLOAT3 rotation(0.f, 0.f, 0.f);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		Draw_Body(pos, rotation);

		//Draw_Body();

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
	float Radius = 3.0f;
	VertexNum = 10 * (UINT)Radius * 0.5;

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		VERTEX_3D* vertex = new VERTEX_3D[VertexNum];

		const float angle = XM_PI / (VertexNum - 1);

		for (int i = 0; i < VertexNum; i++)
		{
			vertex[i].Position = XMFLOAT3(cosf(angle * i) * Radius, sinf(angle * i) * Radius, 0.0f);
			vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(1.0f, 0.f, 0.f, 1.0f);
			vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
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
			sd.pSysMem = vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}

		SAFE_DELETE_ARRAY(vertex);
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer.get())
	{
		IndexNum = VertexNum * 2;
		WORD* index_array = new WORD[IndexNum];

		for (int i = 0; i < VertexNum; i++)
		{
			index_array[i * 2] = i;
			index_array[i * 2 + 1] = (i + 1) % VertexNum;
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
			sd.pSysMem = index_array;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer.reset(buffer);
		}

		SAFE_DELETE(index_array);
	}
}

void BOUNDING_CAPSULE::Init_Line()
{
	// 頂点バッファの設定
	if (nullptr == pVertexBuffer_Line.get())
	{
		VERTEX_3D vertex[6];

		vertex[0].Position = XMFLOAT3(3.f, -2.5, 0.f);
		vertex[1].Position = XMFLOAT3(3.f, 2.5, 0.f);

		vertex[2].Position = XMFLOAT3(3.f, 2.5, 0.f);
		vertex[3].Position = XMFLOAT3(-3.f, -2.5, 0.f);

		vertex[4].Position = XMFLOAT3(-3.f, -2.5, 0.f);
		vertex[5].Position = XMFLOAT3(-3.f, 2.5, 0.f);

		for (int i = 0; i < 6; i++)
		{
			vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(1.0f, 0.f, 0.f, 1.0f);
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

void BOUNDING_CAPSULE::Draw_Body(const XMFLOAT3& position, const XMFLOAT3& rotation)
{
	// 入力アセンブラに頂点バッファを設定
	CRenderer::SetVertexBuffers(pVertexBuffer.get());

	// 入力アセンブラにインデックスバッファを設定
	CRenderer::SetIndexBuffer(pIndexBuffer.get());

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

	CRenderer::GetDeviceContext()->Draw(VertexNum, 0);

	Draw_Line(rotation);
}

void BOUNDING_CAPSULE::Draw_Line(const XMFLOAT3& rotation)
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
	//world *= XMMatrixTranslation(position.x, position.y, position.z);

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

void BOUNDING_CAPSULE::Draw_Ring(const XMFLOAT3& rotation)
{

}