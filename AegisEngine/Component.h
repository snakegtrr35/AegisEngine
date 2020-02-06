#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include	"Game_Object.h"

#include	"manager.h"
#include	"Scene.h"

// コンポーネントクラス
class COMPONENT : public GAME_OBJECT {
protected:

	weak_ptr<GAME_OBJECT> Owner;

	bool Enable;
	bool Draw_Enable;
	bool DestroyFlag;

public:

	COMPONENT() : Enable(true), Draw_Enable (true), DestroyFlag(false) {
		Owner.reset();
	}

	COMPONENT(const shared_ptr<GAME_OBJECT>& owner) : Owner(owner), Enable(true), Draw_Enable(true), DestroyFlag(false) {}

	virtual ~COMPONENT() {}

	virtual void Init() {}

	virtual void Draw() {}
	virtual void Draw_DPP() {}

	virtual void Update(float delta_time) {}

	virtual void Uninit() {}

	void SetEnable(const bool flag) {
		Enable = flag;
	}

	const bool GetEnable() {
		return Enable;
	}

	void SetDestroy() {
		DestroyFlag = true;
	}

	void Set_Draw_Enable(const bool flag) {
		Draw_Enable = flag;
	}

	bool Get_Draw_Enable() {
		return Draw_Enable;
	}

	// メモリ上からの削除
	bool Destroy() {
		if (DestroyFlag)
		{
			return true;
		}
		return false;
	}

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Owner);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(Owner);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(Owner);
	}*/
};

CEREAL_REGISTER_TYPE(COMPONENT)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, COMPONENT)



class COMPONENT_MANEGER {
private:

	vector< unique_ptr<COMPONENT> > Conponent_List;

public:

	// リストへのメニューオブジェクトの追加
	template <typename T>
	T* Add_Component(const shared_ptr<GAME_OBJECT>& owner)
	{
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			if (typeid(T) == typeid(*object->get()))
			{
				return (T*)object->get();
			}
		}

		T* object = new T(owner);

		Conponent_List.emplace_back(object);

		return object;
	}

	// リストへのメニューオブジェクトの追加
	template <typename T>
	T* Add_Component(const GAME_OBJECT* obj)
	{
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			if (typeid(T) == typeid(*object->get()))
			{
				return (T*)object->get();
			}
		}

		auto owner = CManager::Get_Instance()->Get_Scene()->Get_Game_Object(obj);

		T* object = new T(owner);

		Conponent_List.emplace_back(object);

		return object;
	}

	// リストから特定のメニューオブジェクトの取得
	template <typename T>
	T* Get_Component() {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			if (typeid(T) == typeid(*object->get()))
			{
				return (T*)object->get();
			}
		}
		return nullptr;
	}

	COMPONENT_MANEGER() {}

	~COMPONENT_MANEGER() {
		Uninit();
	}

	void Update(float delta_time) {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			object->get()->Update(delta_time);
		}

		//Conponent_List.remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
		//std::remove_if(Conponent_List.begin(), Conponent_List.end(), [](auto& object) { return object->Destroy(); } );
		Conponent_List.erase( remove_if(Conponent_List.begin(), Conponent_List.end(), [](auto& object) { return object->Destroy(); } ), Conponent_List.end());
	}

	void Draw() {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			if (object->get()->Get_Draw_Enable())
			{
				object->get()->Draw();
			}
		}
	}

	void Draw_DPP() {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			if (object->get()->Get_Draw_Enable())
			{
				object->get()->Draw_DPP();
			}
		}
	}

	void Uninit() {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			object->reset(nullptr);
		}
		Conponent_List.clear();
	}
};

#endif // COMPONENT_H!