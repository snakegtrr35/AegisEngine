#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include	"Component.h"

class GAME_OBJECT {
private:
	static unordered_map<size_t, string> Object_Name_Map;

protected:
	string Object_Name;

	bool DestroyFlag;

	XMFLOAT3 Position;
	XMFLOAT3 Edit_Position;
	
	XMFLOAT3 Rotation;
	XMFLOAT3 Edit_Rotation;

	XMVECTOR Quaternion;
	XMVECTOR Edit_Quaternion;
	XMFLOAT4 Q_num;
	XMFLOAT4 Edit_Q_num;

	XMFLOAT3 Scaling;
	XMFLOAT3 Edit_Scaling;

	COMPONENT_MANEGER Component;

public:
	GAME_OBJECT() : Object_Name("none"), DestroyFlag(false), Rotation(XMFLOAT3(0.0f, 0.0f, 0.0f)), Position(XMFLOAT3(0.0f, 0.0f, 0.0f)), Scaling(XMFLOAT3(1.0f, 1.0f, 1.0f)), Quaternion(XMQuaternionIdentity()) {}
	virtual ~GAME_OBJECT() {};

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Uninit() = 0;

	void Set_Destroy() {
		DestroyFlag = true;
	};

	// ÉÅÉÇÉäè„Ç©ÇÁÇÃçÌèú
	bool Destroy() {
		if (DestroyFlag)
		{
			return true;
		}
		return false;
	};

	const string& Get_Object_Name() {
		return Object_Name;
	};

	void Set_Object_Name(const string& name) {

		hash<string> hasher;

		size_t hash = hasher(name);

		if (Object_Name_Map.find(hash) == Object_Name_Map.end())
		{
			Object_Name_Map[hash] = name;
			Object_Name = name;
		}
#ifdef _DEBUG
		else
		{
			Erroer_Message("ä˘Ç…égÇÌÇÍÇƒÇ¢ÇÈñºëOÇ≈Ç∑");
		}
#endif // _DEBUG
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

	static const unordered_map<size_t, string>& Get_Object_Name_Map() {
		return Object_Name_Map;
	}

	//template<class Archive>
	//void serialize( Archive& archive, XMFLOAT3& position, XMFLOAT3& edit_position,
	//				XMFLOAT3& rotation, XMFLOAT3& edit_rotation,
	//				XMFLOAT3& scaling, XMFLOAT3& edit_scaling,
	//				XMFLOAT4 q_num, XMFLOAT4 edit_q_num,
	//				unordered_map<size_t, string>& object_name_map,
	//				COMPONENT_MANEGER& Component )
	//{
	//	archive(position.x, position.y, position.z);
	//}
};

#endif // !GAME_OBJECT_H