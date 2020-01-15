#include	"main.h"
#include	"manager.h"
#include	"Renderer.h"
#include	"Input.h"
#include	"Scene.h"
#include	"Texture_Manager.h"
#include	"Timer.h"
#include	"audio_clip.h"
#include	"ShadowMap.h"
#include	"Effekseer.h"


#ifdef _DEBUG
#include	"My_imgui.h"
#endif // _DEBUG

SCENE_MANAGER* CManager::pSceneManager = nullptr;		// シーンマネージャー
bool CManager::GameEnable = true;			// プログラム自体の終了のためのフラグ

SHADOW_MAP* CManager::pShadowMap = nullptr;

#ifdef _DEBUG
My_imgui* g_MyImgui;		// Imguiのクラス
#endif // _DEBUG

bool CManager::Init()
{
	HRESULT hr;

	// COMの初期化
	hr = CoInitializeEx(0, COINITBASE_MULTITHREADED);

	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}


	CINPUT::Init();

	CRenderer::Init();

	CRenderer::CreateRenderTexture();

#ifdef _DEBUG
	g_MyImgui = new My_imgui();
	g_MyImgui->Init(GetWindow());
#endif // _DEBUG
	AUDIO_MANAGER::Init();

	TEXTURE_MANEGER::Init();

	FONT::Init();

	// 時間関係の初期化
	TIMER::Init();
	CLOCK_TIMER::Init();

	pSceneManager = new SCENE_MANAGER();

	pSceneManager->Init();
	//pSceneManager->Set_Scene<TITLE>();
	pSceneManager->Set_Scene<MAIN_MENU>();

	EFFEKSEER_MANAGER::Init();

	//CRenderer::Change_Window_Mode();

	pShadowMap = new SHADOW_MAP();//
	pShadowMap->Init();
	pShadowMap->Set_Target(pSceneManager->Get_Scene()->Get_Game_Object<PLAYER>("player"));

	return true;
}

void CManager::Update()
{
	TIMER::Update();//
	CLOCK_TIMER::Update();

	// インプットの更新
	CINPUT::Update();

#ifdef _DEBUG
	TEXTURE_MANEGER::Update();
#endif // _DEBUG

	pSceneManager->Update(TIMER::Get_DeltaTime());

	// シャドウマップの更新
	{
		pShadowMap->Update();//
	}

	// Effekseer
	{
		EFFEKSEER_MANAGER::Update(TIMER::Get_DeltaTime());
	}

	MOUSE::Get_Mouse()->Reset_Wheel_Moveset();

	/*if (KEYBOARD::Trigger_Keyboard(VK_F1))
	{
		CRenderer::Change_Window_Mode();
	}*/

	if (KEYBOARD::Press_Keyboard(VK_ESCAPE))
	{
		CManager::GameEnd();
	}

	if (KEYBOARD::Trigger_Keyboard(VK_F2))
	{
		EFFEKSEER_MANAGER::Play("test");
	}
}

void CManager::Draw()
{
#ifdef _DEBUG
	g_MyImgui->Begin();
#endif // _DEBUG

	CRenderer::Begin();

	// シャドウマップの描画
	{
		pShadowMap->Begin();
		pSceneManager->Draw();
		pShadowMap->End();//
	}

	// 最終レンダリング
	{
		CRenderer::SetPass_Rendring();
		pShadowMap->Set();
		pSceneManager->Draw();
	}

	// Effekseer
	{
		EFFEKSEER_MANAGER::Draw();
	}

	//// Direct2D
	//{
	//	D2D1_COLOR_F color;
	//	color.r = 1.0f;
	//	color.g = 1.0f;
	//	color.b = 1.0f;
	//	color.a = 1.0f;

	//	ID2D1SolidColorBrush* brush;
	//	CRenderer::Get2DDeviceContext()->CreateSolidColorBrush(color, &brush);

	//	D2D1_RECT_F rect;
	//	rect = D2D1::RectF(100, 100, 200, 200);

	//	CRenderer::Get2DDeviceContext()->BeginDraw();
	//	//CRenderer::Get2DDeviceContext()->FillRectangle(&rect, brush);

	//	// DirectWrite

	//	color.r = 1.0f;
	//	color.g = 0.0f;
	//	color.b = 0.0f;
	//	color.a = 1.0f;

	//	brush->SetColor(color);

	//	{
	//		rect = D2D1::RectF(500, 100, 900, 200);
	//		const wstring drawText = L"Hello HELL World!!!\n地球の未来にご奉仕するにゃん！";

	//		CRenderer::Get2DDeviceContext()->DrawText(
	//			drawText.c_str(), drawText.size(), CRenderer::GetTextFormat(), &rect, brush);
	//	}

	//	{
	//		D2D1_POINT_2F points;
	//		points.x = 100.0f;
	//		points.y = 100.0f;

	//		CRenderer::Get2DDeviceContext()->DrawTextLayout(
	//			points, CRenderer::GetTextLayout(), brush);
	//	}

	//	CRenderer::Get2DDeviceContext()->EndDraw();
	//	brush->Release();
	//}

#ifdef _DEBUG
	g_MyImgui->Draw();//
	g_MyImgui->End();
	g_MyImgui->Render();//
#endif // _DEBUG

	CRenderer::End();
}

void CManager::Uninit()
{
	SAFE_DELETE(pSceneManager);

	// Effekseer
	EFFEKSEER_MANAGER::Uninit();

	FONT::Uninit();

	TEXTURE_MANEGER::Uninit();

	AUDIO_MANAGER::Uninit();

	SAFE_DELETE(pShadowMap);//

#ifdef _DEBUG
	g_MyImgui->Uninit();
	delete g_MyImgui;
#endif // _DEBUG

	CRenderer::Uninit();

	CINPUT::Uninit();

	// COMの終了処理
	CoUninitialize();
}

SCENE* const CManager::Get_Scene()
{
	if (nullptr != pSceneManager)
		return pSceneManager->Get_Scene();
	else
		return nullptr;
}

SHADOW_MAP* const CManager::Get_ShadowMap()
{
	return pShadowMap;
}

bool CManager::Get_GameEnd()
{
	return GameEnable;
}

void CManager::GameEnd()
{
	GameEnable = false;
}