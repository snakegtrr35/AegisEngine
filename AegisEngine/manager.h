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

	UINT ThreaCount;


public:

	CManager() {};
	~CManager() { Uninit(); };

	static bool Init();

	static CManager* Get_Instance();

	static void Delete();

	void Uninit();
	void Update();
	void Draw();

	

	SCENE* const Get_Scene();
	SHADOW_MAP* const Get_ShadowMap();
	bool Get_GameEnd();
	void GameEnd();

	const UINT Get_ThreadCount();

};

#endif // !MANAGER_H