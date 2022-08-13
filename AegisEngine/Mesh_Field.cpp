#include "Mesh_Field.h"
#include "camera.h"
#include "Debug_Camera.h"
#include "manager.h"
#include "Scene.h"
#include "ShadowMap.h"
#include "texture.h"

using namespace aegis;

MESH_FIELD::MESH_FIELD() : GridSize(Vector3(1.0f, 0.0f, 1.0f)), GridNum(Int2(10, 10))
{
	// テクスチャの生成
	Texture = std::make_unique<TEXTURE>("UVCheckerMap01-512.png");
};

void MESH_FIELD::Init()
{
	GameObject::Init();

	CRenderer* render = CRenderer::getInstance();

	// 頂点バッファの作成
	{
		// 頂点数
		const WORD VertexNum = WORD((GridNum.x + 1) * (GridNum.y + 1));

		VertexArray.resize(VertexNum);

		{
			Vector3 position;
			float legth = 0.0f;
			Vector3 va, vb;
			Vector3 vn;

			// 頂点バッファへの書き込み
			for (int z = 0; z < GridNum.y + 1; z++)
			{
				for (int x = 0; x < GridNum.x + 1; x++)
				{
					position.x = -0.5f * (float)GridNum.x * GridSize.x + (float)x * GridSize.x;
					//position.y = sinf(z * 0.8) * 0.3f;
					position.y = 0.0f;
					position.z = 0.5f * (float)GridNum.y * GridSize.z - (float)z * GridSize.z;

					VertexArray[x + (GridNum.x + 1) * z].Position = position;
					VertexArray[x + (GridNum.y + 1) * z].Normal = Vector3(0.0f, 1.0f, 0.0f);
					VertexArray[x + (GridNum.x + 1) * z].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
					VertexArray[x + (GridNum.x + 1) * z].TexCoord = Vector2((float)x, (float)z);
				}
			}

			// 頂点バッファへの書き込み(Normal)
			for (int z = 1; z < GridNum.y; z++)
			{
				for (int x = 1; x < GridNum.x; x++)
				{
					va.x = VertexArray[(z - 1) * GridNum.y + x].Position.x - VertexArray[(z + 1) * GridNum.x + x].Position.x;
					va.y = VertexArray[(z - 1) * GridNum.y + x].Position.y - VertexArray[(z + 1) * GridNum.x + x].Position.y;
					va.z = VertexArray[(z - 1) * GridNum.y + x].Position.z - VertexArray[(z + 1) * GridNum.x + x].Position.z;

					vb.x = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.x - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.x;
					vb.y = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.y - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.y;
					vb.z = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.z - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.z;

					// 外積
					vn.x = va.y * vb.z - va.z * vb.y;
					vn.y = va.z * vb.x - va.x * vb.z;
					vn.z = va.x * vb.y - va.y * vb.x;

					legth = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);

					vn.x = vn.x / legth;
					vn.y = vn.y / legth;
					vn.z = vn.z / legth;

					VertexArray[x + (GridNum.y + 1) * z].Normal = vn;
				}
			}
		}

		// 頂点バッファの生成
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(VERTEX_3D) * VertexArray.size();
			bd.BindFlags = BindFlag::Vertexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			SubresourceData sd{};
			sd.pSysMem = VertexArray.data();

			VertexBuffer.reset(render->CreateBuffer(bd, sd));
		}
	}

	// インデックスバッファの作成
	{
		IndexNum = (2 + (GridNum.x * 2)) * GridNum.y + (GridNum.y - 1) * 2;

		const UINT size = IndexNum;

		aegis::vector<WORD> indexArray;
		indexArray.resize(IndexNum);

		// 頂点インデックスバッファへの書き込み
		int indexNum = 0;
		int indexFlapX = GridNum.x - 1;
		int indexFlapZ = GridNum.y - 1;
		for (int z = 0; z < GridNum.y; z++)
		{
			for (int x = 0; x < GridNum.x + 1; x++)
			{
				indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1) + x);
				indexArray[indexNum++] = WORD((GridNum.x + 1) * z + x);

				// 縮退ポリゴン用にインデックスの追加
				if (x > indexFlapX && z < indexFlapZ)
				{
					indexArray[indexNum++] = WORD((GridNum.x + 1) * z + (GridNum.x + 1) - 1);
					indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1 + 1));
				}
			}
		}

		// インデックスバッファの生成
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

void MESH_FIELD::Uninit()
{
	GameObject::Uninit();

	Texture.reset(nullptr);
}

void MESH_FIELD::Update(float delta_time)
{
	GameObject::Update(delta_time);
}

void MESH_FIELD::Draw()
{
	CRenderer* render = CRenderer::getInstance();

	// 3Dマトリックス設定
	{
		XMMATRIX world;

		float angle = 0.0f;
		if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
		{
			//angle = 180.0f;
		}

		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() /*&& Empty_weak_ptr<CCamera>(camera01)*/)
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

				render->Set_MatrixBuffer01(*camera01.lock()->Get_Pos());

				render->Set_Shader();
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

				render->Set_Shader();
			}
		}
	}

	// 頂点バッファ設定
	render->SetVertexBuffers(VertexBuffer.get());

	// インデックスバッファ設定
	render->SetIndexBuffer(IndexBuffer.get());

	// テクスチャの設定
	Texture->Set_Texture();

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	GameObject::Draw();
}

void MESH_FIELD::Draw_Shadow()
{
	CRenderer* render = CRenderer::getInstance();

	// 3Dマトリックス設定
	{
		XMMATRIX world;

		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		world = XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		// シャドウマップ用の描画か?
		if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
		{
			XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
			XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

			render->Set_MatrixBuffer(world, view, proj);

			render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::MAX);
		}
	}

	// 頂点バッファ設定
	render->SetVertexBuffers(VertexBuffer.get());

	// インデックスバッファ設定
	render->SetIndexBuffer(IndexBuffer.get());

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	GameObject::Draw_Shadow();
}

void MESH_FIELD::Draw_DPP()
{
	CRenderer* render = CRenderer::getInstance();

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
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	// 頂点バッファ設定
	render->SetVertexBuffers(VertexBuffer.get());

	// インデックスバッファ設定
	render->SetIndexBuffer(IndexBuffer.get());

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
}

const float MESH_FIELD::Get_Height(const Vector3& position)
{
	float x, z;

	Vector3 p0, p1, p2, v01, v02, n, v, hp, va = Vector3(0.f, 0.f, 0.f), vb;

	float dp0n, dvn, dpn, t;

	v = Vector3(0.0f, -10.0f, 0.0f);

	x = (position.x +  (GridSize.x * GridNum.x) * 0.5f) / GridSize.x;
	z = (-position.z + (GridSize.z * GridNum.y) * 0.5f) / GridSize.z;
	va.x = VertexArray[static_cast<int32>((z + 1) * (GridNum.x + 1) + (x + 1))].Position.x - VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position.x;
	va.y = VertexArray[static_cast<int32>((z + 1) * (GridNum.x + 1) + (x + 1))].Position.y - VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position.y;
	va.z = VertexArray[static_cast<int32>((z + 1) * (GridNum.x + 1) + (x + 1))].Position.z - VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position.z;

	vb.x = position.x - VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position.x;
	vb.y = position.y - VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position.y;
	vb.z = position.z - VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position.z;

	if ((va.z * vb.x - va.x * vb.z) > 0.0f)
	{
		p0 = VertexArray[static_cast<int32>((z + 1) * (GridNum.x + 1) + x)].Position;
		p1 = VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position;
		p2 = VertexArray[static_cast<int32>((z + 1) * (GridNum.y + 1) + (x + 1))].Position;
	}
	else
	{
		p0 = VertexArray[static_cast<int32>(z * (GridNum.x + 1) + (x + 1))].Position;
		p1 = VertexArray[static_cast<int32>((z + 1) * (GridNum.x + 1) + (x + 1))].Position;
		p2 = VertexArray[static_cast<int32>(z * (GridNum.x + 1) + x)].Position;
	}

	v01.x = p1.x - p2.x;
	v01.y = p1.y - p2.y;
	v01.z = p1.z - p2.z;

	v02.x = p2.x - p0.x;
	v02.y = p2.y - p0.y;
	v02.z = p2.z - p0.z;

	// 外積
	n.x = v01.y * v02.z - v01.z * v02.y;
	n.y = v01.z * v02.x - v01.x * v02.z;
	n.z = v01.x * v02.y - v01.y * v02.x;

	// 内積
	dvn = v.x * n.x + v.y * n.y + v.z * n.z;

	// 内積
	dp0n = p0.x * n.x + p0.y * n.y + p0.z * n.z;

	// 内積
	dpn = position.x * n.x + position.y * n.y + position.z * n.z;

	t = (dp0n - dpn) / dvn;

	hp.x = position.x + v.x * t;
	hp.y = position.y + v.y * t;
	hp.z = position.z + v.z * t;

	return hp.y;
}

//==============================
// テクスチャの設定
//==============================
void MESH_FIELD::SetTexture(const aegis::string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}





MESH_WALL::MESH_WALL()
{
	VertexArray = nullptr;
	Texture = nullptr;

	GridSize = Vector3(1.0f, 0.0f, 1.0f);
	GridNum = Int2(10, 5);
};

void MESH_WALL::Init()
{
	GameObject::Init();

	CRenderer* render = CRenderer::getInstance();

	const unsigned int VertexNum = (GridNum.x + 1) * (GridNum.y + 1);
	VertexArray = new VERTEX_3D[VertexNum];

	IndexNum = (2 + (GridNum.x * 2)) * GridNum.y + (GridNum.y - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// 頂点バッファへの書き込み
	for (int z = 0; z < GridNum.y + 1; z++)
	{
		for (int x = 0; x < GridNum.x + 1; x++)
		{
			Vector3 position;

			position.x = -0.5f * (float)GridNum.x * GridSize.x + (float)x * GridSize.x;
			position.y = 0.0f;
			position.z = 0.5f * (float)GridNum.y * GridSize.z - (float)z * GridSize.z;

			VertexArray[x + (GridNum.x + 1) * z].Position = position;

			VertexArray[x + (GridNum.y + 1) * z].Normal = Vector3(0.0f, 1.0f, 0.0f);

			VertexArray[x + (GridNum.x + 1) * z].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			VertexArray[x + (GridNum.x + 1) * z].TexCoord = Vector2((float)x, (float)z);
		}
	}

	// 頂点バッファへの書き込み(Normal)
	for (int z = 1; z < GridNum.y; z++)
	{
		for (int x = 1; x < GridNum.x; x++)
		{
			Vector3 va, vb;

			va.x = VertexArray[(z - 1) * GridNum.y + x].Position.x - VertexArray[(z + 1) * GridNum.x + x].Position.x;
			va.y = VertexArray[(z - 1) * GridNum.y + x].Position.y - VertexArray[(z + 1) * GridNum.x + x].Position.y;
			va.z = VertexArray[(z - 1) * GridNum.y + x].Position.z - VertexArray[(z + 1) * GridNum.x + x].Position.z;

			vb.x = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.x - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.x;
			vb.y = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.y - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.y;
			vb.z = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.z - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.z;

			Vector3 vn;

			// 外積
			vn.x = va.y * vb.z - va.z * vb.y;
			vn.y = va.z * vb.x - va.x * vb.z;
			vn.z = va.x * vb.y - va.y * vb.x;

			float legth = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);

			vn.x = vn.x / legth;
			vn.y = vn.y / legth;
			vn.z = vn.z / legth;

			VertexArray[x + (GridNum.y + 1) * z].Normal = vn;
		}
	}

	// 頂点インデックスバッファへの書き込み
	int indexNum = 0;
	int indexFlapX = GridNum.x - 1;
	int indexFlapZ = GridNum.y - 1;
	for (int z = 0; z < GridNum.y; z++)
	{
		for (int x = 0; x < GridNum.x + 1; x++)
		{
			indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1) + x);
			indexArray[indexNum++] = WORD((GridNum.x + 1) * z + x);

			// 縮退ポリゴン用にインデックスの追加
			if (x > indexFlapX && z < indexFlapZ)
			{
				indexArray[indexNum++] = WORD((GridNum.x + 1) * z + (GridNum.x + 1) - 1);
				indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1 + 1));
			}
		}
	}

	// 頂点バッファの生成
	{
		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = BindFlag::Vertexbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};
		sd.pSysMem = VertexArray;

		VertexBuffer. reset(render->CreateBuffer(bd, sd));
	}

	// インデックスバッファの生成
	{
		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(uint16) * IndexNum;
		bd.BindFlags = BindFlag::Indexbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};
		sd.pSysMem = indexArray;

		IndexBuffer.reset(render->CreateBuffer(bd, sd));
	}

	delete[] indexArray;

	// テクスチャの設定
	Texture = new TEXTURE(aegis::string("field004.png"));

	GameObject::InitEnd();
}

void MESH_WALL::Uninit()
{
	SAFE_DELETE(VertexArray);
	SAFE_DELETE(Texture);
}

void MESH_WALL::Update(float delta_time)
{
	GameObject::Update(delta_time);
}

void MESH_WALL::Draw()
{
	CRenderer* render = CRenderer::getInstance();

	// 3Dマトリックス設定
	{
		XMMATRIX world(XMMatrixIdentity());

		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		matrix *= XMMatrixTranslation(position.x, position.y, position.z);

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

				render->Set_Shader();
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

				render->Set_Shader();
			}
		}
	}

	// 頂点バッファ設定
	render->SetVertexBuffers(VertexBuffer.get());

	// インデックスバッファ設定
	render->SetIndexBuffer(IndexBuffer.get());

	// テクスチャの設定
	Texture->Set_Texture();

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

	render->DrawIndexed(IndexNum, 0, 0);

	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		render->Set_Shader();
	}

	GameObject::Draw();
}

void MESH_WALL::Draw_DPP()
{
	CRenderer* render = CRenderer::getInstance();
	// 3Dマトリックス設定
	{
		Vector3 position = Get_Transform().Get_Position();
		Vector3 rotate = Get_Transform().Get_Rotation();
		Vector3 scale = Get_Transform().Get_Scaling();

		XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		matrix *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(matrix, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			render->Set_MatrixBuffer(matrix, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	// 頂点バッファ設定
	render->SetVertexBuffers(VertexBuffer.get());

	// インデックスバッファ設定
	render->SetIndexBuffer(IndexBuffer.get());

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

	render->DrawIndexed(IndexNum, 0, 0);

	// トポロジ設定
	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
}

const float MESH_WALL::Get_Height(const Vector3& position)
{
	int x, z;

	Vector3 p0, p1, p2, v01, v02, n, v, hp, va = Vector3(0.f, 0.f, 0.f), vb;

	float dp0n, dvn, dpn, t;

	v = Vector3(0.0f, -10.0f, 0.0f);

	x = (position.x + (GridSize.x * GridNum.x) * 0.5f) / GridSize.x;
	z = (-position.z + (GridSize.z * GridNum.y) * 0.5f) / GridSize.z;

	va.x = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.x - VertexArray[z * (GridNum.x + 1) + x].Position.x;
	va.y = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.y - VertexArray[z * (GridNum.x + 1) + x].Position.y;
	va.z = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.z - VertexArray[z * (GridNum.x + 1) + x].Position.z;

	vb.x = position.x - VertexArray[z * (GridNum.x + 1) + x].Position.x;
	vb.y = position.y - VertexArray[z * (GridNum.x + 1) + x].Position.y;
	vb.z = position.z - VertexArray[z * (GridNum.x + 1) + x].Position.z;

	if ((va.z * vb.x - va.x * vb.z) > 0.0f)
	{
		p0 = VertexArray[(z + 1) * (GridNum.x + 1) + x].Position;
		p1 = VertexArray[z * (GridNum.x + 1) + x].Position;
		p2 = VertexArray[(z + 1) * (GridNum.y + 1) + (x + 1)].Position;
	}
	else
	{
		p0 = VertexArray[z * (GridNum.x + 1) + (x + 1)].Position;
		p1 = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position;
		p2 = VertexArray[z * (GridNum.x + 1) + x].Position;
	}

	v01.x = p1.x - p2.x;
	v01.y = p1.y - p2.y;
	v01.z = p1.z - p2.z;

	v02.x = p2.x - p0.x;
	v02.y = p2.y - p0.y;
	v02.z = p2.z - p0.z;

	// 外積
	n.x = v01.y * v02.z - v01.z * v02.y;
	n.y = v01.z * v02.x - v01.x * v02.z;
	n.z = v01.x * v02.y - v01.y * v02.x;

	// 内積
	dvn = v.x * n.x + v.y * n.y + v.z * n.z;

	// 内積
	dp0n = p0.x * n.x + p0.y * n.y + p0.z * n.z;

	// 内積
	dpn = position.x * n.x + position.y * n.y + position.z * n.z;

	t = (dp0n - dpn) / dvn;

	hp.x = position.x + v.x * t;
	hp.y = position.y + v.y * t;
	hp.z = position.z + v.z * t;

	return hp.y;
}