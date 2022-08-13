#include "Mesh_Dome.h"
#include "camera.h"
#include "Debug_Camera.h"
#include "manager.h"
#include "Scene.h"
#include "ShadowMap.h"
#include "texture.h"

using namespace aegis;

MESH_DOOM::MESH_DOOM() : Radius(500.0f)
{
	// テクスチャの生成
	Texture = std::make_unique<TEXTURE>("sky.png");
}

void MESH_DOOM::Init()
{
	GameObject::Init();

	CRenderer* render = CRenderer::getInstance();

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
				vertexArray[x + (cornerNum + 1) * y].Position = Vector3(Radius * sinf(theta) * cosf(phi), Radius * cosf(theta), Radius * sinf(theta) * sinf(phi));
				vertexArray[x + (cornerNum + 1) * y].Normal = Vector3(0.0f, 1.0f, 0.0f);
				vertexArray[x + (cornerNum + 1) * y].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
				vertexArray[x + (cornerNum + 1) * y].TexCoord = Vector2(-phi / XM_2PI, -theta / XM_PI);

				phi -= addAngle;
			}

			theta -= addAngle;
		}

		// 頂点バッファ生成
		if (nullptr == VertexBuffer)
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
			bd.BindFlags = BindFlag::Vertexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			SubresourceData sd{};
			sd.pSysMem = vertexArray.data();

			VertexBuffer.reset(render->CreateBuffer(bd, sd));
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
		if (nullptr == IndexBuffer)
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(uint16) * IndexNum;
			bd.BindFlags = BindFlag::Indexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			SubresourceData sd{};
			sd.pSysMem = indexArray.data();

			IndexBuffer.reset(render->CreateBuffer(bd, sd));
		}
	}

	GameObject::InitEnd();
}

//***********************************************************************************************
//  解放
//***********************************************************************************************
void MESH_DOOM::Uninit()
{
	GameObject::Uninit();

	Texture.reset(nullptr);
}

//***********************************************************************************************
//  更新
//***********************************************************************************************
void MESH_DOOM::Update(float delta_time)
{
	GameObject::Update(delta_time);
}

//***********************************************************************************************
//  描画
//***********************************************************************************************
void MESH_DOOM::Draw()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		return;
	}

	CRenderer* render = CRenderer::getInstance();

	{
		XMMATRIX world;

		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	render->SetVertexBuffers(VertexBuffer.get());
	render->SetIndexBuffer(IndexBuffer.get());

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

	Texture.get()->Set_Texture();

	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

	render->DrawIndexed(IndexNum, 0, 0);

	render->Set_Shader();

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	GameObject::Draw();
}

void MESH_DOOM::Draw_Shadow()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		return;
	}

	CRenderer* render = CRenderer::getInstance();

	{
		XMMATRIX world;

		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	render->SetVertexBuffers(VertexBuffer.get());
	render->SetIndexBuffer(IndexBuffer.get());

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

	render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::MAX);

	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	GameObject::Draw_Shadow();
}

void MESH_DOOM::Draw_DPP()
{
}