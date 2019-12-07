#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include	"Component.h"

class GAME_OBJECT {
private:
	static unordered_map<size_t, string> Object_Name_Map;

protected:
	string Object_Name;					//! オブジェクトの名前

	bool DestroyFlag;					//! 削除するかのフラグ

	XMFLOAT3 Position;					//! 座標
	
	XMFLOAT3 Rotation;					//! 回転量

	//XMVECTOR Quaternion;				//! クオータニオン
	//XMVECTOR Edit_Quaternion;			//! デバッグ用のクオータニオン
	//XMFLOAT4 Q_num;					//! クオータニオン用の入れ物


	XMFLOAT3 Scaling;					//! 拡大縮小値


	COMPONENT_MANEGER Component;		//! コンポーネント

#ifdef _DEBUG
	XMFLOAT3 Edit_Position;				//! デバッグ用の座標
	XMFLOAT3 Edit_Rotation;				//! デバッグ用の回転量
	XMFLOAT3 Edit_Scaling;				//! デバッグ用の拡大縮小値

	//XMFLOAT4 Edit_Q_num;				//! デバッグ用のクオータニオン用の入れ物
#endif // _DEBUG

public:
	GAME_OBJECT() : Object_Name("none"), DestroyFlag(false), Rotation(XMFLOAT3(0.0f, 0.0f, 0.0f)), Position(XMFLOAT3(0.0f, 0.0f, 0.0f)), Scaling(XMFLOAT3(1.0f, 1.0f, 1.0f))/*, Quaternion(XMQuaternionIdentity())*/ {}
	virtual ~GAME_OBJECT() {};

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Uninit() = 0;

	void Set_Destroy() {
		DestroyFlag = true;
	};

	// メモリ上からの削除
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
			Erroer_Message("既に使われている名前です");
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

	template<class Archive>
	void serialize( Archive& archive,
					string& name,
					XMFLOAT3& position,
					XMFLOAT3& rotation,
					XMFLOAT3& scaling,
					//XMFLOAT4 q_num,

#ifdef _DEBUG
					XMFLOAT3& edit_position,
					XMFLOAT3& edit_rotation,
					XMFLOAT3& edit_scaling
					//XMFLOAT4 edit_q_num,
#endif // _DEBUG

					/*unordered_map<size_t, string>& object_name_map,
					COMPONENT_MANEGER& component*/ )
	{
		archive(name);

		archive(position.x, position.y, position.z);

		archive(rotation.x, rotation.y, rotation.z);

		archive(scaling.x, scaling.y, scaling.z);


#ifdef _DEBUG
		archive(edit_position.x, edit_position.y, edit_position.z);
		archive(edit_rotation.x, edit_rotation.y, edit_rotation.z);
		archive(edit_scaling.x, edit_scaling.y, edit_scaling.z);

		//XMFLOAT4 Q_num;					//! クオータニオン用の入れ物
		//XMFLOAT4 Edit_Q_num;				//! デバッグ用のクオータニオン用の入れ物
#endif // _DEBUG

		//archive(Object_Name_Map);
		//archive(component);
	}
};

#endif // !GAME_OBJECT_H