#include	"Scene_Manager.h"
#include	"Scene.h"

unique_ptr<SCENE> SCENE_MANAGER::pScene;
bool SCENE_MANAGER::Scene_Change_Enable = false;

void SCENE_MANAGER::Init()
{
	{
		string name("GameInstance");

		ifstream file(name + ".dat", std::ios::binary);

		bool flag = file.is_open();

		if (flag)
		{
			cereal::BinaryInputArchive archive(file);
			archive(*this);
		}
	}
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

#include	"Bullet.h"
#include	"Billboard.h"

void SCENE_MANAGER::Uninit()
{
	{
		pScene->Delete_Game_Objects<BULLET>();
		pScene->Delete_Game_Objects<BILL_BOARD_ANIMATION>();
	}

	pScene.reset(nullptr);

	{
		string name("GameInstance");

		ofstream file(name + ".dat", std::ios::binary);

		cereal::BinaryOutputArchive archive(file);
		archive(*this);
	}
}

void SCENE_MANAGER::Delete_GameInstance(const string& name)
{
	GamaInstance.erase(name);
}

const bool SCENE_MANAGER::Get_Scene_Change_Enable()
{
	return Scene_Change_Enable;
}

SCENE* const SCENE_MANAGER::Get_Scene()
{
	return pScene.get();
};