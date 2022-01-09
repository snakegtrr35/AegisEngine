#pragma once

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include	"Component.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

//#include	"Bounding_Aabb.h"
//#include	"Bounding_Obb.h"
//#include	"Bounding_Shpere.h"
class BOUNDING_AABB;
class BOUNDING_OBB;
class BOUNDING_SHPERE;

class GameObject;

class COMPONENT_MANEGER {
private:

	static bool Draw_Enable_All;

	vector< unique_ptr<COMPONENT, Delete> > Conponents;

public:

	// リストへのコンポーネントの追加
	template <typename T>
	T* Add_Component(const shared_ptr<GameObject>& owner)
	{
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (typeid(T) == typeid(*object.get()))
			{
				return (T*)object.get();
			}
		}

		T* object = new T();

		object->Set_Owner(owner);

		object->Init();

		Conponents.emplace_back(object);

		return object;
	}

	// リストへのコンポーネントの追加
	template <typename T>
	T* Add_Component(const weak_ptr<GameObject>& owner)
	{
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (typeid(T) == typeid(*object.get()))
			{
				return (T*)object.get();
			}
		}

		T* object = new T();

		object->Set_Owner(owner);

		object->Init();

		Conponents.emplace_back(object);

		return object;
	}

	// リストから特定のコンポーネントの取得
	template <typename T>
	T* Get_Component() {
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (typeid(T) == typeid(*object.get()))
			{
				return (T*)object.get();
			}
		}
		return nullptr;
	}

	// リストから全てのコンポーネントの取得
	vector<COMPONENT*> Get_All_Components();

	COMPONENT_MANEGER() {}

	~COMPONENT_MANEGER() {
		Uninit();
	}

	void Init();
	void Update(float delta_time);
	void Draw();
	void Draw_DPP();
	void Uninit();

	static void Set_Draw_Enable_All(const bool flag);
	static const bool Get_Draw_Enable_All();

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(Conponents);
	}
};

#undef new

#endif // !COMPONENT_MANAGER_H
