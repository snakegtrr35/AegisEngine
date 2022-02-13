#include	"Scene_Manager.h"

std::unique_ptr<SCENE> SCENE_MANAGER::pScene;
bool SCENE_MANAGER::Scene_Change_Enable = false;

void SCENE_MANAGER::Init()
{
#ifdef _DEBUG
	{
		std::string name("GameInstance");

		std::ifstream file(name + ".dat", std::ios::binary);

		bool flag = file.is_open();

		if (flag)
		{
			cereal::BinaryInputArchive archive(file);
			archive(*this);
		}
	}
#endif // _DEBUG
}

void SCENE_MANAGER::Draw()
{
	pScene->Draw();
}

void SCENE_MANAGER::Draw_Shadow()
{
	pScene->Draw_Shadow();
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

#ifdef _DEBUG
	{
		std::string name("GameInstance");

		std::ofstream file(name + ".dat", std::ios::binary);

		cereal::BinaryOutputArchive archive(file);
		archive(*this);
	}
#endif // _DEBUG
}

void SCENE_MANAGER::Delete_GameInstance(const std::string& name)
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