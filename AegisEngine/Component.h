#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include	"main.h"

class GAME_OBJECT;

// コンポーネントクラス
class COMPONENT {
private:

protected:

	GAME_OBJECT* Owner;

	bool Enable;
	bool DestroyFlag;

public:

	COMPONENT() {
		Owner = nullptr;
		Enable = true;
		DestroyFlag = false;
	}

	virtual ~COMPONENT() {};

	void Init(GAME_OBJECT* owner) {
		Owner = owner;
	}

	virtual void Update() = 0;

	virtual void Uninit() = 0;

	void SetEnable(const bool const flag) {
		Enable = flag;
	}

	const bool const GetEnable() {
		return Enable;
	}

	void Set_Destroy() {
		DestroyFlag = true;
	}

	// メモリ上からの削除
	bool Destroy() {
		if (DestroyFlag)
		{
			delete this;
			return true;
		}
		return false;
	}
};





class COMPONENT_MANEGER {
private:

	list<COMPONENT*> Conponent_List;

protected:

public:

	// リストへのメニューオブジェクトの追加
	template <typename T>
	T* Add_Component()
	{
		for (auto object : Conponent_List)
		{
			if (typeid(T) == typeid(*object))
			{
				return (T*)object;
			}
		}

		T* object = new T();

		Conponent_List.push_back(object);

		return object;
	}

	// リストから特定のメニューオブジェクトの取得
	template <typename T>
	T* Get_Component()
	{
		for (auto object : Conponent_List)
		{
			if (typeid(T) == typeid(*object))
			{
				return (T*)object;
			}
		}
		return nullptr;
	}

	COMPONENT_MANEGER() {};

	~COMPONENT_MANEGER() {
		Uninit();
	}

	void Update() {
		for (auto object : Conponent_List)
		{
			object->Update();
		}

		Conponent_List.remove_if([](COMPONENT* object) { return object->Destroy(); }); // リストから削除
	}

	void Uninit() {
		for (auto object : Conponent_List)
		{
			SAFE_DELETE(object);
		}
		Conponent_List.clear();
	}
};

#endif // COMPONENT_H!