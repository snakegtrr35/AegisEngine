#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GAME_OBJECT;
//#include	"Game_Object.h"
#include	"manager.h"

// コンポーネントクラス
class COMPONENT /*: public GAME_OBJECT*/ {
protected:

	weak_ptr<GAME_OBJECT> Owner;

	bool Enable;
	bool Draw_Enable;
	bool DestroyFlag;

	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scaling;

public:

	COMPONENT() : Enable(true), DestroyFlag(false), Position(XMFLOAT3(0.0f, 0.0f, 0.0f)), Rotation(XMFLOAT3(0.0f, 0.0f, 0.0f)), Scaling(XMFLOAT3(1.0f, 1.0f, 1.0f)) {
		Owner.reset();
	}

	virtual ~COMPONENT() {}

	virtual void Init() {}
	virtual void Draw() {}
	virtual void Draw_DPP() {}
	virtual void Update(float delta_time) {}
	virtual void Uninit() {}

	virtual void Draw_Inspector() = 0;

	void Set_Owner(const weak_ptr<GAME_OBJECT>& owner) {
		Owner = owner;
	}

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

	XMFLOAT3* const Get_Position() {
		return &Position;
	};

	XMFLOAT3* const Get_Rotation() {
		return &Rotation;
	};

	XMFLOAT3* const Get_Scaling() {
		return &Scaling;
	};



	void Set_Position(XMFLOAT3* position) {
		Position = *position;
	}

	void Set_Position(XMFLOAT3& position) {
		Position = position;
	};

	void Set_Rotation(XMFLOAT3* rotation) {
		Rotation = *rotation;
	};

	void Set_Rotation(XMFLOAT3& rotation) {
		Rotation = rotation;
	};

	void Set_Scaling(XMFLOAT3* scaling) {
		Scaling = *scaling;
	};

	void Set_Scaling(XMFLOAT3& scaling) {
		Scaling = scaling;
	};

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
		ar(Owner);
		ar(Position);
		ar(Rotation);
		ar(Scaling);
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

//CEREAL_REGISTER_TYPE(COMPONENT)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, COMPONENT)



//class COMPONENT_MANEGER {
//private:
//
//	vector< unique_ptr<COMPONENT> > Conponents;
//
//public:
//
//	// リストへのコンポーネントの追加
//	template <typename T>
//	T* Add_Component(const shared_ptr<GAME_OBJECT>& owner)
//	{
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{
//			if (typeid(T) == typeid(*object.get()))
//			{
//				return (T*)object.get();
//			}
//		}
//
//		T* object = new T();
//
//		object->Set_Owner(owner);
//
//		Conponents.emplace_back(object);
//
//		return object;
//	}
//
//	// リストへのコンポーネントの追加
//	template <typename T>
//	T* Add_Component(const weak_ptr<GAME_OBJECT>& owner)
//	{
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{
//			if (typeid(T) == typeid(*object.get()))
//			{
//				return (T*)object.get();
//			}
//		}
//
//		T* object = new T();
//
//		object->Set_Owner(owner);
//
//		Conponents.emplace_back(object);
//
//		return object;
//	}
//
//	// リストから特定のコンポーネントの取得
//	template <typename T>
//	T* Get_Component() {
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{
//			if (typeid(T) == typeid(*object.get()))
//			{
//				return (T*)object.get();
//			}
//		}
//		return nullptr;
//	}
//
//	// リストから全てのコンポーネントの取得
//	vector<COMPONENT*> Get_All_Components() {
//		vector<COMPONENT*> objects;
//		objects.reserve(Conponents.size());
//
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{ 
//			objects.emplace_back(object.get());
//		}
//		return objects;
//	}
//
//	COMPONENT_MANEGER() {}
//
//	~COMPONENT_MANEGER() {
//		Uninit();
//	}
//
//	void Init() {
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{
//			object->Init();
//		}
//	}
//
//	void Update(float delta_time) {
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{
//			if (object->GetEnable())
//			{
//				object->Update(delta_time);
//			}
//		}
//
//		//Conponent_List.remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
//		Conponents.erase(std::remove_if(Conponents.begin(), Conponents.end(), [](auto& object) { return object->Destroy(); } ), Conponents.end());
//	}
//
//	void Draw() {
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{
//			if (object->Get_Draw_Enable())
//			{
//				object->Draw();
//			}
//		}
//	}
//
//	void Draw_DPP() {
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (const auto& object : Conponents)
//		{
//			if (object->Get_Draw_Enable())
//			{
//				object->Draw_DPP();
//			}
//		}
//	}
//
//	void Uninit() {
//		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
//		for (auto& object : Conponents)
//		{
//			object.reset(nullptr);
//		}
//		Conponents.clear();
//	}
//
//	template<class Archive>
//	void serialize(Archive& ar)
//	{
//		ar(Conponents);
//	}
//};

#endif // COMPONENT_H!