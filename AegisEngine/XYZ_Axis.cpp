#include "XYZ_Axis.h"
#include "camera.h"
#include "Debug_Camera.h"
#include "manager.h"
#include "Scene.h"
#include "ShadowMap.h"

IMPLEMENT_OBJECT_TYPE_INFO(GameObject, AXIS)

#define COUNT (6)

using namespace aegis;

AXIS::AXIS()
{
	CRenderer* render = render->getInstance();

	// X
	{
		const VERTEX_3D Vertexs[4] = {
			{ Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },


			//{ Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(0.8f, 0.0f, 0.2f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

			//{ Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(0.8f, 0.0f, -0.2f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		};

		// 頂点バッファの設定
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(VERTEX_3D) * COUNT;
			bd.BindFlags = BindFlag::Vertexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			// サブリソースの設定
			SubresourceData sd{};
			sd.pSysMem = Vertexs;

			// サブリソースの設定
			VertexBuffer[0].reset(render->CreateBuffer(bd, sd));
		}
	}

	// Y
	{
		const VERTEX_3D Vertexs[4] = {
			{ Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },


			//{ Vector3(0.0f, 2.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(0.2f, 0.8f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

			//{ Vector3(0.0f, 2.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(-0.2f, 0.8f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		};

		// 頂点バッファの設定
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(VERTEX_3D) * COUNT;
			bd.BindFlags = BindFlag::Vertexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			// サブリソースの設定
			SubresourceData sd{};
			sd.pSysMem = Vertexs;

			// サブリソースの設定
			VertexBuffer[1].reset(render->CreateBuffer(bd, sd));
		}
	}

	// Z
	{
		const VERTEX_3D Vertexs[4] = {
			{ Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },


			//{ Vector3(0.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(0.2f, 0.0f, 0.8f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },

			//{ Vector3(0.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
			{ Vector3(-0.2f, 0.0f, 0.8f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
		};

		// 頂点バッファの設定
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(VERTEX_3D) * COUNT;
			bd.BindFlags = BindFlag::Vertexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			// サブリソースの設定
			SubresourceData sd{};
			sd.pSysMem = Vertexs;

			// サブリソースの設定
			VertexBuffer[2].reset(render->CreateBuffer(bd, sd));
		}
	}

	// インデックスバッファの設定
	if (nullptr == IndexBuffer)
	{
		HRESULT hr;

		const WORD index[] = {
			0, 1,
			1, 2,
			1, 3,
		};

		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(uint16) * 6;
		bd.BindFlags = BindFlag::Indexbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};
		sd.pSysMem = index;

		IndexBuffer.reset(render->CreateBuffer(bd, sd));
	}
}

AXIS::~AXIS()
{
	Uninit();
}

void AXIS::Init(void)
{
}

void AXIS::Draw(void)
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		return;
	}

	CRenderer* render = render->getInstance();

	XMMATRIX world;

	// 入力アセンブラにインデックスバッファを設定
	render->SetIndexBuffer(IndexBuffer.get());

	// トポロジの設定
	render->SetPrimitiveTopology(PrimitiveTopology::LineStrip);

	// 
	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	for (int i = 0; i < 3; i++)
	{
		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(VertexBuffer[i].get());

		// 3Dマトリックス設定
		{
			world = XMMatrixIdentity();

			Vector3 position = Get_Transform().Get_Position();
			Vector3 rotate = Get_Transform().Get_Rotation();
			Vector3 scale = Get_Transform().Get_Scaling();

			world = XMMatrixScaling(scale.x, scale.y, scale.z);																						// 拡大縮小
			world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));			// 回転(ロールピッチヨウ)
			//world *= XMMatrixTranslation(pos.x, pos.y, pos.z);																								// 移動
			world *= XMMatrixTranslation(position.x, position.y + 0.5f, position.z);

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
		render->DrawIndexed(COUNT, 0, 0);
	}

	// トポロジの設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	render->Set_Shader();
}

void AXIS::Update(float delta_time)
{
}

void AXIS::Uninit(void)
{
	GameObject::Uninit();

	for (int i = 0; i < 3; i++)
	{
		VertexBuffer[i].reset();
	}
}