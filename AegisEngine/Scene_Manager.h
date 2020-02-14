#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class SCENE;

/**
* @brief シーン管理クラス
* @details シーンクラスを管理するためのクラス
*/
class SCENE_MANAGER {
private:
	static unique_ptr<SCENE> pScene;

	static bool Scene_Change_Enable;

protected:

public:
	SCENE_MANAGER() {}

	~SCENE_MANAGER() { Uninit(); }

	void Init();

	void Draw();

	void Draw_DPP();

	void Update(float delta_time);

	void Uninit();

	const bool Get_Scene_Change_Enable();

	static SCENE* const Get_Scene();

	template <typename T>
	static void Set_Scene() {

		Scene_Change_Enable = true;

		T* scene = new T();

		pScene.reset(scene);

		pScene->Init();
	};
};

#endif // ! SCENE_MANAGER_H