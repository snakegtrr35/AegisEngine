#include	"XYZ_Axis.h"

#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

#define COUNT (6)

AXIS::AXIS()
{
	// X
	{
		const VERTEX_3D Vertexs[4] = {
			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },


			//{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.8f, 0.0f, 0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

			//{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.8f, 0.0f, -0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
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
			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[0]);

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
			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },


			//{ XMFLOAT3(0.0f, 2.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.2f, 0.8f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

			//{ XMFLOAT3(0.0f, 2.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-0.2f, 0.8f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
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
			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[1]);

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
			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },


			//{ XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.2f, 0.0f, 0.8f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

			//{ XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-0.2f, 0.0f, 0.8f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
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
			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[2]);

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

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
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
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		XMMATRIX world;

		// 入力アセンブラにインデックスバッファを設定
		CRenderer::SetIndexBuffer(pIndexBuffer);

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		// 
		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		for (int i = 0; i < 3; i++)
		{
			// 入力アセンブラに頂点バッファを設定
			CRenderer::SetVertexBuffers(pVertexBuffer[i]);

			// 3Dマトリックス設定
			{
				world = XMMatrixIdentity();

				world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// 拡大縮小
				world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転(ロールピッチヨウ)
				//world *= XMMatrixTranslation(pos.x, pos.y, pos.z);																								// 移動
				world *= XMMatrixTranslation(Position.x, Position.y + 0.5f, Position.z);

				auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>();
				auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>();

				if (nullptr != camera01)
				{
					CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());
				}
			}

			CRenderer::GetDeviceContext()->DrawIndexed(COUNT, 0, 0);
		}

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		CRenderer::Set_Shader();
	}
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