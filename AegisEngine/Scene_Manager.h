#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include	"Scene.h"
//class SCENE;

/**
* @brief シーン管理クラス
* @details シーンクラスを管理するためのクラス
*/
class SCENE_MANAGER {
private:
	static SCENE* pScene;

protected:

public:
	SCENE_MANAGER() {
		//if (nullptr == pScene)
		//	pScene = new SCENE();
	};

	~SCENE_MANAGER() {
		Uninit();
	};

	void Init() {
		if (nullptr != pScene) pScene->Init();
	};

	void Draw() {
		pScene->Draw();
	};

	void Draw_DPP() {
		pScene->Draw_DPP();
	};

	void Update(float delta_time) {
		pScene->Update(delta_time);
	};

	void Uninit() {
		//pScene->Uninit();
		SAFE_DELETE(pScene);
	};

	static SCENE* const Get_Scene() {
		return pScene;
	};

	template <typename T>
	static void Set_Scene() {
		T* scene = new T();

		SAFE_DELETE(pScene);

		pScene = scene;

		pScene->Init();
	};
};

#endif // ! SCENE_MANAGER_H