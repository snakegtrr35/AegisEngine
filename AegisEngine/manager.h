#pragma once

#ifndef MANAGER_H
#define MANAGER_H

class SCENE_MANAGER;
class SCENE;

class SHADOW_MAP;

class CManager {
private:
	static SCENE_MANAGER* pSceneManager;
	static bool GameEnable;

	static SHADOW_MAP* pShadowMap;

public:
	static bool Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static SCENE* const Get_Scene();
	static SHADOW_MAP* const Get_ShadowMap();
	static bool Get_GameEnd();
	static void GameEnd();

};

#endif // !MANAGER_H