#include	"Game_Object.h"
#include	"Mesh_Dome.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"renderer.h"
#include	"texture.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

MESH_DOOM::MESH_DOOM() : Radius(500.0f)
{
	VertexBuffer.reset(nullptr);
	IndexBuffer.reset(nullptr);

	// テクスチャの生成
	Texture = make_unique<TEXTURE>("sky.png");
}

void MESH_DOOM::Init()
{
	const int cornerNum = 20;

	// 頂点バッファの作成
	{
		// 頂点バッファの確保
		VertexNum = (cornerNum + 1) * (cornerNum / 2 + 1);

		vector<VERTEX_3D> vertexArray;
		vertexArray.reserve(VertexNum);
		vertexArray.assign(VertexNum, VERTEX_3D());

		// 頂点バッファへの頂点情報の書き込み
		float theta = 0.0f, phi = 0.0f;
		float addAngle = XM_2PI / (float)cornerNum;
		for (int y = 0; y < cornerNum / 2 + 1; y++)
		{
			phi = 0.0f;
			for (int x = 0; x < cornerNum + 1; x++)
			{
				vertexArray[x + (cornerNum + 1) * y].Position = XMFLOAT3(Radius * sinf(theta) * cosf(phi), Radius * cosf(theta), Radius * sinf(theta) * sinf(phi));
				vertexArray[x + (cornerNum + 1) * y].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertexArray[x + (cornerNum + 1) * y].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertexArray[x + (cornerNum + 1) * y].TexCoord = XMFLOAT2(-phi / XM_2PI, -theta / XM_PI);

				phi -= addAngle;
			}

			theta -= addAngle;
		}

		// 頂点バッファ生成
		if (nullptr == VertexBuffer.get())
		{
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertexArray.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			VertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの作成
	{
		// インデックスバッファの確保
		IndexNum = (2 + (cornerNum * 2)) * cornerNum / 2 + (cornerNum / 2 - 1) * 2;

		vector<WORD> indexArray;
		indexArray.reserve(IndexNum);
		indexArray.assign(IndexNum, 0);

		// 頂点インデックスバッファへ頂点インデックスの書き込み
		int indexNum = 0;
		int indexFlapX = cornerNum - 1;
		int indexFlapY = cornerNum / 2 - 1;
		for (int y = 0; y < cornerNum / 2; y++)
		{
			for (int x = 0; x < cornerNum + 1; x++)
			{
				indexArray[indexNum++] = WORD((cornerNum + 1) * (y + 1) + x);
				indexArray[indexNum++] = WORD((cornerNum + 1) * y + x);

				// 縮退ポリゴン用
				if (x > indexFlapX && y < indexFlapY)
				{
					indexArray[indexNum++] = WORD((cornerNum + 1) * y + (cornerNum + 1) - 1);
					indexArray[indexNum++] = WORD((cornerNum + 1) * (y + 1 + 1));
				}
			}
		}

		// インデックスバッファ生成
		if (nullptr == IndexBuffer.get())
		{
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = indexArray.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			IndexBuffer.reset(buffer);
		}
	}
}

//***********************************************************************************************
//  解放
//***********************************************************************************************
void MESH_DOOM::Uninit()
{
	VertexBuffer.reset(nullptr);
	IndexBuffer.reset(nullptr);
	Texture.reset(nullptr);
}

//***********************************************************************************************
//  更新
//***********************************************************************************************
void MESH_DOOM::Update(float delta_time)
{
	GAME_OBJECT::Update(delta_time);
}

//***********************************************************************************************
//  描画
//***********************************************************************************************
void MESH_DOOM::Draw()
{
	if(CManager::Get_Instance()->Get_ShadowMap()->Get_Enable()) return;

	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// 拡大縮小
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// 移動

		auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	CRenderer::SetVertexBuffers(VertexBuffer.get());
	CRenderer::SetIndexBuffer(IndexBuffer.get());

	// トポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Texture.get()->Set_Texture();

	CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);

	CRenderer::Set_Shader();

	GAME_OBJECT::Draw();
}

void MESH_DOOM::Draw_DPP()
{
}