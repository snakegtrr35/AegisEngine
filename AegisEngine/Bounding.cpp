#include	"Bounding.h"

#include	"manager.h"
#include	"Scene.h"


BOUNDING_SHPERE::BOUNDING_SHPERE()
{
	Radius = 0.f;
	IndexNum = 0;
}

BOUNDING_SHPERE::~BOUNDING_SHPERE()
{
	Uninit();
}

void BOUNDING_SHPERE::Init()
{
	Create_Buffer();
}

void BOUNDING_SHPERE::Draw()
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		// 入力アセンブラにインデックスバッファを設定
		CRenderer::SetIndexBuffer(pIndexBuffer.get());

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		Draw_Ring(XMFLOAT3(0.f, 0.f, 0.f));

		Draw_Ring(XMFLOAT3(0.f, 90.0f, 0.f));

		Draw_Ring(XMFLOAT3(90.0f, 0.f, 0.f));

		CRenderer::Set_Shader();
	}
}

void BOUNDING_SHPERE::Update(float delta_time)
{
}

void BOUNDING_SHPERE::Uninit()
{
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);
}

void BOUNDING_SHPERE::Create_Buffer()
{
	if(nullptr != pVertexBuffer.get())
		pVertexBuffer.reset(nullptr);

	if (nullptr != pIndexBuffer.get())
		pIndexBuffer.reset(nullptr);

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		const UINT cnt = 10 * (UINT)Radius;

		VERTEX_3D* vertex = new VERTEX_3D[cnt];

		float angle = XM_2PI / cnt;

		for (int i = 0; i < cnt; i++)
		{
			vertex[i].Position = XMFLOAT3(cosf(angle * i) * Radius, sinf(angle * i) * Radius, 0.0f);
			vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * cnt;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}

		SAFE_DELETE(vertex);
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer.get())
	{
		const UINT cnt = 10 * (UINT)Radius;

		IndexNum = cnt * 2;
		WORD* index_array = new WORD[IndexNum];

		for (int i = 0; i < cnt; i++)
		{
			index_array[i * 2] = i;
			index_array[i * 2 + 1] = (i + 1) % cnt;
		}

		// インデックスバッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index_array;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer.reset(buffer);
		}

		SAFE_DELETE(index_array);
	}
}

void BOUNDING_SHPERE::Draw_Ring(const XMFLOAT3& rotation)
{
	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>();
		auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>();

		if (nullptr != camera01)

		{
			CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

			CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

			CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
		}
	}

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

void BOUNDING_SHPERE::Set_Radius(const float radius)
{
	Radius = max(radius, 0.01f);

	Create_Buffer();
}

const float BOUNDING_SHPERE::Get_Radius()
{
	return Radius;
}



unique_ptr<ID3D11Buffer, Release> BOUNDING_AABB::pVertexBuffer;
unique_ptr<ID3D11Buffer, Release> BOUNDING_AABB::pIndexBuffer;
const char BOUNDING_AABB::IndexNum = 24;



BOUNDING_AABB::BOUNDING_AABB()
{
	//Radius = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

BOUNDING_AABB::~BOUNDING_AABB()
{
	Uninit();
}

void BOUNDING_AABB::Init()
{
	/*{
		// 頂点バッファの確保
		VERTEX_3D vertexArray[8];

		// インデックスバッファの確保
		unsigned short indexArray[24];

		// 頂点バッファへの頂点情報の書き込み
		XMFLOAT3 max, min;

		max = XMFLOAT3(0.5f, 0.5f, 0.5f);
		min = XMFLOAT3(-0.5f, -0.5f, -0.5f);

		float x = sqrtf((max.x - min.x) * (max.x - min.x));
		float y = sqrtf((max.y - min.y) * (max.y - min.y));
		float z = sqrtf((max.z - min.z) * (max.z - min.z));
		for (int i = 0; i < 2; i++)
		{
			XMFLOAT3 height = min;
			vertexArray[i * 4 + 0].Position = XMFLOAT3(height.x, height.y + i * y, height.z + z);
			vertexArray[i * 4 + 1].Position = XMFLOAT3(height.x + x, height.y + i * y, height.z + z);
			vertexArray[i * 4 + 2].Position = XMFLOAT3(height.x + x, height.y + i * y, height.z);
			vertexArray[i * 4 + 3].Position = XMFLOAT3(height.x, height.y + i * y, height.z);
		}
		for (int i = 0; i < 8; i++)
		{
			vertexArray[i].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertexArray[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertexArray[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// インデックスバッファへインデックスの書き込み
		for (int i = 0; i < 4; i++)
		{
			indexArray[i * 2] = i;
			indexArray[i * 2 + 1] = (i + 1) % 4;

			indexArray[8 + i * 2] = 4 + i;
			indexArray[8 + i * 2 + 1] = 4 + (i + 1) % 4;
		}
		indexArray[16] = 0;
		indexArray[17] = 4;
		indexArray[18] = 1;
		indexArray[19] = 5;
		indexArray[20] = 2;
		indexArray[21] = 6;
		indexArray[22] = 3;
		indexArray[23] = 7;

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * 8;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = vertexArray;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}

		// インデックスバッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(WORD) * 24;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = indexArray;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer.reset(buffer);
		}
	}*/



	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		VERTEX_3D Vertex[8];

		Vertex[0].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
		Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		Vertex[0].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[1].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
		Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		Vertex[1].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[1].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[3].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
		Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		Vertex[3].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[3].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[2].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
		Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		Vertex[2].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[2].TexCoord = XMFLOAT2(0.0f, 0.0f);


		Vertex[4].Position = XMFLOAT3(-0.5f, 0.5f, 0.5f);
		Vertex[4].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		Vertex[4].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[4].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[5].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);
		Vertex[5].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		Vertex[5].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[5].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[7].Position = XMFLOAT3(-0.5f, -0.5f, 0.5f);
		Vertex[7].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		Vertex[7].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[7].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[6].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);
		Vertex[6].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		Vertex[6].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
		Vertex[6].TexCoord = XMFLOAT2(0.0f, 0.0f);

		for (char i = 0; i < 8; i++)
		{
			Vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
			Vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * 8;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer.get())
	{
		ID3D11Buffer* buffer;

		const WORD index[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,

		0, 4,
		1, 5,
		2, 6,
		3, 7

		//0, 1,
		//1, 3,
		//3, 2,
		//2, 0,

		//4, 5,
		//5, 7,
		//7, 6,
		//6, 4,

		//0, 4,
		//1, 5,
		//2, 6,
		//3, 7
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.ByteWidth = sizeof(WORD) * 24;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		pIndexBuffer.reset(buffer);
	}

	/*// インデックスバッファの確保
	unsigned short indexArray[2];

	// インデックスバッファへインデックスの書き込み
	for (int i = 0; i < 4; i++)
	{
		indexArray[i * 2] = i;
		indexArray[i * 2 + 1] = (i + 1) % 4;
	
		indexArray[8 + i * 2] = 4 + i;
		indexArray[8 + i * 2 + 1] = 4 + (i + 1) % 4;
	}
	indexArray[16] = 0;
	indexArray[17] = 4;
	indexArray[18] = 1;
	indexArray[19] = 5;
	indexArray[20] = 2;
	indexArray[21] = 6;
	indexArray[22] = 3;
	indexArray[23] = 7;

	indexArray[0] = 0;
	indexArray[1] = 1;

	// インデックスバッファの設定
	{
		ID3D11Buffer* buffer;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.ByteWidth = sizeof(WORD) * 2;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &indexArray[0];

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		pIndexBuffer.reset(buffer);
	}*/
}

void BOUNDING_AABB::Draw()
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定.
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		CRenderer::SetIndexBuffer(pIndexBuffer.get());

		// 3Dマトリックス設定
		{
			XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																				// 拡大縮小
			world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転
			world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// 移動

			//auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>();
			//auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>();

			//if (nullptr != camera01)
			//{
			//	// シャドウマップ用の描画か?
			//	if (CManager::Get_ShadowMap()->Get_Enable())
			//	{
			//		XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
			//		XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

			//		CRenderer::Set_MatrixBuffer(world, view, proj);

			//		CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			//	}
			//	else
			//	{
			//		CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

			//		CRenderer::Set_Shader();
			//	}
			//}
			//else
			//{
			//	// シャドウマップ用の描画か?
			//	if (CManager::Get_ShadowMap()->Get_Enable())
			//	{
			//		XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
			//		XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

			//		CRenderer::Set_MatrixBuffer(world, view, proj);

			//		CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			//	}
			//	else
			//	{
			//		CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

			//		CRenderer::Set_Shader();
			//	}
			//}
		}

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		CRenderer::DrawIndexed(24, 0, 0);

		//CRenderer::GetDeviceContext()->Draw(16, 0);

		CRenderer::Set_Shader();
	}
}

void BOUNDING_AABB::Update(float delta_time)
{
}

void BOUNDING_AABB::Uninit()
{
}

//void BOUNDING_AABB::Set_Radius(const XMFLOAT3& radius)
//{
//	Radius = radius;
//}

//XMFLOAT3& const BOUNDING_AABB::Get_Radius()
//{
//	return Radius;
//}