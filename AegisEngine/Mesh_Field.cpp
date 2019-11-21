#include "renderer.h"
#include "texture.h"
#include "Mesh_Field.h"

#include	"manager.h"
#include	"Scene.h"

MESH_FIELD::MESH_FIELD()
{
	VertexArray = nullptr;
	IndexBuffer = VertexBuffer = nullptr;
	Texture = nullptr;

	GridSize = XMFLOAT3(2.0f, 0.0f, 2.0f);
	GridNum = XMINT2(100, 100);
};

void MESH_FIELD::Init()
{
	unsigned int VertexNum = (GridNum.x + 1) * (GridNum.y + 1);
	VertexArray = new VERTEX_3D[VertexNum];

	IndexNum = (2 + (GridNum.x * 2)) * GridNum.y + (GridNum.y - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// 頂点バッファへの書き込み
	for (int z = 0; z < GridNum.y + 1; z++)
	{
		for (int x = 0; x < GridNum.x + 1; x++)
		{
			XMFLOAT3 position;

			position.x = -0.5f * (float)GridNum.x * GridSize.x + (float)x * GridSize.x;
			//position.y = sinf(z * 0.8) * 0.3f;
			position.y = 0.0f;
			position.z = 0.5f * (float)GridNum.y * GridSize.z - (float)z * GridSize.z;

			VertexArray[x + (GridNum.x + 1) * z].Position = position;

			VertexArray[x + (GridNum.y + 1) * z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			VertexArray[x + (GridNum.x + 1) * z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			VertexArray[x + (GridNum.x + 1) * z].TexCoord = XMFLOAT2((float)x, (float)z);
		}
	}

	// 頂点バッファへの書き込み(Normal)
	for (int z = 1; z < GridNum.y; z++)
	{
		for (int x = 1; x < GridNum.x; x++)
		{
			XMFLOAT3 va, vb;

			va.x = VertexArray[(z - 1) * GridNum.y + x].Position.x - VertexArray[(z + 1) * GridNum.x + x].Position.x;
			va.y = VertexArray[(z - 1) * GridNum.y + x].Position.y - VertexArray[(z + 1) * GridNum.x + x].Position.y;
			va.z = VertexArray[(z - 1) * GridNum.y + x].Position.z - VertexArray[(z + 1) * GridNum.x + x].Position.z;

			vb.x = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.x - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.x;
			vb.y = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.y - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.y;
			vb.z = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.z - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.z;

			XMFLOAT3 vn;

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
			indexArray[indexNum++] = (GridNum.x + 1) * (z + 1) + x;
			indexArray[indexNum++] = (GridNum.x + 1) * z + x;

			// 縮退ポリゴン用にインデックスの追加
			if (x > indexFlapX && z < indexFlapZ)
			{
				indexArray[indexNum++] = (GridNum.x + 1) * z + (GridNum.x + 1) - 1;
				indexArray[indexNum++] = (GridNum.x + 1) * (z + 1 + 1) + 0;
			}
		}
	}


	// 頂点バッファの生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = VertexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);
	}

	// インデックスバッファの生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = indexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &IndexBuffer);
	}

	// メモリ解放
	SAFE_DELETE_ARRAY(indexArray);

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// テクスチャの設定
	Texture = new TEXTURE();
}

void MESH_FIELD::Uninit()
{
	SAFE_DELETE(VertexArray);
	SAFE_RELEASE(VertexBuffer);
	SAFE_RELEASE(IndexBuffer);
	SAFE_DELETE(Texture);
}

void MESH_FIELD::Update(float delta_time)
{
}

void MESH_FIELD::Draw()
{
	// 3Dマトリックス設定
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// 拡大縮小
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// 移動
		
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

	// 頂点バッファ設定
	CRenderer::SetVertexBuffers(VertexBuffer);

	// インデックスバッファ設定
	CRenderer::SetIndexBuffer(IndexBuffer);

	// テクスチャの設定
	Texture->Set_Texture();

	// トポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

const float MESH_FIELD::Get_Height(const XMFLOAT3& position)
{
	int x, z;

	XMFLOAT3 p0, p1, p2, v01, v02, n, v, hp, va = XMFLOAT3(0.f, 0.f, 0.f), vb;

	float dp0n, dvn, dpn, t;

	v = XMFLOAT3(0.0f, -10.0f, 0.0f);

	x = (position.x +  (GridSize.x * GridNum.x) * 0.5f) / GridSize.x;
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

//==============================
// テクスチャの設定
//==============================
void MESH_FIELD::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}





MESH_WALL::MESH_WALL()
{
	VertexArray = nullptr;
	IndexBuffer = VertexBuffer = nullptr;
	Texture = nullptr;

	GridSize = XMFLOAT3(1.0f, 0.0f, 1.0f);
	GridNum = XMINT2(10, 5);
};

void MESH_WALL::Init()
{
	unsigned int VertexNum = (GridNum.x + 1) * (GridNum.y + 1);
	VertexArray = new VERTEX_3D[VertexNum];

	IndexNum = (2 + (GridNum.x * 2)) * GridNum.y + (GridNum.y - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// 頂点バッファへの書き込み
	for (int z = 0; z < GridNum.y + 1; z++)
	{
		for (int x = 0; x < GridNum.x + 1; x++)
		{
			XMFLOAT3 position;

			position.x = -0.5f * (float)GridNum.x * GridSize.x + (float)x * GridSize.x;
			position.y = 0.0f;
			position.z = 0.5f * (float)GridNum.y * GridSize.z - (float)z * GridSize.z;

			VertexArray[x + (GridNum.x + 1) * z].Position = position;

			VertexArray[x + (GridNum.y + 1) * z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			VertexArray[x + (GridNum.x + 1) * z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			VertexArray[x + (GridNum.x + 1) * z].TexCoord = XMFLOAT2((float)x, (float)z);
		}
	}

	// 頂点バッファへの書き込み(Normal)
	for (int z = 1; z < GridNum.y; z++)
	{
		for (int x = 1; x < GridNum.x; x++)
		{
			XMFLOAT3 va, vb;

			va.x = VertexArray[(z - 1) * GridNum.y + x].Position.x - VertexArray[(z + 1) * GridNum.x + x].Position.x;
			va.y = VertexArray[(z - 1) * GridNum.y + x].Position.y - VertexArray[(z + 1) * GridNum.x + x].Position.y;
			va.z = VertexArray[(z - 1) * GridNum.y + x].Position.z - VertexArray[(z + 1) * GridNum.x + x].Position.z;

			vb.x = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.x - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.x;
			vb.y = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.y - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.y;
			vb.z = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.z - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.z;

			XMFLOAT3 vn;

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
			indexArray[indexNum++] = (GridNum.x + 1) * (z + 1) + x;
			indexArray[indexNum++] = (GridNum.x + 1) * z + x;

			// 縮退ポリゴン用にインデックスの追加
			if (x > indexFlapX && z < indexFlapZ)
			{
				indexArray[indexNum++] = (GridNum.x + 1) * z + (GridNum.x + 1) - 1;
				indexArray[indexNum++] = (GridNum.x + 1) * (z + 1 + 1) + 0;
			}
		}
	}


	// 頂点バッファの生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = VertexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);
	}

	// インデックスバッファの生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = indexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &IndexBuffer);
	}

	delete[] indexArray;

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// テクスチャの設定
	Texture = new TEXTURE(string("field004.png"));
}

void MESH_WALL::Uninit()
{
	SAFE_DELETE(VertexArray);
	SAFE_RELEASE(VertexBuffer);
	SAFE_RELEASE(IndexBuffer);
	SAFE_DELETE(Texture);
}

void MESH_WALL::Update(float delta_time)
{
}

void MESH_WALL::Draw()
{
	// 3Dマトリックス設定
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// 拡大縮小
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// 移動
		CRenderer::SetWorldMatrix(&world);
	}

	// 頂点バッファ設定
	CRenderer::SetVertexBuffers(VertexBuffer);

	// インデックスバッファ設定
	CRenderer::SetIndexBuffer(IndexBuffer);

	// テクスチャの設定
	Texture->Set_Texture();

	// トポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

const float MESH_WALL::Get_Height(const XMFLOAT3& position)
{
	int x, z;

	XMFLOAT3 p0, p1, p2, v01, v02, n, v, hp, va = XMFLOAT3(0.f, 0.f, 0.f), vb;

	float dp0n, dvn, dpn, t;

	v = XMFLOAT3(0.0f, -10.0f, 0.0f);

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