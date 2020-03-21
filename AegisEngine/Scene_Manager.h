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

#define INTEGER		bool, char, BYTE, short, WORD, int, UINT, long, DWORD, long long, unsigned long long
#define DECIAL		float, double
#define DX_MATH		XMINT2, XMINT3, XMINT4, XMUINT2, XMUINT3, XMUINT4, XMFLOAT2, XMFLOAT3, XMFLOAT4

	typedef std::variant<INTEGER, DECIAL, DX_MATH, string> VARIANT;

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
		GamaInstance.emplace(name, object);
	}

	template <typename T>
	const T* Get_GameInstance(const string& name) {

		if (GamaInstance.end() == GamaInstance.find(name)) return nullptr;

		return std::get_if<T>(&GamaInstance[name]);
	}

	void Delete_GameInstance(const string& name);

	static SCENE* const Get_Scene();

	template <typename T>
	static void Set_Scene() {

		Scene_Change_Enable = true;

		T* scene = new T();

		if(nullptr != pScene.get()) pScene->Uninit();

		pScene.reset(scene);

		pScene->Init();
	};

	template<typename Archive>
	void serialize(Archive& ar)
	{
		//ar(GamaInstance);
	}
};

#endif // ! SCENE_MANAGER_H