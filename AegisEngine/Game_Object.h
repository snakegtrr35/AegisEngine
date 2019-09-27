#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include	"Component.h"

class GAME_OBJECT {
private:
	static unordered_set<string> Object_Name_Set;

protected:
	string Object_Name;

	bool DestroyFlag;

	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scaling;

	COMPONENT_MANEGER Component;

public:
	GAME_OBJECT() {
		Object_Name = "none";
		DestroyFlag = false; 
		Rotation = Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
	};
	virtual ~GAME_OBJECT() {};

	virtual void Init(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Update(void) = 0;
	virtual void Uninit(void) = 0;

	void Set_Destroy() {
		DestroyFlag = true;
	};

	// ÉÅÉÇÉäè„Ç©ÇÁÇÃçÌèú
	bool Destroy() {
		if (DestroyFlag)
		{
			Uninit();
			delete this;
			return true;
		}
		return false;
	};

	const string& Get_Object_Name() {
		return Object_Name;
	};

	void Set_Object_Name(const string& name) {

		if (Object_Name_Set.find(name) == Object_Name_Set.end())
		{
			Object_Name_Set.insert(name);
			Object_Name = name;
		}
		else
		{
			Object_Name_Set.insert("none");
		}
	};


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

	COMPONENT_MANEGER* const Get_Component() {
		return &Component;
	}
};

#endif // !GAME_OBJECT_H
