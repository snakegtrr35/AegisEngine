#include	"Field.h"
#include	"Renderer.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

static const UINT g_InstanceNum = 40 * 40 * 40;

FIELD::FIELD()
{
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);

	// テクスチャの設定
	Texture.reset(new TEXTURE(string("UVCheckerMap01-512.png")));
}

FIELD::FIELD(XMFLOAT3 position, XMFLOAT2 wh)
{
	HRESULT hr;

	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);

	WH = wh;

	Vertex[0].Position = XMFLOAT3(1.0f, 0.0f, -1.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(-1.0f, 0.0f, -1.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(1.0f, 0.0f, 1.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(-1.0f, 0.0f, 1.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファの設定
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &buffer);

		if (FAILED(hr))
		{
			return;
		}

		pVertexBuffer.reset(buffer);
	}

	////　インスタンシング用のバッファの生成
	//{
	//	// マトリクス
	//	XMMATRIX matrix;
	//	vector<XMMATRIX> instMatrix;
	//	instMatrix.reserve(g_InstanceNum);

	//	XMFLOAT3 position[g_InstanceNum], rotation, scale;

	//	for (int y = 0; y < g_InstanceNum; y++)
	//	{
	//		position[y] = XMFLOAT3(10.0f, (y * 5) - 10.0f, 0.f);
	//	}

	//	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//	for (int i = 0; i < g_InstanceNum; i++)
	//	{
	//		matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	//		matrix *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	//		matrix *= XMMatrixTranslation(position[i].x, position[i].y, position[i].z);

	//		instMatrix.emplace_back(XMMatrixTranspose(matrix));
	//	}

	//	{
	//		ID3D11Buffer* buffer = nullptr;

	//		D3D11_BUFFER_DESC bd;
	//		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	//		bd.ByteWidth = sizeof(XMMATRIX) * g_InstanceNum;
	//		bd.Usage = D3D11_USAGE_DEFAULT;
	//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//		bd.CPUAccessFlags = 0;
	//		bd.MiscFlags = 0;
	//		bd.StructureByteStride = 0;

	//		// サブリソースの設定
	//		D3D11_SUBRESOURCE_DATA srd;
	//		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

	//		srd.pSysMem = instMatrix.data();
	//		srd.SysMemPitch = 0;
	//		srd.SysMemSlicePitch = 0;

	//		// 頂点バッファの生成
	//		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &buffer);

	//		if (FAILED(hr))
	//		{
	//			return;
	//		}

	//		pInstanceBuffer.reset(buffer);
	//	}
	//}

	// インデックスバッファの設定
	{
		ID3D11Buffer* buffer = nullptr;

		const WORD index[] = {
		0, 1, 2,
		1, 3, 2,
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

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &buffer);
		if (FAILED(hr))
		{
			return;
		}

		pIndexBuffer.reset(buffer);
	}

	// テクスチャの設定
	Texture.reset(new TEXTURE(string("UVCheckerMap01-512.png")));
}

FIELD::~FIELD()
{
	Uninit();
}

void FIELD::Init()
{
	HRESULT hr;

	Vertex[0].Position = XMFLOAT3(1.0f, 0.0f, -1.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(-1.0f, 0.0f, -1.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(1.0f, 0.0f, 1.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(-1.0f, 0.0f, 1.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// 頂点バッファの設定
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &buffer);

		if (FAILED(hr))
		{
			return;
		}

		pVertexBuffer.reset(buffer);
	}

	//　インスタンシング用のバッファの生成
	{
		// マトリクス
		XMMATRIX matrix;
		vector<XMMATRIX> instMatrix;
		instMatrix.reserve(g_InstanceNum);

		XMFLOAT3 position[g_InstanceNum], rotation, scale;

		int i = 0;
		for (int y = 0; y < 40; y++)
		{
			for (int x = 0; x < 40; x++)
			{
				for (int z = 0; z < 40; z++, i++)
				{
					position[i] = XMFLOAT3(x * 3.0f + 10.0f, (y * 1) - 20.0f, (z * 3.0f) - 20.0f);
				}
			}
		}

		rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

		scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < g_InstanceNum; i++)
		{
			matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
			matrix *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, XMConvertToRadians(rotation.z));
			matrix *= XMMatrixTranslation(position[i].x, position[i].y, position[i].z);

			instMatrix.emplace_back(XMMatrixTranspose(matrix));
		}

		{
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

			bd.ByteWidth = sizeof(XMMATRIX) * g_InstanceNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			// サブリソースの設定
			D3D11_SUBRESOURCE_DATA srd;
			ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

			srd.pSysMem = instMatrix.data();
			srd.SysMemPitch = 0;
			srd.SysMemSlicePitch = 0;

			// 頂点バッファの生成
			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &buffer);

			if (FAILED(hr))
			{
				return;
			}

			pInstanceBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	{
		ID3D11Buffer* buffer = nullptr;

		const WORD index[] = {
		0, 1, 2,
		1, 3, 2,
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

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &buffer);
		if (FAILED(hr))
		{
			return;
		}

		pIndexBuffer.reset(buffer);
	}
}

void FIELD::Draw()
{
	// 入力アセンブラに頂点バッファを設定.
	//CRenderer::SetVertexBuffers(pVertexBuffer.get());

	CRenderer::Set_InputLayout(INPUTLAYOUT::INSTANCING);//
	CRenderer::Set_Shader(SHADER_INDEX_V::INSTANCING, SHADER_INDEX_P::NO_LIGHT);//
	CRenderer::SetVertexBuffers(pVertexBuffer.get(), pInstanceBuffer.get(), sizeof(VERTEX_3D));//

	CRenderer::SetIndexBuffer(pIndexBuffer.get());

	Texture->Set_Texture();//

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			// シャドウマップ用の描画か?
			if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

				CRenderer::Set_MatrixBuffer(world, view, proj);

				CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			}
			else
			{
				CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

				CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

				//CRenderer::Set_Shader();
			}
		}
		else
		{
			// シャドウマップ用の描画か?
			if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

				CRenderer::Set_MatrixBuffer(world, view, proj);

				CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			}
			else
			{
				CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

				CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

				//CRenderer::Set_Shader();
			}
		}
	}

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CRenderer::DrawIndexed(6, 0, 0);
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		CRenderer::GetDeviceContext()->DrawIndexedInstanced(6, g_InstanceNum, 0, 0, 0);//
	}

	CRenderer::Set_Shader();
	CRenderer::Set_InputLayout();//

	GAME_OBJECT::Draw();
}

void FIELD::Draw_DPP()
{
	//// 3Dマトリックス設定
	//{
	//	XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	//	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
	//	world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

	//	const auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	//	const auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	//	if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
	//	{
	//		CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
	//	}
	//	else
	//	{
	//		CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
	//	}
	//}

	//// 入力アセンブラに頂点バッファを設定.
	//CRenderer::SetVertexBuffers(pVertexBuffer.get());

	//CRenderer::SetIndexBuffer(pIndexBuffer.get());

	//CRenderer::DrawIndexed(6, 0, 0);
}

void FIELD::Update(float delta_time)
{
	GAME_OBJECT::Update(delta_time);
}


void FIELD::Uninit()
{
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);

	Texture.reset(nullptr);
}

//==============================
// ポジションの設定
//==============================
void FIELD::SetPosition(const XMFLOAT3 position)
{
	Position = position;
}

//==============================
// 幅と高さの設定
//==============================
void FIELD::SetWH(const XMFLOAT2 wh)
{
	WH = wh;
}

//==============================
// テクスチャの設定
//==============================
void FIELD::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}