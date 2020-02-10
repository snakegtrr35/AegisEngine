#pragma once

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include	"Component.h"

class GAME_OBJECT;

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

		T* object = new T();

		object->Set_Owner(owner);

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

		T* object = new T();

		object->Set_Owner(owner);

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

	void Init() {
		//for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		for (const auto& object : Conponents)
		{
			object->Init();
		}
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
		Conponents.erase(std::remove_if(Conponents.begin(), Conponents.end(), [](auto& object) { return object->Destroy(); }), Conponents.end());
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

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(Conponents);
	}
};
#endif // !COMPONENT_MANAGER_H
