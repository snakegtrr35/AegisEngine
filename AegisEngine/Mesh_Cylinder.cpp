#include	"GameObject.h"
#include	"Mesh_Cylinder.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

using namespace aegis;

void MESH_CYlLINDER::Init()
{
	CRenderer* render = CRenderer::getInstance();

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
	if (nullptr == VertexBuffer)
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = vertexArray;

		render->GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);
	}

	// インデックスバッファ生成
	if (nullptr == IndexBuffer)
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = indexArray;

		render->GetDevice()->CreateBuffer(&bd, &sd, &IndexBuffer);\
	}

	// メモリ解放
	SAFE_DELETE_ARRAY(vertexArray);
	SAFE_DELETE_ARRAY(indexArray);

	// テクスチャの設定
	Texture.reset(new TEXTURE("field004.png"));
}

void MESH_CYlLINDER::Uninit()
{
	GameObject::Uninit();

	Texture.reset();
}

void MESH_CYlLINDER::Draw()
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

	render->SetVertexBuffers(VertexBuffer.Get());	// 頂点バッファ設定
	render->SetIndexBuffer(IndexBuffer.Get());		// インデックスバッファ設定

	Texture.get()->Set_Texture();

	// トポロジ設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GameObject::Draw();
}

void MESH_CYlLINDER::Draw_Shadow()
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

	render->SetVertexBuffers(VertexBuffer.Get());	// 頂点バッファ設定
	render->SetIndexBuffer(IndexBuffer.Get());		// インデックスバッファ設定

	// トポロジ設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::MAX);

	// ポリゴン描画
	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GameObject::Draw_Shadow();
}

void MESH_CYlLINDER::Draw_DPP()
{
	CRenderer* render = CRenderer::getInstance();
	{
		XMMATRIX world;

		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	render->SetVertexBuffers(VertexBuffer.Get());	// 頂点バッファ設定
	render->SetIndexBuffer(IndexBuffer.Get());		// インデックスバッファ設定

	// トポロジ設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MESH_CYlLINDER::Update(float delta_time)
{
	GameObject::Update(delta_time);
}
