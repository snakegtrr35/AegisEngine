#include	"Game_Object.h"
#include	"Skybox.h"

#include	"camera.h"
#include	"Debug_Camera.h"
//#include	"texture.h"
#include	"renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

#include	"Library/DirectXTex/DDSTextureLoader.h"

unique_ptr<ID3D11Buffer, Release> SKYBOX::VertexBuffer;
unique_ptr<ID3D11Buffer, Release> SKYBOX::IndexBuffer;

static constexpr UINT Indecies = 36;

static ID3D11ShaderResourceView* Srv = nullptr;

SKYBOX::SKYBOX()
{
	//Texture = make_unique<TEXTURE>("");

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

SKYBOX::~SKYBOX(){ Uninit(); }

void SKYBOX::Init()
{
	// 頂点バッファの作成
	if(nullptr == VertexBuffer.get())
	{
		vector<VERTEX_3D> vertex_array;
		vertex_array.resize(8);
		for (auto& vertex : vertex_array)
		{
			vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// 右上奥
		vertex_array[0].Position = XMFLOAT3(-1.0f, 1.0f, 1.0f);

		// 左上奥
		vertex_array[1].Position = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 右下奥
		vertex_array[2].Position = XMFLOAT3(-1.0f, -1.0f, 1.0f);

		// 左下奥
		vertex_array[3].Position = XMFLOAT3(1.0f, -1.0f, 1.0f);

		// 右上前
		vertex_array[4].Position = XMFLOAT3(-1.0f, 1.0f, -1.0f);

		// 左上前
		vertex_array[5].Position = XMFLOAT3(1.0f, 1.0f, -1.0f);

		// 右下前
		vertex_array[6].Position = XMFLOAT3(-1.0f, -1.0f, -1.0f);

		// 左下前
		vertex_array[7].Position = XMFLOAT3(1.0f, -1.0f, -1.0f);

		// 頂点バッファの生成
		{
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * vertex_array.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex_array.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			VertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの作成
	if (nullptr == IndexBuffer.get())
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
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(WORD) * index_array.size();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index_array.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			IndexBuffer.reset(buffer);
		}
	}

	{
		auto device = CRenderer::GetDevice();
		auto device_context = CRenderer::GetDeviceContext();
		
		CreateDDSTextureFromFile(device, device_context, L"asset/texture/cloudySea.dds", nullptr, &Srv, nullptr, nullptr);
	}
}

void SKYBOX::Draw()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable()) return;

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired())
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

	// 頂点バッファ設定
	CRenderer::SetVertexBuffers(VertexBuffer.get());

	// インデックスバッファ設定
	CRenderer::SetIndexBuffer(IndexBuffer.get());

	// テクスチャの設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Srv);

	// トポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CRenderer::Set_Shader(SHADER_INDEX_V::SKYBOX, SHADER_INDEX_P::SKYBOX);

	CRenderer::GetDeviceContext()->DrawIndexed(Indecies, 0, 0);

	CRenderer::Set_Shader();
}

void SKYBOX::Draw_DPP()
{
}

void SKYBOX::Update(float delta_time)
{
}

void SKYBOX::Uninit()
{
	VertexBuffer.reset(nullptr);
	IndexBuffer.reset(nullptr);
}