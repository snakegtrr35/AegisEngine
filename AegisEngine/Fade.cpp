#include	"GameObject.h"
#include	"Fade.h"
#include	"Scene.h"
#include	"manager.h"
#include	"ShadowMap.h"
#include	"Renderer.h"

using namespace Aegis;

bool FADE::FadeEnable = false;

FADE::FADE() : WH(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f)), Color(Vector4(0.f, 0.f, 0.f, 1.0f)), Cnt(0), Time(0), AdditionalAlpha(0.f)
{
	Position = Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	FadeEnable = false;
}

FADE::~FADE()
{
	Uninit();
}

void FADE::Init()
{
	SPRITE::Init();
}

void FADE::Draw()
{
	CRenderer* render = CRenderer::getInstance();

	if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable()) return;

	{
		Vertex[0].Position = Vector3(Position.x - WH.x, Position.y - WH.y, 0.0f);
		Vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[0].Diffuse = Color;
		Vertex[0].TexCoord = Vector2(0.0f, 0.0f);

		Vertex[1].Position = Vector3(Position.x + WH.x, Position.y - WH.y, 0.0f);
		Vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[1].Diffuse = Color;
		Vertex[1].TexCoord = Vector2(1.0f, 0.0f);

		Vertex[2].Position = Vector3(Position.x - WH.x, Position.y + WH.y, 0.0f);
		Vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[2].Diffuse = Color;
		Vertex[2].TexCoord = Vector2(0.0f, 1.0f);

		Vertex[3].Position = Vector3(Position.x + WH.x, Position.y + WH.y, 0.0f);
		Vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Vertex[3].Diffuse = Color;
		Vertex[3].TexCoord = Vector2(1.0f, 1.0f);

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			render->GetDeviceContext()->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
			render->GetDeviceContext()->Unmap(pVertexBuffer.Get(), 0);
		}
	}

	// 入力アセンブラに頂点バッファを設定
	render->SetVertexBuffers(pVertexBuffer.Get());

	// 入力アセンブラにインデックスバッファを設定
	render->SetIndexBuffer(pIndexBuffer.Get());

	// 2Dマトリックス設定
	render->SetWorldViewProjection2D(*Get_Transform().Get_Scaling());

	render->Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	render->DrawIndexed(6, 0, 0);

	render->Set_Shader();
}

void FADE::Update(float delta_time)
{
	static float fps = 0.f;

	fps += delta_time;

	if (ANIMETION_FRAME_60 <= fps)
	{
		Cnt++;
		fps = 0.f;
	}

	Color.w = clamp((Color.w + AdditionalAlpha), 0.0f, 1.0f);

	if (Get_FadeEnable())
	{
		CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);
		FadeEnable = true;
		Enable = false;
	}
}

void FADE::Uninit()
{
}

void FADE::Set_Time(const WORD time)
{
	Time = time;
}

bool FADE::Get_FadeEnable()
{
	return (Time <= Cnt);
}

void FADE::Set_Color(const Vector4& color)
{
	Color = color;
}

void FADE::Set_AdditionalAlpha(const WORD time, const float sign)
{
	AdditionalAlpha = (1.0f / (float)time) * sign;
}

void FADE::Start_FadeOut(const WORD time)
{
	if (nullptr == CManager::Get_Instance()->Get_Scene()->Get_Game_Object("fade"))
	{
		FADE* pFade = nullptr;

		pFade = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<FADE>(LAYER_NAME::UI, "fade");

		//pFade->Init();

		pFade->Set_Time((WORD)time);
		pFade->Set_Color(Vector4(0.f, 0.f, 0.f, 0.f));
		pFade->Set_AdditionalAlpha(time, 1.0f);
		FadeEnable = false;
	}
}

void FADE::Start_FadeIn(const WORD time)
{
	if (nullptr == CManager::Get_Instance()->Get_Scene()->Get_Game_Object("fade"))
	{
		FADE* pFade = nullptr;

		pFade = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<FADE>(LAYER_NAME::UI, "fade");

		//pFade->Init();

		pFade->Set_Time((WORD)time);
		pFade->Set_Color(Vector4(0.f, 0.f, 0.f, 1.0f));
		pFade->Set_AdditionalAlpha(time, -1.0f);
		FadeEnable = false;
	}
}

bool FADE::End_Fade()
{
	bool flag = FadeEnable;
	FadeEnable = false;
	return flag;
}

//void FADE::Set_Enable(const bool flag)
//{
//	FadeEnable = flag;
//}