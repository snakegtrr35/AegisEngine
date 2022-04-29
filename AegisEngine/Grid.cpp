#include	"Grid.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

IMPLEMENT_OBJECT_TYPE_INFO(GameObject, GRID)

#define COUNT (11 * 2 * 2)

using namespace aegis;

GRID::GRID()
{
	CRenderer* render = CRenderer::getInstance();

	const VERTEX_3D Vertexs[COUNT] = {

		// 横
		{ Vector3(-5.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, -4.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, -4.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, -3.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, -3.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, -2.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, -2.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, 2.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, 2.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, 3.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, 3.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, 4.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, 4.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-5.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(5.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },


		// 縦
		{ Vector3(-5.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-5.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(-4.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-4.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(-3.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-3.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(-2.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-2.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(-1.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-1.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(0.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(1.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, 0.0f,-5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(2.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(2.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(3.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(3.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(4.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(4.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(5.0f, 0.0f, 5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(5.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
	};

	// 頂点バッファの設定
	{
		HRESULT hr;
		D3D11_BUFFER_DESC bd = {};

		bd.ByteWidth = sizeof(VERTEX_3D) * COUNT;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertexs;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			assert(false);
			return;
		}
	}
}

GRID::~GRID()
{
	Uninit();
}

void GRID::Init()
{
}

void GRID::Draw()
{
	CRenderer* render = CRenderer::getInstance();

	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定.
		render->SetVertexBuffers(pVertexBuffer.Get());

		// 3Dマトリックス設定
		{
			XMMATRIX world = XMMatrixIdentity();

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

		// トポロジの設定
		render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		render->Draw(COUNT, 0);

		// インスタンシング
		//render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//render->DrawInstanced(COUNT, 200000, 0, 0);

		render->Set_Shader();
	}
}

void GRID::Update(float delta_time)
{
}

void GRID::Uninit()
{
	GameObject::Uninit();

	SAFE_RELEASE(pVertexBuffer);
}