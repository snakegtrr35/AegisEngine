#pragma once

#ifndef SCENE_H
#define SCENE_H

#include	"Game_Object.h"
#include	"Light.h"


template <typename T>
bool Empty_weak_ptr(const weak_ptr<T>& w)
{
	bool flag = (!w.owner_before(weak_ptr<T>{})) && (!weak_ptr<T>{}.owner_before(w));

	//if (nullptr == w.get())
	//{
	//	flag = false;
	//}

	return flag;
}

enum class LAYER_NAME {

	BACKGROUND = 0,		//! 背景のためのレイヤー
	GAMEOBJECT,			//! ゲームオブジェクトのためのレイヤー
	EFFECT,				//! エフェクトのためのレイヤー
	//DEBUG,			//! デバッグ標準のためのレイヤー
	UI,					//! UIのためのレイヤー
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

protected:
	static list<shared_ptr<GAME_OBJECT>> GameObjects[(int)LAYER_NAME::MAX_LAYER];

	static bool PauseEnable;

	static LIGHTS Light_Manager;

	static std::mutex Load_Mutex;

	static bool Load_Enable;

public:

	//// リストへの追加
	template <typename T>
	static T* Add_Game_Object(LAYER_NAME layer, const string& name)
	{
		//T* object = new T();
		shared_ptr<T> object(new T());

		object->Set_Object_Name(name);

		GameObjects[(int)layer].emplace_back(object);

		return object.get();
	}

	// リストから特定の名前のオブジェクトの取得
	template <typename T>
	static weak_ptr<T> Get_Game_Object(const string& name = "none")
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object : GameObjects[i])
			{
				if (!GameObjects[i].empty())
				{
					if (typeid(T) == typeid(*object.get()))
					{
						if (name == object.get()->Get_Object_Name())
						{
							weak_ptr<T> obj(static_pointer_cast<T>(object));

							return  obj;
						}
					}
				}
				else
				{
					continue;
				}
			}
		}

		weak_ptr<T> obj;
		return obj;
	}

	// リストから特定のオブジェクの取得
	// 引数 name オブジェクト名
	static GAME_OBJECT* Get_Game_Object(const string& name)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					if (name == object->get()->Get_Object_Name())
					{
						return object->get();
					}
				}
				else
				{
					continue;
				}
			}
		}
		return nullptr;
	}

	// リストから特定のオブジェクの取得
	// 引数 name オブジェクト名

	static weak_ptr<GAME_OBJECT> Get_Game_Object(const GAME_OBJECT* me)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					if (me == object->get())
					{
						weak_ptr<GAME_OBJECT> obj(*object);

						return  obj;
					}
				}
				else
				{
					continue;
				}
			}
		}
		weak_ptr<GAME_OBJECT> obj;
		return obj;
	}

	// リストから特定のオブジェクト（複数）の取得
	template <typename T>
	static vector<T*> Get_Game_Objects()
	{
		vector<T*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					if (typeid(T) == typeid(*object->get()))
					{
						objects.emplace_back((T*)object->get());
					}
				}
				else
				{
					continue;
				}
			}
		}
		return objects;
	}

	// 全オブジェクトの取得
	static vector<GAME_OBJECT*> Get_All_Game_Object()
	{
		vector<GAME_OBJECT*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					objects.push_back(object->get());
				}
				else
				{
					continue;
				}
			}
		}
		return objects;
	}

	// 全オブジェクトの取得
	template <typename T>
	static void Delete_Game_Objects()
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			if (!GameObjects[i].empty())
			{
				for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
				{
					if (typeid(T) == typeid(*object->get()))
					{
						object->get()->Set_Destroy();
					}
				}
			}
		}

		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
		}
	}

	SCENE();
	virtual ~SCENE() {
		Uninit();
	}

	/**
	* @brief 簡単な説明（～する関数）
	* @param[in] a(引数名) 引数の説明
	* @param[out] b(引数名) 引数の説明
	* @return bool 戻り値の説明
	* @details 詳細な説明
	*/
	virtual void Init(void) {
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->get()->Init();
			}
			Light_Manager.Init();
		}

		End_Pause();
	};

	/**
	* @brief 簡単な説明（～する関数）
	* @param[in] a(引数名) 引数の説明
	* @param[out] b(引数名) 引数の説明
	* @return bool 戻り値の説明
	* @details 詳細な説明
	*/
	virtual void Draw(void) {
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->get()->Draw();
			}
		}
	};

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	virtual void Draw_DPP(void) {
		//for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		//{
		//	for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
		//	{
		//		object->get()->Draw_DPP();
		//	}
		//}

		for (int i = (int)LAYER_NAME::MAX_LAYER - 1; (int)LAYER_NAME::BACKGROUND <= i; i--)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->get()->Draw_DPP();
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
	virtual void Update(float delta_time) {
		if (true == PauseEnable)	// ポーズ中
		{
			for (auto object = GameObjects[(int)LAYER_NAME::UI].begin(); object != GameObjects[(int)LAYER_NAME::UI].end(); object++)
			{
					object->get()->Update(delta_time);
			}

			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
			}
		}
		else	// ポーズ中ではない
		{
			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				//for (GAME_OBJECT* object : GameObjects[i])
				for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
				{
					object->get()->Update(delta_time);
				}

				GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
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
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->reset();
			}
			GameObjects[i].clear();
		}
		Light_Manager.Uninit();
	};

	/**
	* @brief ロード終了を知らせる関数
	* @details ロード終了を知らせるフラグを終了する
	*/
	static void SetLockLoad();

	/**
	* @brief ロード中かを判定する関数
	* @return bool ロード中かを判定するフラグ( ロード中なら true )
	* @details ロード中かを判定するフラグを取得する関数
	*/
	static bool GetLockLoad();

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
		return (UINT)GameObjects[(int)layer].size();
	}

	/**
	* @brief オブジェクトの個数を取得する関数
	* @details 特定レイヤーのオブジェクトの個数を返す
	* @return UINT オブジェクトの個数
	*/
	static LIGHTS* Get_Light_Manager() {
		return &Light_Manager;
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			if(!GameObjects[i].empty()) GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
		}

		ar(GameObjects);
		ar(Light_Manager);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(GameObjects);
		ar(Light_Manager);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(GameObjects);
		ar(Light_Manager);
	}*/
};

#endif // !SCENE_H