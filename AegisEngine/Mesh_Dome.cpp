//***********************************************************************************************
//
//  スカイドーム [skydome.cpp]
//
//***********************************************************************************************

//***********************************************************************************************
//  インクルードファイル
//***********************************************************************************************
#include	"main.h"
#include	"renderer.h"
#include	"texture.h"
#include	"Mesh_Dome.h"


//***********************************************************************************************
//  初期化
//***********************************************************************************************
void MESH_DOOM::Init()
{
	int cornerNum = (int)Radius * 10;

	// 頂点バッファの確保
	VertexNum = (cornerNum + 1) * (cornerNum / 4 + 1);
	VERTEX_3D* vertexArray = new VERTEX_3D[VertexNum];

	// インデックスバッファの確保
	IndexNum = (2 + (cornerNum * 2)) * cornerNum / 4 + (cornerNum / 4 - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// 頂点バッファへの頂点情報の書き込み
	float theta = 0.0f, phi = 0.0f;
	float addAngle = 2.0f * XM_PI / (float)cornerNum;
	for (int y = 0; y < cornerNum / 4 + 1; y++)
	{
		phi = 0.0f;
		for (int x = 0; x < cornerNum + 1; x++)
		{
			vertexArray[x + (cornerNum + 1) * y].Position	= XMFLOAT3(Radius * sinf(theta) * cosf(phi), Radius * cosf(theta), Radius * sinf(theta) * sinf(phi));
			vertexArray[x + (cornerNum + 1) * y].Normal		= XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertexArray[x + (cornerNum + 1) * y].Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexArray[x + (cornerNum + 1) * y].TexCoord	= XMFLOAT2(-phi / (XM_2PI), -theta / (XM_PI * 0.25f) - 1.0f);

			phi -= addAngle;
		}

		theta -= addAngle;
	}

	// 頂点インデックスバッファへ頂点インデックスの書き込み
	int indexNum = 0;
	int indexFlapX = cornerNum - 1;
	int indexFlapY = cornerNum / 4 - 1;
	for (int y = 0; y < cornerNum / 4; y++)
	{
		for (int x = 0; x < cornerNum + 1; x++)
		{
			indexArray[indexNum++] = (cornerNum + 1) * (y + 1) + x;
			indexArray[indexNum++] = (cornerNum + 1) * y + x;

			// 縮退ポリゴン用
			if (x > indexFlapX && y < indexFlapY)
			{
				indexArray[indexNum++] = (cornerNum + 1) * y + (cornerNum + 1) - 1;
				indexArray[indexNum++] = (cornerNum + 1) * (y + 1 + 1) + 0;
			}
		}
	}
	

	// 頂点バッファ生成
	if(nullptr == VertexBuffer.get())
	{
		ID3D11Buffer* pVB = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pVB);

		VertexBuffer.reset(pVB);
	}

	// インデックスバッファ生成
	if (nullptr == IndexBuffer.get())
	{
		ID3D11Buffer* pIB = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = indexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pIB);

		IndexBuffer.reset(pIB);
	}

	// メモリ解放
	SAFE_DELETE_ARRAY(vertexArray);
	SAFE_DELETE_ARRAY(indexArray);

	// トランスフォーム初期化
	Position	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling		= XMFLOAT3(1.0f, 1.0f, 1.0f);

	// テクスチャの設定
	Texture.reset(new TEXTURE(string("sky.jpg")));
}

//***********************************************************************************************
//  解放
//***********************************************************************************************
void MESH_DOOM::Uninit()
{
	VertexBuffer.reset();
	IndexBuffer.reset();
	Texture.reset();
}

//***********************************************************************************************
//  更新
//***********************************************************************************************
void MESH_DOOM::Update()
{


}

//***********************************************************************************************
//  描画
//***********************************************************************************************
void MESH_DOOM::Draw()
{
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// 拡大縮小
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// 移動

		CRenderer::SetWorldMatrix(&world);
	}

	CRenderer::SetVertexBuffers(VertexBuffer.get());
	CRenderer::SetIndexBuffer(IndexBuffer.get());

	// トポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	

	Texture.get()->Set_Texture();

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);

}
