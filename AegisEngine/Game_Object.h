#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include	"Component_Manager.h"

class GAME_OBJECT {
private:
	static unordered_set<string> Object_Name_Map;

	//friend COMPONENT_MANEGER;

protected:
	string Object_Name;					//! オブジェクトの名前

	bool DestroyFlag;					//! 削除するかのフラグ

	XMFLOAT3 Position;					//! 座標
	
	XMFLOAT3 Rotation;					//! 回転量

	//XMVECTOR Quaternion;				//! クオータニオン
	//XMVECTOR Edit_Quaternion;			//! デバッグ用のクオータニオン
	//XMFLOAT4 Q_num;					//! クオータニオン用の入れ物


	XMFLOAT3 Scaling;					//! 拡大縮小値


	unique_ptr<COMPONENT_MANEGER, Delete> Component;		//! コンポーネント

#ifdef _DEBUG
	XMFLOAT3 Edit_Position;				//! デバッグ用の座標
	XMFLOAT3 Edit_Rotation;				//! デバッグ用の回転量
	XMFLOAT3 Edit_Scaling;				//! デバッグ用の拡大縮小値

	//XMFLOAT4 Edit_Q_num;				//! デバッグ用のクオータニオン用の入れ物
#endif // _DEBUG

public:

	GAME_OBJECT();
	virtual ~GAME_OBJECT();

	virtual void Init();
	virtual void Draw();
	virtual void Draw_DPP() = 0;
	virtual void Update(float delta_time);
	virtual void Uninit() = 0;

	void Set_Destroy();

	// メモリ上からの削除
	bool Destroy();

	const string& Get_Object_Name() {
		return Object_Name;
	};

	void Set_Object_Name(const string& name);


	void Set_Object_Map(const string& name) {
			Object_Name_Map.emplace(name);
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
		return Component.get();
	}

	//static const unordered_map<size_t, string>& Get_Object_Name_Map() {
	static const unordered_set<string>& Get_Object_Name_Map() {
		return Object_Name_Map;
	}

	template<class Archive>
	void serialize( Archive& ar)
	{
		ar(Object_Name);

		ar(Position);

		ar(Rotation);

		ar(Scaling);

		Set_Object_Map(Object_Name);

		ar(Component);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(Object_Name);

	//	ar(Position);

	//	ar(Rotation);

	//	ar(Scaling);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(Object_Name);

	//	ar(Position);

	//	ar(Rotation);

	//	ar(Scaling);

	//	Set_Object_Name(Object_Name);
	//}
};

#endif // !GAME_OBJECT_H