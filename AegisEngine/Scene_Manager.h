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

	typedef std::variant<bool, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long, float, double> VARIANT;

	static unique_ptr<SCENE> pScene;

	unordered_map<string, VARIANT > GamaInstance;

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

	template <typename T>
	void Set_GameInstance(const string& name, T object) {
		GamaInstance[name] = object;
	}

	template <typename T>
	const T* Get_GameInstance(const string& name) {

		if (GamaInstance.end() == GamaInstance.find(name)) return nullptr;

		return std::get_if<T>(&GamaInstance[name]);
	}

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