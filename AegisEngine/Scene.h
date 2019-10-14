#pragma once

#ifndef SCENE_H
#define SCENE_H

#include	"main.h"
#include	"Renderer.h"

#include	"camera.h"
#include	"Field.h"
#include	"Player.h"
#include	"Enemy.h"
#include	"model.h"
#include	"Sprite.h"
#include	"Polygon_3D.h"
#include	"Billboard.h"
#include	"Score.h"
#include	"Circle_Shadow.h"

#include	"Grid.h"
#include	"XYZ_Axis.h"
#include	"Bounding.h"

#include	"Mesh_Field.h"
#include	"Mesh_Cylinder.h"
#include	"Mesh_Dome.h"

class GAME_OBJECT;

enum class LAYER_NAME {

	BACKGROUND = 0,
	GAMEOBJECT,
	EFFECT,
	UI,
	MAX_LAYER
};

enum class SCENE_INDEX {
	TITLE = 0,
	GAME,
	RESULT,
	MAX_INDEX
};

/**
* @brief シーンクラス
* @details シーンのクラスの基底クラス
*/
class SCENE {
private:

	friend class GAME_OBJECT;

protected:
	static list<GAME_OBJECT*> GameObjects[(int)LAYER_NAME::MAX_LAYER];

	static bool PauseEnable;

public:

	// リストへの追加
	template <typename T>
	static T* Add_Game_Object(LAYER_NAME layer)
	{
		T* object = new T();

		object->Init();

		GameObjects[(int)layer].push_back(object);

		return object;
	}

	// リストへの追加
	template <typename T>
	static T* Add_Game_Object(LAYER_NAME layer, const string& name)
	{
		T* object = new T();

		object->Init();
			
		object->Set_Object_Name(name);

		GameObjects[(int)layer].push_back(object);

		return object;
	}

	// リストから特定のオブジェクトの取得
	template <typename T>
	static T* Get_Game_Object()
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	// リストから特定の名前のオブジェクトの取得
	template <typename T>
	static T* Get_Game_Object(const string& name)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					if (name == object->Get_Object_Name())
					{
						return (T*)object;
					}
				}
			}
		}
		return nullptr;
	}

	// リストから特定のオブジェクト（複数）の取得
	template <typename T>
	static vector<T*> Get_Game_Objects()
	{
		vector<T*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}

	// リストから特定のオブジェクト（複数）の取得
	// 引数 T* 型名 name オブジェクト名
	template <typename T>
	static vector<T*> Get_Game_Objects(const string& name)
	{
		vector<T*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					if (name == object->Get_Object_Name())
					{
						objects.push_back((T*)object);
					}
				}
			}
		}
		return objects;
	}

	// リストから特定のオブジェクト（複数）の取得
	// 引数 name オブジェクト名
	static GAME_OBJECT* Get_Game_Object(const string& name)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (name == object->Get_Object_Name())
				{
					return object;
				}
			}
		}
		return nullptr;
	}

	// 全オブジェクトの取得
	static vector<GAME_OBJECT*> Get_All_Game_Object()
	{
		vector<GAME_OBJECT*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				objects.push_back(object);
			}
		}
		return objects;
	}

	SCENE() {};
	virtual ~SCENE() {
		Uninit();
	};

	/**
	* @brief 簡単な説明（～する関数）
	* @param[in] a(引数名) 引数の説明
	* @param[out] b(引数名) 引数の説明
	* @return bool 戻り値の説明
	* @details 詳細な説明
	*/
	virtual void Init(void) {
	};

	/**
	* @brief 簡単な説明（～する関数）
	* @param[in] a(引数名) 引数の説明
	* @param[out] b(引数名) 引数の説明
	* @return bool 戻り値の説明
	* @details 詳細な説明
	*/
	virtual void Draw(void) {
	};

	/**
	* @brief 簡単な説明（～する関数）
	* @param[in] a(引数名) 引数の説明
	* @param[out] b(引数名) 引数の説明
	* @return bool 戻り値の説明
	* @details 詳細な説明
	*/
	virtual void Update(void) {
		if (true == PauseEnable)	// ポーズ中
		{
			for (GAME_OBJECT* object : GameObjects[(int)LAYER_NAME::UI])
			{
					object->Update();
			}

			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				GameObjects[i].remove_if([](GAME_OBJECT* object) { return object->Destroy(); }); // リストから削除
			}
		}
		else	// ポーズ中ではない
		{
			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				for (GAME_OBJECT* object : GameObjects[i])
				{
					object->Update();
				}

				GameObjects[i].remove_if([](GAME_OBJECT* object) { return object->Destroy(); }); // リストから削除
			}
		}
	};

	/**
	* @brief 簡単な説明（～する関数）
	* @param[in] a(引数名) 引数の説明
	* @param[out] b(引数名) 引数の説明
	* @return bool 戻り値の説明
	* @details 詳細な説明
	*/
	virtual void Uninit(void) {
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				object->Uninit();
				SAFE_DELETE(object);
			}
			GameObjects[i].clear();
		}
	};

	/**
	* @brief 簡単な説明（～する関数）
	* @param[in] a(引数名) 引数の説明
	* @param[out] b(引数名) 引数の説明
	* @return bool 戻り値の説明
	* @details 詳細な説明
	*/
	static void Destroy_Game_Object(GAME_OBJECT* game_object) {
		game_object->Set_Destroy();
	};

	/**
	* @brief ポーズを実行する関数
	* @details UIだけUpdateを行います
	* @details Drawは全て行います
	*/
	static void Start_Pause() {
		PauseEnable = true;
	};

	/**
	* @brief ポーズを終了する関数
	* @details ポーズを終了する
	*/
	static void End_Pause() {
		PauseEnable = false;
	};

	/**
	* @brief ポーズが有効か無効かを返す関数
	* @details ポーズ変数を返す
	* @return bool ポーズ変数
	*/
	static const bool Get_PauseEnable() {
		return PauseEnable;
	};

	/**
	* @brief オブジェクトの個数を取得する関数
	* @details 特定レイヤーのオブジェクトの個数を返す
	* @return UINT オブジェクトの個数
	*/
	static const UINT Get_Game_Object_Count(const LAYER_NAME layer) {
		return GameObjects[(int)layer].size();
	}
};

/**
* @brief タイトルシーンクラス
* @details シーンクラスの派生クラス
*/
class TITLE : public SCENE {
private:

protected:

public:
	TITLE() {};
	~TITLE() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;
};

/**
* @brief ゲームシーンクラス
* @details シーンクラスの派生クラス
*/
class GAME : public SCENE {
private:

protected:

public:
	GAME() {};
	~GAME() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;
};

/**
* @brief リザルトシーンクラス
* @details シーンクラスの派生クラス
*/
class RESULT : public SCENE {
private:

protected:

public:
	RESULT() {};
	~RESULT() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;

	static bool Clear_Flag;
};

/**
* @brief メインメニューシーンクラス
* @details シーンクラスの派生クラス
* @details タイトルとゲームの間のシーン
*/
class MAIN_MENU : public SCENE {
private:

protected:

public:
	MAIN_MENU() {};
	~MAIN_MENU() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;

	static string Model_Name;
};



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
		if (nullptr == pScene)
			pScene = new SCENE();
	};

	~SCENE_MANAGER() {
		Uninit();
	};

	void Init() {
		pScene->Init();
	};

	void Draw() {
		pScene->Draw();
	};

	void Update() {
		pScene->Update();
	};

	void Uninit() {
		pScene->Uninit();
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

#endif // !SCENE_H