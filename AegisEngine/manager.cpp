#include "main.h"
#include "manager.h"
#include "Renderer.h"
#include "Input.h"

#include "Scene.h"
#include "Scene_Manager.h"
#include "Title.h"
#include "Main_Menu.h"
#include "Game.h"
#include "Result.h"

#include "texture.h"
#include "TextureManager.h"
#include "Model_Manager.h"
#include "Timer.h"

//#include "audio_clip.h"
#include "./include/engine/modules/audio/audio_clip.h"

#include "ShadowMap.h"
#include "Effekseer.h"

#include "Clustered.h"

#ifdef _DEBUG
#include "My_imgui.h"
#endif // _DEBUG

#include "include/engine/core/memory/aegisAllocator.h"

#include <d3d11sdklayers.h>

//#pragma comment(lib, "./audio.lib")

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

std::unique_ptr<CManager> CManager::Manager;

using namespace aegis;

ID3D11Debug* gpD3dDebug = nullptr;



bool CManager::Init()
{
	if (nullptr == Manager.get())
	{
		Manager = std::make_unique<CManager>();
	}

	atexit(aegis::memory::AegisAllocator::uninitAllocator);

	HRESULT hr;

	UINT count = std::thread::hardware_concurrency();
	if (0 == count)
	{
		FAILDE_ASSERT;
		return false;
	}
	Manager->Set_ThreadCount(count);

	// COMの初期化
	hr = CoInitializeEx(0, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	//hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//if (FAILED(hr))
	//{
	//	FAILDE_ASSERT;
	//	return false;
	//}


	CINPUT::Init();

	if (false == CRenderer::getInstance()->Init())
	{
		return false;
	}

	audio::CreateInstance();
	audio::AudioInstance()->Init();

	TextureManager::Init();

	MODEL_MANEGER::Init();
	//MODEL_MANEGER::Get_Instance()->Add("player_neutral.fbx");

	FONT::Init();

	// 時間関係の初期化
	Timer::Init();
	//CLOCK_TIMER::Init();

	EffekseerManager::getInstance()->Init();

	Manager->cluster.reset(new CLUSTERED());
	Manager->cluster->Init();


	//CRenderer::Change_Window_Mode();

#ifdef _DEBUG
	Manager->imgui.reset(new aegis::My_imgui());
	Manager->imgui->Init(GetWindow());
#endif // _DEBUG

	Manager->pSceneManager.reset(new SCENE_MANAGER());
	Manager->pSceneManager->Init();

	Manager->pSceneManager->Set_GameInstance<int>("test", 100);

	auto i = Manager->pSceneManager->Get_GameInstance<int>("test");

	//Manager->pSceneManager->Delete_GameInstance("test");

	Manager->pSceneManager->Set_Scene<TITLE>();
	//Manager->pSceneManager->Set_Scene<MAIN_MENU>();
	//Manager->pSceneManager->Set_Scene<GAME>();

	Manager->pShadowMap.reset(new SHADOW_MAP());//
	Manager->pShadowMap->Init();

	return true;
}

void CManager::Update()
{
	Timer::Update();//
	//CLOCK_TIMER::Update();

	// インプットの更新
	CINPUT::Update();

	if (false == this->Get_Scene()->GetLockLoad())
	{
#ifdef _DEBUG
		TextureManager::Get_Instance()->Update();

		if (Play_Enable)
		{
			pSceneManager->Update((float)Timer::Get_DeltaTime());
		}
		else
		{
			auto camera = pSceneManager->Get_Scene()->Get_Game_Object("camera");
			if(nullptr != camera) camera->Update((float)Timer::Get_DeltaTime());
		}
#else
		pSceneManager->Update(Timer::Get_DeltaTime());
#endif // _DEBUG

		if (pSceneManager->Get_Scene_Change_Enable()) return;

		// シャドウマップの更新
		{
			pShadowMap->Update();//
		}

		// Effekseer
		{
			EffekseerManager::getInstance()->Update((float)Timer::Get_DeltaTime());
		}

		MOUSE::Get_Mouse()->Reset_Wheel_Moveset();

		Manager->pSceneManager->Get_Scene()->Get_Light_Manager()->Update();

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
			EffekseerManager::getInstance()->Play("test");
		}

		Manager->cluster->Update();
	}

#ifdef _DEBUG
	imgui->Update();
#endif // _DEBUG
}

void CManager::Draw()
{
	CRenderer* render = CRenderer::getInstance();

#ifdef _DEBUG
	imgui->Begin();
#endif // _DEBUG

	//render->Begin();

	// シャドウマップの描画
	{
		pShadowMap->Begin();
		pSceneManager->Draw_Shadow();
		pShadowMap->End();//
	}

	render->Begin();

	// 最終レンダリング
	{
		render->SetPassRendring();
		pShadowMap->Set();
		Manager->pSceneManager->Get_Scene()->Get_Light_Manager()->Draw();
		Manager->cluster->Draw();
		//pShadowMap->Set_SamplerState();
		pSceneManager->Draw();
	}

	// Effekseer
	{
		EffekseerManager::getInstance()->Draw();
	}

	/*// Direct2D
	{
		D2D1_COLOR_F color;
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;
		color.a = 1.0f;

		ID2D1SolidColorBrush* brush;
		render->Get2DDeviceContext()->CreateSolidColorBrush(color, &brush);

		D2D1_RECT_F rect;
		rect = D2D1::RectF(100, 100, 200, 200);

		render->Get2DDeviceContext()->BeginDraw();
		//CRenderer::Get2DDeviceContext()->FillRectangle(&rect, brush);

		// DirectWrite

		color.r = 1.0f;
		color.g = 0.0f;
		color.b = 0.0f;
		color.a = 1.0f;

		brush->SetColor(color);

		{
			rect = D2D1::RectF(500, 100, 900, 200);
			const std::wstring drawText = L"Hello HELL World!!!\n地球の未来にご奉仕するにゃん！";

			render->Get2DDeviceContext()->DrawText(
				drawText.c_str(), drawText.size(), render->GetTextFormat(), &rect, brush);
		}

		{
			D2D1_POINT_2F points;
			points.x = 100.0f;
			points.y = 100.0f;

			render->Get2DDeviceContext()->DrawTextLayout(
				points, render->GetTextLayout(), brush);
		}

		render->Get2DDeviceContext()->EndDraw();
		brush->Release();
	}*/

	{
		render->End_Draw();
	}

#ifdef _DEBUG
	if (false == this->Get_Scene()->GetLockLoad())
	{
		imgui->Draw();//
	}
	imgui->End();
	imgui->Render();//
#endif // _DEBUG

	render->End();
}

void CManager::Uninit()
{
	COMPONENT_MANEGER::getInstance()->Uninit();

	pSceneManager->Uninit();
	pSceneManager.reset(nullptr);

	//pShadowMap->Uninit();
	pShadowMap.reset(nullptr);//

#ifdef _DEBUG

	{
		//std::unique_ptr<My_imgui> temp;

		//temp.swap(imgui);
	}

	imgui->Uninit();
	imgui.reset(nullptr);
#endif // _DEBUG

	// Effekseer
	EffekseerManager::getInstance()->Uninit();

	FONT::Uninit();

	audio::AudioInstance()->Uninit();
	audio::DestroyInstance();

	CRenderer::getInstance()->Uninit();

	CINPUT::Uninit();

	//aegis::memory::AegisAllocator::uninitAllocator();

	// COMの終了処理
	CoUninitialize();

	//{
	//	// 詳細表示
	//	if (gpD3dDebug)
	//	{
	//		auto hr = gpD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//		if (FAILED(hr))
	//		{
	//			return;
	//		}
	//
	//		gpD3dDebug->Release();
	//	}
	//}
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
	return pShadowMap.get();
}

const bool CManager::Get_Mouse_Over_ImGui()
{
#ifdef _DEBUG
	return imgui->Get_Mouse_Over_Enable();
#else
	return false;
#endif // _DEBUG
}

bool CManager::Get_GameEnd()
{
	return GameEnable;
}

void CManager::GameEnd()
{
	GameEnable = false;
}

CManager* CManager::Get_Instance()
{
	return Manager.get();
}

void CManager::Delete()
{
	Manager.reset(nullptr);
}

void CManager::Set_ThreadCount(const UINT count)
{
	ThreadCount = count;
}

const UINT CManager::Get_ThreadCount()
{
	return ThreadCount;
}

#ifdef _DEBUG
void CManager::Set_Play_Enable(const bool flag)
{
	Play_Enable = flag;
}
#endif // _DEBUG