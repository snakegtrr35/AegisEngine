#include	"Scene_Manager.h"
#include	"Scene.h"

unique_ptr<SCENE> SCENE_MANAGER::pScene;

void SCENE_MANAGER::Init()
{
	if (nullptr != pScene) pScene->Init();
}

void SCENE_MANAGER::Draw()
{
	pScene->Draw();
}

void SCENE_MANAGER::Draw_DPP()
{
	pScene->Draw_DPP();
}

void SCENE_MANAGER::Update(float delta_time)
{
	pScene->Update(delta_time);
}

void SCENE_MANAGER::Uninit()
{
	pScene.reset(nullptr);
}