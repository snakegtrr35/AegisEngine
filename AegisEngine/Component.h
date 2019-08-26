#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include	"main.h"

class GAME_OBJECT;

// �R���|�[�l���g�N���X
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

	// �������ォ��̍폜
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

	// ���X�g�ւ̃��j���[�I�u�W�F�N�g�̒ǉ�
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

	// ���X�g�������̃��j���[�I�u�W�F�N�g�̎擾
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

		Conponent_List.remove_if([](COMPONENT* object) { return object->Destroy(); }); // ���X�g����폜
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