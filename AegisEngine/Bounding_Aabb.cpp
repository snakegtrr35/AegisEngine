#include	"Bounding_Aabb.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

BOUNDING_AABB::BOUNDING_AABB()
{
}

BOUNDING_AABB::~BOUNDING_AABB()
{
	Uninit();
}

void BOUNDING_AABB::Init()
{
	// 頂点バッファの設定
	if (nullptr == pVertexBuffer_BOX.get())
	{
		const char VertexNum = 8;

		VERTEX_3D Vertex[VertexNum];

		Vertex[0].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);

		Vertex[1].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);

		Vertex[2].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);

		Vertex[3].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);


		Vertex[4].Position = XMFLOAT3(-0.5f, 0.5f, 0.5f);

		Vertex[5].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);

		Vertex[6].Position = XMFLOAT3(-0.5f, -0.5f, 0.5f);

		Vertex[7].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);

		for (char i = 0; i < VertexNum; i++)
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

			pVertexBuffer_BOX.reset(buffer);
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
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定.
		CRenderer::SetVertexBuffers(pVertexBuffer_BOX.get());

		CRenderer::SetIndexBuffer(pIndexBuffer_BOX.get());

		// 3Dマトリックス設定
		{
			XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
			world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

			CCamera* camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
			DEBUG_CAMERA* camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			if (nullptr != camera01)
			{
				CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

				CRenderer::Set_Shader();
			}
			else
			{
				CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

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

void BOUNDING_AABB::Update(float delta_time)
{
	OverWrite();
}


void BOUNDING_AABB::Uninit()
{
}

void BOUNDING_AABB::OverWrite()
{
	if (Color != Default_Color && nullptr != pVertexBuffer_BOX.get())
	{
		Color = Default_Color;

		const char VertexNum = 8;

		VERTEX_3D Vertex[VertexNum];

		Vertex[0].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
		Vertex[0].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);

		Vertex[1].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
		Vertex[1].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);

		Vertex[2].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
		Vertex[2].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);

		Vertex[3].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
		Vertex[3].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);


		Vertex[4].Position = XMFLOAT3(-0.5f, 0.5f, 0.5f);
		Vertex[4].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);

		Vertex[5].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);
		Vertex[5].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);

		Vertex[6].Position = XMFLOAT3(-0.5f, -0.5f, 0.5f);
		Vertex[6].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);

		Vertex[7].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);
		Vertex[7].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);

		for (char i = 0; i < VertexNum; i++)
		{
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(pVertexBuffer_BOX.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * VertexNum);
			CRenderer::GetDeviceContext()->Unmap(pVertexBuffer_BOX.get(), 0);
		}
	}
}