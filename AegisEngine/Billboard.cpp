#include	"Renderer.h"
#include	"camera.h"
#include	"Billboard.h"
#include	"Scene.h"

ID3D11Buffer* BILL_BOARD::pIndexBuffer = nullptr;		// インデックスバッファ

BILL_BOARD::BILL_BOARD()
{
	pVertexBuffer = nullptr;
	pIndexBuffer = nullptr;
	Texture = nullptr;

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	WH = XMFLOAT2(1.0f, 1.0f);

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// 頂点バッファの設定
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer)
	{
		HRESULT hr;

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

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}

	// テクスチャの設定
	Texture = new TEXTURE(string("number.png"));
}

//==============================
// コンストラクタ
// position : 中心座標
// wh : 幅と高さ
//==============================
BILL_BOARD::BILL_BOARD(XMFLOAT3 position, XMFLOAT2 wh)
{
	pVertexBuffer = nullptr;
	pIndexBuffer = nullptr;
	Texture = nullptr;

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Position = position;
	WH = wh;

	// 頂点バッファの設定
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	// インデックスバッファの設定
	if(nullptr == pIndexBuffer)
	{
		HRESULT hr;

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

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}

	// テクスチャの設定
	Texture = new TEXTURE(string("number02.png"));
}

BILL_BOARD::~BILL_BOARD()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_DELETE(Texture);
}

void BILL_BOARD::Init(void)
{
}

void BILL_BOARD::Draw(void)
{
	Vertex[0].Position = XMFLOAT3(-WH.x, WH.y, 0.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(WH.x, WH.y, 0.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(-WH.x, -WH.y, 0.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(WH.x, -WH.y, 0.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファの書き換え
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
		CRenderer::GetDeviceContext()->Unmap(pVertexBuffer, 0);
	}

	// 入力アセンブラに頂点バッファを設定.
	CRenderer::SetVertexBuffers(pVertexBuffer);

	CRenderer::SetIndexBuffer(pIndexBuffer);

	// テクスチャの設定
	Texture->Set_Texture();

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX mtxViewInv;		// mtxViewの逆行列

		mtxViewInv = CCamera::Get_Camera_View();

		// 逆行列の転置行列 ( 逆行列(mtxViewInv)の生成 )
		XMVECTOR det;
		mtxViewInv = XMMatrixInverse(&det, mtxViewInv);

		// 逆行列(mtxViewInv)の値の変更
		XMFLOAT4X4 mat44;
		XMStoreFloat4x4(&mat44, mtxViewInv);

		mat44._41 = 0.0f;
		mat44._42 = 0.0f;
		mat44._43 = 0.0f;

		mtxViewInv = XMLoadFloat4x4(&mat44);

		world = XMMatrixMultiply(world, mtxViewInv);

		world *= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);							// 拡大縮小
		world *= XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);			// 回転
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);					// 移動

		CRenderer::SetWorldMatrix(&world);
	}

	CRenderer::Set_Shader(SHADER_INDEX_V::NO_LIGHT, SHADER_INDEX_P::DEFAULT);

	CRenderer::DrawIndexed(6, 0, 0);

	CRenderer::Set_Shader();
}

void BILL_BOARD::Update(void)
{
}

void BILL_BOARD::Uninit(void)
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_DELETE(Texture);
}

//////////////////////////////////////////////////

int BILL_BOARD_ANIMATION::FrameCount = 0;

BILL_BOARD_ANIMATION::BILL_BOARD_ANIMATION()
{
	WaitFrame = 0.0f;
	CreatCount = 0;
	PatternCount = 0;
	Age = 0;

	Ty = Tx = 0.f;
	Tw = Th = 0.f;

	Ty_Param = Tx_Param = -1.0f;

	Pattern_Max_X = 0;
	Pattern_Max_Y = 0;

	Texture->Set_Texture_Name(string("Explosion.png"));
}

//==============================
// コンストラクタ
// position : 中心座標
// wh : 幅と高さ
//==============================
BILL_BOARD_ANIMATION::BILL_BOARD_ANIMATION(XMFLOAT3 position, XMFLOAT2 wh)
{
	WaitFrame = 0.0f;
	CreatCount = 0;
	PatternCount = 0;
	Age = 0;

	Ty = Tx = 0.f;
	Tw = Th = 0.f;

	Ty_Param = Tx_Param = -1.0f;

	Pattern_Max_X = 0;
	Pattern_Max_Y = 0;

	Position = position;

	WH = wh;

	Texture->Set_Texture_Name(string("Explosion.png"));
}

BILL_BOARD_ANIMATION::~BILL_BOARD_ANIMATION()
{
	Uninit();
}

void BILL_BOARD_ANIMATION::Init(void)
{
}

void BILL_BOARD_ANIMATION::Draw(void)
{
	Draw(Tx_Param, Ty_Param);
}

void BILL_BOARD_ANIMATION::Draw(float tx, float ty)
{
	if (-1.0f == tx)
	{
		Tx = (float)(Tw * (PatternCount % Pattern_Max_X));
	}
	else
	{
		Tx = tx;
	}


	if (-1.0f == tx)
	{
		Ty = (float)(Th * (PatternCount / Pattern_Max_Y));
	}
	else
	{
		Ty = ty;
	}

	XMINT2* wh = Texture->Get_WH();

	// UV座標計算
	float u[2], v[2];
	u[0] = Tx / wh->x;
	v[0] = Ty / wh->y;
	u[1] = (Tx + Tw) / wh->x;
	v[1] = (Ty + Th) / wh->y;

	Vertex[0].Position = XMFLOAT3(-WH.x, WH.y, 0.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(u[0], v[0]);

	Vertex[1].Position = XMFLOAT3(WH.x, WH.y, 0.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(u[1], v[0]);

	Vertex[2].Position = XMFLOAT3(-WH.x, -WH.y, 0.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(u[0], v[1]);

	Vertex[3].Position = XMFLOAT3(WH.x, -WH.y, 0.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(u[1], v[1]);

	// 頂点バッファの書き換え
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
		CRenderer::GetDeviceContext()->Unmap(pVertexBuffer, 0);
	}

	// 入力アセンブラに頂点バッファを設定.
	CRenderer::SetVertexBuffers(pVertexBuffer);

	CRenderer::SetIndexBuffer(pIndexBuffer);

	// テクスチャの設定
	Texture->Set_Texture();

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX mtxViewInv;		// mtxViewの逆行列

		mtxViewInv = CCamera::Get_Camera_View();

		// 逆行列の転置行列 ( 逆行列(mtxViewInv)の生成 )
		XMVECTOR det;
		mtxViewInv = XMMatrixInverse(&det, mtxViewInv);

		// 逆行列(mtxViewInv)の値の変更
		XMFLOAT4X4 mat44;
		XMStoreFloat4x4(&mat44, mtxViewInv);

		mat44._41 = 0.0f;
		mat44._42 = 0.0f;
		mat44._43 = 0.0f;

		mtxViewInv = XMLoadFloat4x4(&mat44);

		world = XMMatrixMultiply(world, mtxViewInv);

		world *= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);						// 拡大縮小

		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);				// 移動
		//world *= XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);		// 回転

		CRenderer::SetWorldMatrix(&world);
	}

	CRenderer::Set_Shader(SHADER_INDEX_V::NO_LIGHT, SHADER_INDEX_P::DEFAULT);

	CRenderer::DrawIndexed(6, 0, 0);

	CRenderer::Set_Shader();
}

void BILL_BOARD_ANIMATION::Update(void)
{
	Age++;

	PatternCount = (int)(Age / WaitFrame);

	if ((Pattern_Max_X * Pattern_Max_Y) <= PatternCount)
	{
		SCENE::Destroy_Game_Object(this);
	}
}

void BILL_BOARD_ANIMATION::Uninit(void)
{
}

// テクスチャアニメーションのパラメーターの設定
//
// 引数:wait_frame ... 待ちフレーム
//      tw ... テクスチャ切り取り幅
//      th ... テクスチャ切り取り高さ
void BILL_BOARD_ANIMATION::SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y)
{
	WaitFrame = wait_frame;
	Pattern_Max_X = x;
	Pattern_Max_Y = y;

	XMINT2* wh = Texture->Get_WH();

	Tw = (float)(wh->x / Pattern_Max_X);
	Th = (float)(wh->y / Pattern_Max_Y);
}

// テクスチャアニメーションのテクスチャ切り取り座標の設定
	//
	// 引数:tx_param ... テクスチャ切り取り幅（手動）
	//      ty_param ... テクスチャ切り取り高さ（手動）
void BILL_BOARD_ANIMATION::Set_Param_Txy(const float& tx_param, const float& ty_param )
{
	Tx_Param = tx_param;
	Ty_Param = ty_param;
}