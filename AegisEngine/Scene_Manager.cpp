#include	"Scene_Manager.h"
#include	"Scene.h"

unique_ptr<SCENE> SCENE_MANAGER::pScene;
bool SCENE_MANAGER::Scene_Change_Enable = false;

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
	if(Scene_Change_Enable) Scene_Change_Enable = false;

	pScene->Update(delta_time);
}

void SCENE_MANAGER::Uninit()
{
	pScene.reset(nullptr);
}

const bool SCENE_MANAGER::Get_Scene_Change_Enable()
{
	return Scene_Change_Enable;
}

SCENE* const SCENE_MANAGER::Get_Scene()
{
	return pScene.get();
};