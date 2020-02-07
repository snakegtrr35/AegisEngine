#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

//#include	"Scene.h"
class SCENE;

/**
* @brief シーン管理クラス
* @details シーンクラスを管理するためのクラス
*/
class SCENE_MANAGER {
private:
	static unique_ptr<SCENE> pScene;

protected:

public:
	SCENE_MANAGER() {}

	~SCENE_MANAGER() { Uninit(); }

	void Init();

	void Draw();

	void Draw_DPP();

	void Update(float delta_time);

	void Uninit();

	static SCENE* const Get_Scene() {
		return pScene.get();
	};

	template <typename T>
	static void Set_Scene() {
		T* scene = new T();

		pScene.reset(scene);

		pScene->Init();
	};
};

#endif // ! SCENE_MANAGER_H