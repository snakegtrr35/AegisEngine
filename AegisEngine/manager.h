#pragma once

#ifndef MANAGER_H
#define MANAGER_H

class SCENE_MANAGER;
class SCENE;

class SHADOW_MAP;
class My_imgui;

class CManager {
private:

	static unique_ptr<CManager> Manager;

	unique_ptr<SCENE_MANAGER, Delete> pSceneManager;
	bool GameEnable;

	unique_ptr<SHADOW_MAP, Delete> pShadowMap;

#ifdef _DEBUG
	unique_ptr<My_imgui, Delete> imgui;		// Imgui‚ÌƒNƒ‰ƒX
#endif // _DEBUG

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

	const bool Get_Mouse_Over_ImGui();

	bool Get_GameEnd();
	void GameEnd();
	void Set_ThreadCount(const UINT count);
	const UINT Get_ThreadCount();
};

#endif // !MANAGER_H