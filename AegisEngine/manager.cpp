#include	"main.h"
#include	"manager.h"
#include	"Renderer.h"
#include	"Input.h"
#include	"Scene.h"
#include	"texture.h"
#include	"Timer.h"
#include	"audio_clip.h"

#ifdef _DEBUG
#include	"My_imgui.h"
#endif // _DEBUG

SCENE_MANAGER* CManager::pSceneManager;		// シーンマネージャー
bool CManager::GameEnable = true;			// プログラム自体の終了のためのフラグ

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

	if (nullptr == pSceneManager)
		pSceneManager = new SCENE_MANAGER();

	pSceneManager->Init();
	//pSceneManager->Set_Scene<TITLE>();
	pSceneManager->Set_Scene<MAIN_MENU>();

	//CRenderer::Change_Window_Mode();

	return true;
}

void CManager::Update()
{
	CLOCK_TIMER::Update();

	// インプットの更新
	CINPUT::Update();

	pSceneManager->Update();

	MOUSE::Reset_Wheel_Moveset();

	if (KEYBOARD::Trigger_Keyboard(VK_F5))
	{
		CRenderer::Change_Window_Mode();
	}

	if (KEYBOARD::Press_Keyboard(VK_ESCAPE) && KEYBOARD::Press_Keyboard(VK_CONTROL))
	{
		CManager::GameEnd();
	}
}

void CManager::Draw()
{
#ifdef _DEBUG
	g_MyImgui->Begin();
#endif // _DEBUG

	CRenderer::Begin();

	CRenderer::SetRenderTargetView(true);//
	pSceneManager->Draw();//

	CRenderer::SetRenderTargetView(false);
	pSceneManager->Draw();

	// Direct2D

	D2D1_COLOR_F color;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

	ID2D1SolidColorBrush* brush;
	CRenderer::Get2DDeviceContext()->CreateSolidColorBrush(color, &brush);

	D2D1_RECT_F rect;
	rect = D2D1::RectF(100, 100, 200, 200);

	CRenderer::Get2DDeviceContext()->BeginDraw();
	//CRenderer::Get2DDeviceContext()->FillRectangle(&rect, brush);

	// DirectWrite

	color.r = 1.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	color.a = 1.0f;

	brush->SetColor(color);

	{
		rect = D2D1::RectF(500, 100, 900, 200);
		const wstring drawText = L"Hello HELL World!!!\n地球の未来にご奉仕するにゃん！";

		CRenderer::Get2DDeviceContext()->DrawText(
			drawText.c_str(), drawText.size(), CRenderer::GetTextFormat(), &rect, brush);
	}

	{
		D2D1_POINT_2F points;
		points.x = 100.0f;
		points.y = 100.0f;

		CRenderer::Get2DDeviceContext()->DrawTextLayout(
			points, CRenderer::GetTextLayout(), brush);
	}

	CRenderer::Get2DDeviceContext()->EndDraw();
	brush->Release();

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

	FONT::Uninit();

	TEXTURE_MANEGER::Uninit();

	AUDIO_MANAGER::Uninit();

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

bool CManager::Get_GameEnd()
{
	return GameEnable;
}

void CManager::GameEnd()
{
	GameEnable = false;
}