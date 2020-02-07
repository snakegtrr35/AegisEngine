#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

//#include	"Scene.h"
class SCENE;

/**
* @brief �V�[���Ǘ��N���X
* @details �V�[���N���X���Ǘ����邽�߂̃N���X
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