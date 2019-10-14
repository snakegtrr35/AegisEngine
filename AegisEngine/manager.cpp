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