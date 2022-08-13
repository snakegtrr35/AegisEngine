#include "Skybox.h"

#include "camera.h"
#include "Debug_Camera.h"
#include "manager.h"
#include "Scene.h"
#include "ShadowMap.h"

#include "TextureImporter.h"

using namespace aegis;

aegis::uniquePtr<aegis::Buffer> SKYBOX::VertexBuffer;
aegis::uniquePtr<aegis::Buffer> SKYBOX::IndexBuffer;

static constexpr UINT Indecies = 36;

SKYBOX::SKYBOX()
{
	//Texture = std::make_unique<TEXTURE>("");
}

SKYBOX::~SKYBOX(){ Uninit(); }

void SKYBOX::Init()
{
	GameObject::Init();

	CRenderer* render = render->getInstance();

	// 頂点バッファの作成
	if(nullptr == VertexBuffer)
	{
		vector<VERTEX_3D> vertex_array;
		vertex_array.resize(8);
		for (auto& vertex : vertex_array)
		{
			vertex.Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// 右上奥
		vertex_array[0].Position = Vector3(-1.0f, 1.0f, 1.0f);

		// 左上奥
		vertex_array[1].Position = Vector3(1.0f, 1.0f, 1.0f);

		// 右下奥
		vertex_array[2].Position = Vector3(-1.0f, -1.0f, 1.0f);

		// 左下奥
		vertex_array[3].Position = Vector3(1.0f, -1.0f, 1.0f);

		// 右上前
		vertex_array[4].Position = Vector3(-1.0f, 1.0f, -1.0f);

		// 左上前
		vertex_array[5].Position = Vector3(1.0f, 1.0f, -1.0f);

		// 右下前
		vertex_array[6].Position = Vector3(-1.0f, -1.0f, -1.0f);

		// 左下前
		vertex_array[7].Position = Vector3(1.0f, -1.0f, -1.0f);

		// 頂点バッファの生成
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(VERTEX_3D) * vertex_array.size();
			bd.BindFlags = BindFlag::Vertexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			SubresourceData sd{};
			sd.pSysMem = vertex_array.data();

			VertexBuffer.reset(render->CreateBuffer(bd, sd));
		}
	}

	// インデックスバッファの作成
	if (nullptr == IndexBuffer)
	{
		const vector<WORD> index_array = {	// Front Face
											0, 1, 2,
											1, 3, 2,
											// Back Face
											5, 4, 7,
											4, 6, 7,
											// Top Face
											4, 5, 0,
											5, 1, 0,
											// Bottom Face
											2, 3, 6,
											3, 7, 6,
											// Left Face
											4, 0, 6,
											0, 2, 6,
											// Right Face
											1, 5, 3,
											5, 7, 3 };

		// インデックスバッファの生成
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(uint16) * index_array.size();
			bd.BindFlags = BindFlag::Indexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			SubresourceData sd{};
			sd.pSysMem = index_array.data();

			IndexBuffer.reset(render->CreateBuffer(bd, sd));
		}
	}

	{
		auto textureImporter = TextureImporter::getInstance();
		Texture.reset(textureImporter->GetShaderResourceView(L"asset/texture/cloudySea.dds"));
	}

	GameObject::InitEnd();
}

void SKYBOX::Draw()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		return;
	}

	CRenderer* render = render->getInstance();

	// 3Dマトリックス設定
	{
		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		XMMATRIX world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired())
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

	// 頂点バッファ設定
	render->SetVertexBuffers(VertexBuffer.get());

	// インデックスバッファ設定
	render->SetIndexBuffer(IndexBuffer.get());

	// テクスチャの設定
	aegis::ShaderResourceView* shaderResourceViews[] = { Texture.get()};
	render->PSSetShaderResources(0, 1, shaderResourceViews);

	render->Set_Shader(SHADER_INDEX_V::SKYBOX, SHADER_INDEX_P::SKYBOX);

	render->DrawIndexed(Indecies, 0, 0);

	render->Set_Shader();
}

void SKYBOX::Draw_DPP()
{
}

void SKYBOX::Update(float delta_time)
{
}

void SKYBOX::Uninit()
{
	GameObject::Uninit();
}