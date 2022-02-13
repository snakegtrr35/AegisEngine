#include	"XYZ_Axis.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

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
			HRESULT hr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

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
			hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[0]);

			if (FAILED(hr))
			{
				assert(false);
				return;
			}
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
			HRESULT hr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

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
			hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[1]);

			if (FAILED(hr))
			{
				assert(false);
				return;
			}
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
			HRESULT hr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

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
			hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[2]);

			if (FAILED(hr))
			{
				assert(false);
				return;
			}
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer)
	{
		HRESULT hr;

		const WORD index[] = {
			0, 1,
			1, 2,
			1, 3,
		};

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = index;
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		hr = render->GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT
		}
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
	render->SetIndexBuffer(pIndexBuffer.Get());

	// トポロジの設定
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 
	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	for (int i = 0; i < 3; i++)
	{
		// 入力アセンブラに頂点バッファを設定
		render->SetVertexBuffers(pVertexBuffer[i].Get());

		// 3Dマトリックス設定
		{
			world = XMMatrixIdentity();

			Vector3 position = *Get_Transform().Get_Position();
			Vector3 rotate = *Get_Transform().Get_Rotation();
			Vector3 scale = *Get_Transform().Get_Scaling();

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
	render->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	render->Set_Shader();
}

void AXIS::Update(float delta_time)
{
}

void AXIS::Uninit(void)
{
	for (int i = 0; i < 3; i++)
	{
		SAFE_RELEASE(pVertexBuffer[i]);
	}
}