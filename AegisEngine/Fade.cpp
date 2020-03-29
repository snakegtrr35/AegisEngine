#include	"Game_Object.h"
#include	"Fade.h"
#include	"Scene.h"
#include	"manager.h"
#include	"ShadowMap.h"
#include	"Renderer.h"

bool FADE::FadeEnable = false;

FADE::FADE()
{
	WH = Position = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	Color = XMFLOAT4(0.f, 0.f, 0.f, 1.0f);

	Cnt = Time = 0;

	AdditionalAlpha = 0.f;

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
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		Vertex[0].Position = XMFLOAT3(Position.x - WH.x, Position.y - WH.y, 0.0f);
		Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[0].Diffuse = Color;
		Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		Vertex[1].Position = XMFLOAT3(Position.x + WH.x, Position.y - WH.y, 0.0f);
		Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[1].Diffuse = Color;
		Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		Vertex[2].Position = XMFLOAT3(Position.x - WH.x, Position.y + WH.y, 0.0f);
		Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[2].Diffuse = Color;
		Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		Vertex[3].Position = XMFLOAT3(Position.x + WH.x, Position.y + WH.y, 0.0f);
		Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		Vertex[3].Diffuse = Color;
		Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

		// 頂点バッファの書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(pVertexBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4頂点分コピー
			CRenderer::GetDeviceContext()->Unmap(pVertexBuffer.get(), 0);
		}

		// 入力アセンブラに頂点バッファを設定
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		// 入力アセンブラにインデックスバッファを設定
		CRenderer::SetIndexBuffer(pIndexBuffer.get());

		if (nullptr == ShaderResourceView)
		{
			// テクスチャの設定
			Texture->Set_Texture();
		}
		else
		{
			CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &ShaderResourceView);
		}

		// 2Dマトリックス設定
		CRenderer::SetWorldViewProjection2D(Scaling);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		CRenderer::DrawIndexed(6, 0, 0);

		CRenderer::Set_Shader();
	}
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
	//FadeEnable = false;
}

void FADE::Set_Time(const unsigned short time)
{
	Time = time;
}

bool FADE::Get_FadeEnable()
{
	return (Time <= Cnt);
}

void FADE::Set_Color(const XMFLOAT4& const color)
{
	Color = color;
}

void FADE::Set_AdditionalAlpha(const unsigned short time, const float sign)
{
	AdditionalAlpha = (1.0f / (float)time) * sign;
}

void FADE::Start_FadeOut(const float time)
{
	if (nullptr == CManager::Get_Instance()->Get_Scene()->Get_Game_Object("fade"))
	{
		FADE* pFade = nullptr;

		pFade = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<FADE>(LAYER_NAME::UI, "fade");

		pFade->Init();

		pFade->Set_Time((WORD)time);
		pFade->Set_Color(XMFLOAT4(0.f, 0.f, 0.f, 0.f));
		pFade->Set_AdditionalAlpha((WORD)time, 1.0f);
		FadeEnable = false;
	}
}

void FADE::Start_FadeIn(const float time)
{
	if (nullptr == CManager::Get_Instance()->Get_Scene()->Get_Game_Object("fade"))
	{
		FADE* pFade = nullptr;

		pFade = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<FADE>(LAYER_NAME::UI, "fade");

		pFade->Init();

		pFade->Set_Time((WORD)time);
		pFade->Set_Color(XMFLOAT4(0.f, 0.f, 0.f, 1.0f));
		pFade->Set_AdditionalAlpha((WORD)time, -1.0f);
		FadeEnable = false;
	}
}

bool FADE::End_Fade()
{
	bool flag = FadeEnable;
	FadeEnable = false;
	return flag;
}

void FADE::Set_Enable(const bool flag)
{
	FadeEnable = flag;
}