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

SCENE_MANAGER* CManager::pSceneManager;		// �V�[���}�l�[�W���[
bool CManager::GameEnable = true;			// �v���O�������̂̏I���̂��߂̃t���O

#ifdef _DEBUG
My_imgui* g_MyImgui;		// Imgui�̃N���X
#endif // _DEBUG

bool CManager::Init()
{
	HRESULT hr;

	// DirectXTex�̏�����
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

	if (nullptr == pSceneManager)
		pSceneManager = new SCENE_MANAGER();

	AUDIO_MANAGER::Init();

	pSceneManager->Init();
	//pSceneManager->Set_Scene<TITLE>();
	pSceneManager->Set_Scene<MAIN_MENU>();

	TEXTURE_MANEGER::Init();

	FONT::Init();

	// ���Ԋ֌W�̏�����
	TIMER::Init();
	CROCK_TIMER::Init();

	return true;
}

void CManager::Update()
{
	// �C���v�b�g�̍X�V
	CINPUT::Update();

	pSceneManager->Update();
}

void CManager::Draw()
{
#ifdef _DEBUG
	g_MyImgui->Begin();
#endif // _DEBUG

	CROCK_TIMER::Update();

	CRenderer::Begin();

	//CRenderer::SetRenderTargetView(true);
	//pSceneManager->Draw();//

	CRenderer::SetRenderTargetView(false);
	pSceneManager->Draw();//
	
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

	// DirectXTex�̏I������
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