#include	"GameObject.h"
#include	"Billboard.h"

#include	"camera.h"
#include	"Debug_Camera.h"
#include	"Renderer.h"
#include	"texture.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

IMPLEMENT_OBJECT_TYPE_INFO(GameObject, BILL_BOARD)

using namespace aegis;

ComPtr<ID3D11Buffer> BILL_BOARD::pIndexBuffer = nullptr;		// インデックスバッファ

BILL_BOARD::BILL_BOARD() : pVertexBuffer(nullptr), Texture(nullptr), WH(Vector2(1.0f, 1.0f))
{
	// テクスチャの設定
	Texture = std::make_unique<TEXTURE>();
}

//==============================
// コンストラクタ
// position : 中心座標
// wh : 幅と高さ
//==============================
BILL_BOARD::BILL_BOARD(Vector3 position, Vector2 wh) : pVertexBuffer(nullptr), Texture(nullptr), WH(wh)
{
	Get_Transform().Set_Position(position);

	// テクスチャの設定
	Texture = std::make_unique<TEXTURE>();
}

BILL_BOARD::~BILL_BOARD()
{
	Uninit();
}

void BILL_BOARD::Init()
{
	CRenderer* render = CRenderer::getInstance();

	// 頂点バッファの設定
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// 頂点バッファの生成
		hr = render->GetDevice()->CreateBuffer(&bd, nullptr, &pVertexBuffer);

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

		D3D11_BUFFER_DESC ibDesc{};
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData{};
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

void BILL_BOARD::Draw()
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		return;
	}

	CRenderer* render = CRenderer::getInstance();

	{
		VERTEX_3D vertex[4];

		vertex[0].Position = Vector3(-WH.x, WH.y, 0.0f);
		vertex[0].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = Vector2(0.0f, 0.0f);

		vertex[1].Position = Vector3(WH.x, WH.y, 0.0f);
		vertex[1].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = Vector2(1.0f, 0.0f);

		vertex[2].Position = Vector3(-WH.x, -WH.y, 0.0f);
		vertex[2].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = Vector2(0.0f, 1.0f);

		vertex[3].Position = Vector3(WH.x, -WH.y, 0.0f);
		vertex[3].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = Vector2(1.0f, 1.0f);

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
			render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
		}
	}

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer.Get());

	render->SetIndexBuffer(pIndexBuffer.Get());

	// テクスチャの設定
	Texture->Set_Texture();

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX mtxViewInv;		// mtxViewの逆行列

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			mtxViewInv = camera01.lock()->Get_Camera_View();
		}
		else
		{
			mtxViewInv = camera02.lock()->Get_Camera_View();
		}

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

		Vector3 position = *Get_Transform().Get_Position();
		Vector3 rotate = *Get_Transform().Get_Rotation();
		Vector3 scale = *Get_Transform().Get_Scaling();

		world *= XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

			render->Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

			render->Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
		}
	}

	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

	render->DrawIndexed(6, 0, 0);

	render->Set_Shader();

	GameObject::Draw();
}

void BILL_BOARD::Draw_DPP()
{
	CRenderer* render = CRenderer::getInstance();
	{
		VERTEX_3D vertex[4];

		vertex[0].Position = Vector3(-WH.x, WH.y, 0.0f);
		vertex[0].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = Vector2(0.0f, 0.0f);

		vertex[1].Position = Vector3(WH.x, WH.y, 0.0f);
		vertex[1].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = Vector2(1.0f, 0.0f);

		vertex[2].Position = Vector3(-WH.x, -WH.y, 0.0f);
		vertex[2].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = Vector2(0.0f, 1.0f);

		vertex[3].Position = Vector3(WH.x, -WH.y, 0.0f);
		vertex[3].Normal = Vector3(0.0f, 0.0f, 1.0f);
		vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = Vector2(1.0f, 1.0f);

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
			render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
		}
	}

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer.Get());

	render->SetIndexBuffer(pIndexBuffer.Get());

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX mtxViewInv;		// mtxViewの逆行列

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			mtxViewInv = camera01.lock()->Get_Camera_View();
		}
		else
		{
			mtxViewInv = camera02.lock()->Get_Camera_View();
		}

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

		Vector3 position = *Get_Transform().Get_Position();
		Vector3 rotate = *Get_Transform().Get_Rotation();
		Vector3 scale = *Get_Transform().Get_Scaling();

		world *= XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);


		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	render->DrawIndexed(6, 0, 0);
}

void BILL_BOARD::Update(float delta_time)
{
	GameObject::Update(delta_time);
}

void BILL_BOARD::Uninit()
{
	Texture.reset(nullptr);
}

// 幅と高さの設定
void BILL_BOARD::SetWH(const Vector2& wh)
{
	WH = wh;
};

// テクスチャの設定
void BILL_BOARD::SetTexture(const std::string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
};

//////////////////////////////////////////////////

int BILL_BOARD_ANIMATION::FrameCount = 0;

BILL_BOARD_ANIMATION::BILL_BOARD_ANIMATION() : WaitFrame(0), CreatCount(0), PatternCount(0), Age(0), Tx(0.f), Ty(0.f), Tw(0.f), Th(0.f), Tx_Param(-1.0f), Ty_Param(-1.0f), Pattern_Max_X(0), Pattern_Max_Y(0)
{
	Texture->Set_Texture_Name("Explosion.png");
}

BILL_BOARD_ANIMATION::BILL_BOARD_ANIMATION(Vector3 position, Vector2 wh) : WaitFrame(0), CreatCount(0), PatternCount(0), Age(0), Tx(0.f), Ty(0.f), Tw(0.f), Th(0.f), Tx_Param(-1.0f), Ty_Param(-1.0f), Pattern_Max_X(0), Pattern_Max_Y(0)
{
	Get_Transform().Set_Position(position);

	WH = wh;

	Texture->Set_Texture_Name("Explosion.png");
}

BILL_BOARD_ANIMATION::~BILL_BOARD_ANIMATION()
{
	Uninit();
}

void BILL_BOARD_ANIMATION::Init()
{
	BILL_BOARD::Init();
}

void BILL_BOARD_ANIMATION::Draw()
{
	Draw(Tx_Param, Ty_Param);
}

void BILL_BOARD_ANIMATION::Draw_DPP()
{
	Draw_DPP(Tx_Param, Ty_Param);
}

void BILL_BOARD_ANIMATION::Draw(float tx, float ty)
{
	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		return;
	}

	CRenderer* render = CRenderer::getInstance();

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

	{
		XMINT2* wh = Texture->Get_WH();
		VERTEX_3D vertex[4];

		// UV座標計算
		float u[2], v[2];
		u[0] = Tx / wh->x;
		v[0] = Ty / wh->y;
		u[1] = (Tx + Tw) / wh->x;
		v[1] = (Ty + Th) / wh->y;

		vertex[0].Position = Vector3(-WH.x, WH.y, 0.0f);
		vertex[0].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = Vector2(u[0], v[0]);

		vertex[1].Position = Vector3(WH.x, WH.y, 0.0f);
		vertex[1].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = Vector2(u[1], v[0]);

		vertex[2].Position = Vector3(-WH.x, -WH.y, 0.0f);
		vertex[2].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = Vector2(u[0], v[1]);

		vertex[3].Position = Vector3(WH.x, -WH.y, 0.0f);
		vertex[3].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = Vector2(u[1], v[1]);

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
			render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
		}
	}

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer.Get());

	render->SetIndexBuffer(pIndexBuffer.Get());

	// テクスチャの設定
	Texture->Set_Texture();

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX mtxViewInv;		// mtxViewの逆行列

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			mtxViewInv = camera01.lock()->Get_Camera_View();
		}
		else
		{
			mtxViewInv = camera02.lock()->Get_Camera_View();
		}

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

		Vector3 position = *Get_Transform().Get_Position();
		Vector3 rotate = *Get_Transform().Get_Rotation();
		Vector3 scale = *Get_Transform().Get_Scaling();

		world *= XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

			render->Set_MatrixBuffer01(*camera01.lock()->Get_Pos());

			render->Set_Shader();
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

			render->Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

			render->Set_Shader();
		}
	}

	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

	render->DrawIndexed(6, 0, 0);

	render->Set_Shader();
}

void BILL_BOARD_ANIMATION::Draw_DPP(float tx, float ty)
{
	CRenderer* render = CRenderer::getInstance();

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

	{
		XMINT2* wh = Texture->Get_WH();
		VERTEX_3D vertex[4];

		// UV座標計算
		float u[2], v[2];
		u[0] = Tx / wh->x;
		v[0] = Ty / wh->y;
		u[1] = (Tx + Tw) / wh->x;
		v[1] = (Ty + Th) / wh->y;

		vertex[0].Position = Vector3(-WH.x, WH.y, 0.0f);
		vertex[0].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = Vector2(u[0], v[0]);

		vertex[1].Position = Vector3(WH.x, WH.y, 0.0f);
		vertex[1].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = Vector2(u[1], v[0]);

		vertex[2].Position = Vector3(-WH.x, -WH.y, 0.0f);
		vertex[2].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = Vector2(u[0], v[1]);

		vertex[3].Position = Vector3(WH.x, -WH.y, 0.0f);
		vertex[3].Normal = Vector3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = Vector2(u[1], v[1]);

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
			render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
		}
	}

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer.Get());

	render->SetIndexBuffer(pIndexBuffer.Get());

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX mtxViewInv;		// mtxViewの逆行列

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			mtxViewInv = camera01.lock()->Get_Camera_View();
		}
		else
		{
			mtxViewInv = camera02.lock()->Get_Camera_View();
		}

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

		Vector3 position = *Get_Transform().Get_Position();
		Vector3 rotate = *Get_Transform().Get_Rotation();
		Vector3 scale = *Get_Transform().Get_Scaling();

		world *= XMMatrixScaling(scale.x, scale.y, scale.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		world *= XMMatrixTranslation(position.x, position.y, position.z);

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	render->DrawIndexed(6, 0, 0);
}

void BILL_BOARD_ANIMATION::Update(float delta_time)
{
	Age++;

	PatternCount = (WORD)(Age / WaitFrame);

	if ((Pattern_Max_X * Pattern_Max_Y) <= PatternCount)
	{
		SCENE::Destroy_Game_Object(this);
	}
}

void BILL_BOARD_ANIMATION::Uninit()
{
	BILL_BOARD::Uninit();
}

void BILL_BOARD_ANIMATION::SetParam(const WORD wait_frame, const BYTE x, const BYTE y)
{
	WaitFrame = wait_frame;
	Pattern_Max_X = x;
	Pattern_Max_Y = y;

	XMINT2* wh = Texture->Get_WH();

	Tw = (float)(wh->x / Pattern_Max_X);
	Th = (float)(wh->y / Pattern_Max_Y);
}

void BILL_BOARD_ANIMATION::Set_Param_Txy(const float tx_param, const float ty_param )
{
	Tx_Param = tx_param;
	Ty_Param = ty_param;
}