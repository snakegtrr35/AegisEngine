#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

//class GAME_OBJECT;
#include	"Game_Object.h"
#include	"manager.h"

// �R���|�[�l���g�N���X
class COMPONENT : public GAME_OBJECT {
protected:

	weak_ptr<GAME_OBJECT> Owner;

	bool Enable;
	bool Draw_Enable;
	bool DestroyFlag;

public:

	COMPONENT() : Enable(true), DestroyFlag(false) {
		Owner.reset();
	}

	COMPONENT(const shared_ptr<GAME_OBJECT>& owner) : Owner(owner), Enable(true), DestroyFlag(false) {}

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

	// �������ォ��̍폜
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

	vector< unique_ptr<COMPONENT> > Conponents;

public:

	// ���X�g�ւ̃R���|�[�l���g�̒ǉ�
	template <typename T>
	T* Add_Component(const shared_ptr<GAME_OBJECT>& owner)
	{
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (typeid(T) == typeid(*object.get()))
			{
				return (T*)object.get();
			}
		}

		T* object = new T(owner);

		Conponents.emplace_back(object);

		return object;
	}

	// ���X�g�ւ̃R���|�[�l���g�̒ǉ�
	template <typename T>
	T* Add_Component(const weak_ptr<GAME_OBJECT>& owner)
	{
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (typeid(T) == typeid(*object.get()))
			{
				return (T*)object.get();
			}
		}

		T* object = new T(owner);

		Conponents.emplace_back(object);

		return object;
	}

	// ���X�g�������̃R���|�[�l���g�̎擾
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

	// ���X�g����S�ẴR���|�[�l���g�̎擾
	vector<COMPONENT*> Get_All_Components() {
		vector<COMPONENT*> objects;
		objects.reserve(Conponents.size());

		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{ 
			objects.emplace_back(object.get());
		}
		return objects;
	}

	COMPONENT_MANEGER() {}

	~COMPONENT_MANEGER() {
		Uninit();
	}

	void Update(float delta_time) {
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (object->GetEnable())
			{
				object->Update(delta_time);
			}
		}

		//Conponent_List.remove_if([](auto& object) { return object->Destroy(); }); // ���X�g����폜
		Conponents.erase(std::remove_if(Conponents.begin(), Conponents.end(), [](auto& object) { return object->Destroy(); } ), Conponents.end());
	}

	void Draw() {
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (object->Get_Draw_Enable())
			{
				object->Draw();
			}
		}
	}

	void Draw_DPP() {
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			if (object->Get_Draw_Enable())
			{
				object->Draw_DPP();
			}
		}
	}

	void Uninit() {
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (auto& object : Conponents)
		{
			object.reset(nullptr);
		}
		Conponents.clear();
	}
};

#endif // COMPONENT_H!