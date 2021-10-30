#include	"GameObject.h"
#include	"Sprite_Animation.h"
#include	"manager.h"
#include	"ShadowMap.h"

using namespace Aegis;

SPRITE_ANIMATION::SPRITE_ANIMATION()
{
	WaitFrame = 0.f;
	CreatCount = 0;
	PatternCount = 0;
	Age = 0;

	FrameCount = 0.f;

	Ty = Tx = 0.f;
	Tw = Th = 0.f;

	Pattern_Max_X = 0;
	Pattern_Max_Y = 0;

	Ty_Param = Tx_Param = -1.0f;

	SetTexture(string("Explosion.png"));
}

SPRITE_ANIMATION::SPRITE_ANIMATION(Vector2 position, Vector4 size)
{
	WaitFrame = 0.f;
	CreatCount = 0;
	PatternCount = 0;
	Age = 0;

	FrameCount = 0.f;

	Ty = Tx = 0.f;
	Tw = Th = 0.f;

	Pattern_Max_X = 0;
	Pattern_Max_Y = 0;

	Position = position;

	Size = size;

	Ty_Param = Tx_Param = -1.0f;

	SetTexture(string("Explosion.png"));
}

SPRITE_ANIMATION::~SPRITE_ANIMATION()
{
	Uninit();
}

void SPRITE_ANIMATION::Init(void)
{
	SPRITE::Init();
}

void SPRITE_ANIMATION::Draw(void)
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		Draw2(Tx_Param, Ty_Param);
	}
}

void SPRITE_ANIMATION::Draw_DPP(void)
{
	Draw_DPP2(Tx_Param, Ty_Param);
}

void SPRITE_ANIMATION::Draw2(float tx, float ty)
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

	XMINT2* wh = Texture->Get_WH();

	// UV座標計算
	float u[2], v[2];
	u[0] = (float)(Tx / wh->x);
	v[0] = (float)(Ty / wh->y);
	u[1] = (float)((Tx + Tw) / wh->x);
	v[1] = (float)((Ty + Th) / wh->y);

	Vertex[0].Position = Vector3(Position.x - Size.w, Position.y - Size.x, 0.0f);
	Vertex[0].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[0].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[0].TexCoord = Vector2(u[0], v[0]);

	Vertex[1].Position = Vector3(Position.x + Size.y, Position.y - Size.x, 0.0f);
	Vertex[1].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[1].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[1].TexCoord = Vector2(u[1], v[0]);

	Vertex[2].Position = Vector3(Position.x - Size.w, Position.y + Size.z, 0.0f);
	Vertex[2].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[2].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[2].TexCoord = Vector2(u[0], v[1]);

	Vertex[3].Position = Vector3(Position.x + Size.y, Position.y + Size.z, 0.0f);
	Vertex[3].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[3].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[3].TexCoord = Vector2(u[1], v[1]);

	// 頂点バッファの書き換え
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
		render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
	}

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer.Get());

	// 入力アセンブラにインデックスバッファを設定
	render->SetIndexBuffer(pIndexBuffer.Get());

	// テクスチャの設定
	Texture->Set_Texture();

	// 2Dマトリックス設定
	render->SetWorldViewProjection2D(*Get_Transform().Get_Scaling());

	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

	render->DrawIndexed(6, 0, 0);

	render->Set_Shader();
}

void SPRITE_ANIMATION::Draw_DPP2(float tx, float ty)
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

	XMINT2* wh = Texture->Get_WH();

	// UV座標計算
	float u[2], v[2];
	u[0] = (float)(Tx / wh->x);
	v[0] = (float)(Ty / wh->y);
	u[1] = (float)((Tx + Tw) / wh->x);
	v[1] = (float)((Ty + Th) / wh->y);

	Vertex[0].Position = Vector3(Position.x - Size.w, Position.y - Size.x, 0.0f);
	Vertex[0].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[0].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[0].TexCoord = Vector2(u[0], v[1]);

	Vertex[1].Position = Vector3(Position.x + Size.y, Position.y - Size.x, 0.0f);
	Vertex[1].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[1].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[1].TexCoord = Vector2(u[1], v[1]);

	Vertex[2].Position = Vector3(Position.x - Size.w, Position.y + Size.z, 0.0f);
	Vertex[2].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[2].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[2].TexCoord = Vector2(u[0], v[0]);

	Vertex[3].Position = Vector3(Position.x + Size.y, Position.y + Size.z, 0.0f);
	Vertex[3].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Vertex[3].Diffuse = Vector4(Color.r, Color.g, Color.b, Color.a);
	Vertex[3].TexCoord = Vector2(u[1], v[0]);

	// 頂点バッファの書き換え
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
		render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
	}

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer.Get());

	// 入力アセンブラにインデックスバッファを設定
	render->SetIndexBuffer(pIndexBuffer.Get());

	// 2Dマトリックス設定
	render->SetWorldViewProjection2D(*Get_Transform().Get_Scaling());

	render->DrawIndexed(6, 0, 0);
}

void SPRITE_ANIMATION::Update(float delta_time)
{
	FrameCount += delta_time;

	if (ANIMETION_FRAME_60 <= FrameCount)
	{
		Age++;

		FrameCount = 0.0f;
	}

	PatternCount = (int)(Age / WaitFrame);

	if ((Pattern_Max_X * Pattern_Max_Y) <= PatternCount)
	{
		//SCENE::Destroy_Game_Object(this);
	}
}

void SPRITE_ANIMATION::Uninit(void)
{
}

// テクスチャアニメーションのパラメーターの設定
//
// 引数:wait_frame ... 待ちフレーム
//      tw ... テクスチャ切り取り数（x）
//      th ... テクスチャ切り取り数（y）
void SPRITE_ANIMATION::SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y)
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
void SPRITE_ANIMATION::Set_Param_Txy(const float& tx_param, const float& ty_param)
{
	Tx_Param = tx_param;
	Ty_Param = ty_param;
}