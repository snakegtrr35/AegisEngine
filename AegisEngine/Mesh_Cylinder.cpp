#include	"GameObject.h"
#include	"Mesh_Cylinder.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

using namespace Aegis;

void MESH_CYlLINDER::Init()
{
	int cornerNum = (int)Radius * CylinderLength * 10;

	// 頂点バッファの確保
	VertexNum = (cornerNum + 1) * (CylinderLength + 1);
	VERTEX_3D* vertexArray = new VERTEX_3D[VertexNum];

	// インデックスバッファの確保
	IndexNum = (2 + (cornerNum * 2)) * CylinderLength + (CylinderLength - 1) * 2;
	WORD* indexArray = new WORD[IndexNum];

	// 頂点バッファへの頂点情報の書き込み
	float theta = 0.0f;
	float addAngle = 2.0f * XM_PI / ((float)CylinderLength * 60.0f);
	for (int y = 0; y < CylinderLength + 1; y++)
	{
		theta = 0.0f;

		for (int x = 0; x < cornerNum + 1; x++)
		{
			vertexArray[x + (cornerNum + 1) * y].Position	= Vector3(Radius * cosf(theta), (float)CylinderLength - (float)y, Radius * sinf(theta));
			vertexArray[x + (cornerNum + 1) * y].Normal		= Vector3(0.0f, 1.0f, 0.0f);
			vertexArray[x + (cornerNum + 1) * y].Diffuse	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexArray[x + (cornerNum + 1) * y].TexCoord	= Vector2(-theta / (2.0f * XM_PI / (float)CylinderLength), (float)y / CylinderLength);

			theta -= addAngle;
		}
	}

	// 頂点インデックスバッファへ頂点インデックスの書き込み
	int indexNum = 0;
	int indexFlapX = cornerNum - 1;
	int indexFlapY = CylinderLength - 1;
	for (int y = 0; y < CylinderLength; y++)
	{
		for (int x = 0; x < cornerNum + 1; x++)
		{
			indexArray[indexNum++] = WORD((cornerNum + 1) * (y + 1) + x);
			indexArray[indexNum++] = WORD((cornerNum + 1) * y + x);

			// 縮退ポリゴン用にインデックスの追加
			if (x > indexFlapX && y < indexFlapY)
			{
				indexArray[indexNum++] = WORD((cornerNum + 1) * y + (cornerNum + 1) - 1);
				indexArray[indexNum++] = WORD((cornerNum + 1) * (y + 1 + 1));
			}
		}
	}


	// 頂点バッファ生成
	if (nullptr == VertexBuffer.get())
	{
		ID3D11Buffer* pVB = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pVB);

		VertexBuffer.reset(pVB);
	}

	// インデックスバッファ生成
	if (nullptr == IndexBuffer.get())
	{
		ID3D11Buffer* pIB = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = indexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pIB);

		IndexBuffer.reset(pIB);
	}

	// メモリ解放
	SAFE_DELETE_ARRAY(vertexArray);
	SAFE_DELETE_ARRAY(indexArray);

	// テクスチャの設定
	Texture.reset(new TEXTURE(string("field004.png")));
}

void MESH_CYlLINDER::Uninit()
{
	VertexBuffer.reset();
	IndexBuffer.reset();
	Texture.reset();
}

void MESH_CYlLINDER::Draw()
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		{
			XMMATRIX world;

			Vector3 position = *Get_Transform().Get_Position();
			Vector3 rotate = *Get_Transform().Get_Rotation();
			Vector3 scale = *Get_Transform().Get_Scaling();

			world = XMMatrixScaling(scale.x, scale.y, scale.z);
			world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
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

		CRenderer::SetVertexBuffers(VertexBuffer.get());	// 頂点バッファ設定
		CRenderer::SetIndexBuffer(IndexBuffer.get());		// インデックスバッファ設定

		Texture.get()->Set_Texture();

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// トポロジ設定

		// ポリゴン描画
		CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
	}

	GAME_OBJECT::Draw();
}

void MESH_CYlLINDER::Draw_DPP()
{
	{
		XMMATRIX world;

		Vector3 position = *Get_Transform().Get_Position();
		Vector3 rotate = *Get_Transform().Get_Rotation();
		Vector3 scale = *Get_Transform().Get_Scaling();

		world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	CRenderer::SetVertexBuffers(VertexBuffer.get());	// 頂点バッファ設定
	CRenderer::SetIndexBuffer(IndexBuffer.get());		// インデックスバッファ設定

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// トポロジ設定

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

void MESH_CYlLINDER::Update(float delta_time)
{
	GAME_OBJECT::Update(delta_time);
}
