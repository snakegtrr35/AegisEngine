#include	"GameObject.h"
#include	"Field.h"
#include	"Renderer.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

IMPLEMENT_OBJECT_TYPE_INFO(GameObject, FIELD)

using namespace aegis;

static const UINT g_InstanceNum = 40 * 40 * 40;

FIELD::FIELD()
{
	// テクスチャの設定
	Texture.reset(new TEXTURE(aegis::string("UVCheckerMap01-512.png")));
}

FIELD::FIELD(Vector3 position, Vector2 wh)
{
	CRenderer* render = CRenderer::getInstance();
	HRESULT hr;

	WH = wh;

	VERTEX_3D Vertex[4];

	Vertex[0].Position = Vector3(1.0f, 0.0f, -1.0f);
	Vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	Vertex[1].Position = Vector3(-1.0f, 0.0f, -1.0f);
	Vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	Vertex[2].Position = Vector3(1.0f, 0.0f, 1.0f);
	Vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	Vertex[3].Position = Vector3(-1.0f, 0.0f, 1.0f);
	Vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// 頂点バッファの設定
	{
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd{};
		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	////　インスタンシング用のバッファの生成
	//{
	//	// マトリクス
	//	XMMATRIX matrix;
	//	vector<XMMATRIX> instMatrix;
	//	instMatrix.reserve(g_InstanceNum);

	//	Vector3 position[g_InstanceNum], rotation, scale;

	//	for (int y = 0; y < g_InstanceNum; y++)
	//	{
	//		position[y] = Vector3(10.0f, (y * 5) - 10.0f, 0.f);
	//	}

	//	rotation = Vector3(0.0f, 0.0f, 0.0f);

	//	scale = Vector3(1.0f, 1.0f, 1.0f);

	//	for (int i = 0; i < g_InstanceNum; i++)
	//	{
	//		matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	//		matrix *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	//		matrix *= XMMatrixTranslation(position[i].x, position[i].y, position[i].z);

	//		instMatrix.emplace_back(XMMatrixTranspose(matrix));
	//	}

	//	{
	//		D3D11_BUFFER_DESC bd{};
	//		bd.ByteWidth = sizeof(XMMATRIX) * g_InstanceNum;
	//		bd.Usage = D3D11_USAGE_DEFAULT;
	//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//		bd.CPUAccessFlags = 0;
	//		bd.MiscFlags = 0;
	//		bd.StructureByteStride = 0;

	//		// サブリソースの設定
	//		D3D11_SUBRESOURCE_DATA srd{};
	//		srd.pSysMem = instMatrix.data();
	//		srd.SysMemPitch = 0;
	//		srd.SysMemSlicePitch = 0;

	//		// 頂点バッファの生成
	//		hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pInstanceBuffer);

	//		if (FAILED(hr))
	//		{
	//			return;
	//		}
	//	}
	//}

	// インデックスバッファの設定
	{
		const WORD index[] = {
		0, 1, 2,
		1, 3, 2,
		};

		D3D11_BUFFER_DESC ibDesc{};
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
			return;
		}
	}

	// テクスチャの設定
	Texture.reset(new TEXTURE(aegis::string("UVCheckerMap01-512.png")));
}

FIELD::~FIELD()
{
	Uninit();
}

void FIELD::Init()
{
	CRenderer* render = CRenderer::getInstance();

	HRESULT hr;
	VERTEX_3D Vertex[4];

	Vertex[0].Position = Vector3(1.0f, 0.0f, -1.0f);
	Vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	Vertex[1].Position = Vector3(-1.0f, 0.0f, -1.0f);
	Vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	Vertex[2].Position = Vector3(1.0f, 0.0f, 1.0f);
	Vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	Vertex[3].Position = Vector3(-1.0f, 0.0f, 1.0f);
	Vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = Vector2(1.0f, 1.0f);


	// 頂点バッファの設定
	{
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd{};

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	//　インスタンシング用のバッファの生成
	{
		// マトリクス
		XMMATRIX matrix;
		vector<XMMATRIX> instMatrix;
		instMatrix.reserve(g_InstanceNum);

		Vector3 position[g_InstanceNum], rotation, scale;

		int i = 0;
		for (int y = 0; y < 40; y++)
		{
			for (int x = 0; x < 40; x++)
			{
				for (int z = 0; z < 40; z++, i++)
				{
					position[i] = Vector3(x * 3.0f + 10.0f, (y * 1) - 20.0f, (z * 3.0f) - 20.0f);
				}
			}
		}

		rotation = Vector3(0.0f, 0.0f, 0.0f);

		scale = Vector3(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < g_InstanceNum; i++)
		{
			matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
			matrix *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, XMConvertToRadians(rotation.z));
			matrix *= XMMatrixTranslation(position[i].x, position[i].y, position[i].z);

			instMatrix.emplace_back(XMMatrixTranspose(matrix));
		}

		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(XMMATRIX) * g_InstanceNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			// サブリソースの設定
			D3D11_SUBRESOURCE_DATA srd{};
			srd.pSysMem = instMatrix.data();
			srd.SysMemPitch = 0;
			srd.SysMemSlicePitch = 0;

			// 頂点バッファの生成
			hr = render->GetDevice()->CreateBuffer(&bd, &srd, &pInstanceBuffer);

			if (FAILED(hr))
			{
				return;
			}
		}
	}

	// インデックスバッファの設定
	{
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

		hr = render->GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}
}

void FIELD::Draw()
{
	CRenderer* render = CRenderer::getInstance();

	// 入力アセンブラに頂点バッファを設定.
	//render->SetVertexBuffers(pVertexBuffer.Get());

	render->Set_InputLayout(INPUTLAYOUT::INSTANCING);//
	render->Set_Shader(SHADER_INDEX_V::INSTANCING, SHADER_INDEX_P::NO_LIGHT);//
	render->SetVertexBuffers(pVertexBuffer.Get(), pInstanceBuffer.Get(), sizeof(VERTEX_3D));//

	render->SetIndexBuffer(pIndexBuffer.Get());

	Texture->Set_Texture();//

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

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			// シャドウマップ用の描画か?
			if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

				render->Set_MatrixBuffer(world, view, proj);

				render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			}
			else
			{
				render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

				render->Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

				//render->Set_Shader();
			}
		}
		else
		{
			// シャドウマップ用の描画か?
			if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

				render->Set_MatrixBuffer(world, view, proj);

				render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			}
			else
			{
				render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

				render->Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

				//render->Set_Shader();
			}
		}
	}

	//render->DrawIndexed(6, 0, 0);
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		render->GetDeviceContext()->DrawIndexedInstanced(6, g_InstanceNum, 0, 0, 0);//
	}

	render->Set_Shader();
	render->Set_InputLayout();//

	GameObject::Draw();
}

void FIELD::Draw_Shadow()
{
	CRenderer* render = CRenderer::getInstance();

	// 入力アセンブラに頂点バッファを設定.
	render->SetVertexBuffers(pVertexBuffer.Get());

	render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::MAX);

	render->SetIndexBuffer(pIndexBuffer.Get());

	Texture->Set_Texture();//

	// 3Dマトリックス設定
	{
		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		XMMATRIX world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		{
			// シャドウマップ用の描画か?
			if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

				render->Set_MatrixBuffer(world, view, proj);
			}
		}
	}

	render->DrawIndexed(6, 0, 0);

	GameObject::Draw_Shadow();
}

void FIELD::Draw_DPP()
{
	//CRenderer* render = CRenderer::getInstance();

	//// 3Dマトリックス設定
	//{
	//	XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	//	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
	//	world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

	//	const auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	//	const auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	//	if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
	//	{
	//		render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
	//	}
	//	else
	//	{
	//		render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
	//	}
	//}

	//// 入力アセンブラに頂点バッファを設定.
	//render->SetVertexBuffers(pVertexBuffer.Get());

	//render->SetIndexBuffer(pIndexBuffer.Get());

	//render->DrawIndexed(6, 0, 0);
}

void FIELD::Update(float delta_time)
{
	GameObject::Update(delta_time);
}


void FIELD::Uninit()
{
	GameObject::Uninit();

	Texture.reset(nullptr);
}

//==============================
// ポジションの設定
//==============================
void FIELD::SetPosition(Vector3& position)
{
	Get_Transform().Set_Position(position);
}

//==============================
// 幅と高さの設定
//==============================
void FIELD::SetWH(const Vector2 wh)
{
	WH = wh;
}

//==============================
// テクスチャの設定
//==============================
void FIELD::SetTexture(const aegis::string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}