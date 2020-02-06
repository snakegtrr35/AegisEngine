#pragma once

#ifndef MANAGER_H
#define MANAGER_H

class SCENE_MANAGER;
class SCENE;

class SHADOW_MAP;

class CManager {
private:

	static unique_ptr<CManager> Manager;

	SCENE_MANAGER* pSceneManager;
	bool GameEnable;

	SHADOW_MAP* pShadowMap;

	UINT ThreadCount;

public:

	CManager() {}
	~CManager() { Uninit(); }

	static bool Init();
	void Uninit();
	void Update();
	void Draw();

	static CManager* Get_Instance();
	static void Delete();

	SCENE* const Get_Scene();
	SHADOW_MAP* const Get_ShadowMap();
	bool Get_GameEnd();
	void GameEnd();
	void Set_ThreadCount(const UINT count);
	const UINT Get_ThreadCount();
};

#endif // !MANAGER_H