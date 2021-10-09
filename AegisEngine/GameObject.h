/**
 * @file Game_Object.h
 * @brief Game_Objectのヘッダーファイル
 */

#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include	"Component_Manager.h"
#include	"Transform.h"
#include	"include/engine/core/uuid/uuid.h"

 /**
 * @brief ゲームオブジェクトクラス
 * @details ゲームオブジェクトクラス
 */
class GAME_OBJECT {
private:
	//! ゲームオブジェクトの名称一覧(静的なもの)
	static unordered_set<string> Object_Name_Map;

protected:

	//! オブジェクトの名前
	string Object_Name;

	//! 削除するかのフラグ
	bool DestroyFlag;

	////! 座標
	//XMFLOAT3 Position;
	//
	////! 回転量
	//XMFLOAT3 Rotation;
	//
	////! 拡大縮小値
	//XMFLOAT3 Scaling;

	Aegis::Transform Transform;

	//! コンポーネント
	unique_ptr<COMPONENT_MANEGER, Delete> Component;

	Aegis::uuid Uuid;

	////! クオータニオン
	//XMVECTOR Quaternion;
	////! デバッグ用のクオータニオン
	//XMVECTOR Edit_Quaternion;
	////! クオータニオン用の入れ物
	//XMFLOAT4 Q_num;

#ifdef _DEBUG
	//XMFLOAT3 Edit_Position;				//! デバッグ用の座標
	//XMFLOAT3 Edit_Rotation;				//! デバッグ用の回転量
	//XMFLOAT3 Edit_Scaling;				//! デバッグ用の拡大縮小値

	//XMFLOAT4 Edit_Q_num;				//! デバッグ用のクオータニオン用の入れ物
#endif // _DEBUG

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	GAME_OBJECT();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	virtual ~GAME_OBJECT();

	/**
	* @brief 初期化関数
	* @details 仮想初期化関数
	*/
	virtual void Init();

	/**
	* @brief 描画関数
	* @details 仮想描画関数
	*/
	virtual void Draw();

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details 純粋仮想Depth-pre-pass関数
	*/
	virtual void Draw_DPP() = 0;

	/**
	* @brief 更新関数
	* @details 仮想更新関数
	*/
	virtual void Update(float delta_time);

	/**
	* @brief 終了処理関数
	* @details 純粋仮想終了処理関数
	*/
	virtual void Uninit() = 0;

	/**
	* @brief 削除要請関数
	* @details 削除をしてもらう為の関数(削除フラグを立てる)
	*/
	void Set_Destroy();

	/**
	* @brief 削除判定関数
	* @return bool 削除判定のフラグ
	* @details 削除をするか判断する関数(手動では呼ばないで下さい)
	*/
	bool Destroy();

	/**
	* @brief 名前を取得する関数
	* @return string オブジェクトの名前
	* @details オブジェクトの名前を取得する関数
	*/
	const string& Get_Object_Name() {
		return Object_Name;
	};

	/**
	* @brief 名前を設定する関数
	* @param name 名前(string)
	* @details オブジェクトの名前を設定する関数
	*/
	void Set_Object_Name(const string& name);

	///**
	//* @brief 座標を取得する関数
	//* @return XMFLOAT3* 座標(XMFLOAT3*)
	//* @details　オブジェクトの座標を取得する関数
	//*/
	//XMFLOAT3* const Get_Position() {
	//	return &Position;
	//};

	///**
	//* @brief 回転を取得する関数
	//* @return XMFLOAT3* 回転(XMFLOAT3*)
	//* @details　オブジェクトの回転を取得する関数
	//*/
	//XMFLOAT3* const Get_Rotation() {
	//	return &Rotation;
	//};

	///**
	//* @brief スケートを取得する関数
	//* @return XMFLOAT3* スケート(XMFLOAT3*)
	//* @details　オブジェクトのスケートを取得する関数
	//*/
	//XMFLOAT3* const Get_Scaling() {
	//	return &Scaling;
	//};

	///**
	//* @brief 座標を設定する関数
	//* @param position 座標(XMFLOAT3*)
	//* @details オブジェクトの座標を設定する関数
	//*/
	//void Set_Position(XMFLOAT3* position) {
	//	Position = *position;
	//}

	///**
	//* @brief 座標を設定する関数
	//* @param position 座標(XMFLOAT3)
	//* @details オブジェクトの座標を設定する関数
	//*/
	//void Set_Position(XMFLOAT3& position) {
	//	Position = position;
	//};

	///**
	//* @brief 回転を設定する関数
	//* @param position 回転量(XMFLOAT3*)
	//* @details オブジェクトの回転を設定する関数
	//*/
	//void Set_Rotation(XMFLOAT3* rotation) {
	//	Rotation = *rotation;
	//};

	///**
	//* @brief 回転を設定する関数
	//* @param position 回転量(XMFLOAT3)
	//* @details オブジェクトの回転を設定する関数
	//*/
	//void Set_Rotation(XMFLOAT3& rotation) {
	//	Rotation = rotation;
	//};

	///**
	//* @brief スケールを設定する関数
	//* @param position スケール(XMFLOAT3*)
	//* @details オブジェクトのスケールを設定する関数
	//*/
	//void Set_Scaling(XMFLOAT3* scaling) {
	//	Scaling = *scaling;
	//};

	///**
	//* @brief スケールを設定する関数
	//* @param position スケール(XMFLOAT3)
	//* @details オブジェクトのスケールを設定する関数
	//*/
	//void Set_Scaling(XMFLOAT3& scaling) {
	//	Scaling = scaling;
	//};

	Aegis::Transform& Get_Transform()
	{
		return Transform;
	}

	/**
	* @brief コンポーネントを取得する関数
	* @return COMPONENT_MANEGER* 座標(COMPONENT_MANEGER*)
	* @details オブジェクトのコンポーネントを取得する関数
	*/
	COMPONENT_MANEGER* const Get_Component() {
		return Component.get();
	}

	/**
	* @brief ゲームオブジェクトネームマップを取得する関数
	* @return unordered_set<string> ゲームオブジェクトネームマップ(unordered_set<string>)
	* @details ゲームオブジェクトネームマップを取得する関数
	*/
	static const unordered_set<string>& Get_Object_Name_Map() {
		return Object_Name_Map;
	}

	template<class Archive>
	void serialize( Archive& ar)
	{
		ar(Object_Name);

		ar(Uuid);

		//ar(Position);
		//
		//ar(Rotation);
		//
		//ar(Scaling);

		ar(Transform);

		Set_Object_Name(Object_Name);

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