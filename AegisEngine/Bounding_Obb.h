﻿/**
 * @file Bounding_Obb.h
 * @brief OBBのヘッダーファイル
 */

#pragma once

#ifndef BOUNDING_OBB_H
#define BOUNDING_OBB_H

#include "Bounding.h"

// OBB
class BOUNDING_OBB : public BOUNDING {

	OBJECT_TYPE_INFO(BOUNDING, BOUNDING_OBB)

private:
	//! 頂点バッファ
	aegis::uniquePtr<aegis::Buffer> VertexBuffer;

	//! OBB
	BoundingOrientedBox Obb;

	//! 各辺の長さ
	aegis::Vector3 Radius;

	//! 回転量(クオータニオン)
	aegis::Vector4 Quaternion;

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BOUNDING_OBB() : Radius(aegis::Vector3(0.5f, 0.5f, 0.5f)), Quaternion(aegis::Vector4(0.f, 0.f, 0.f, 1.0f)) {}

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~BOUNDING_OBB();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init() override;

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw() override;

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP() override {}

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time) override;

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit() override;

	/**
	* @brief インスペクターでの描画関数
	* @details インスペクターでの描画をする関数
	*/
	void Draw_Inspector() override;

	/**
	* @brief 頂点バッファの更新関数
	* @details 頂点バッファの更新をする関数
	*/
	void OverWrite() override;

	/**
	* @brief バウンディングOBB(当たり判定専用)を取得する関数
	* @return BoundingOrientedBox& バウンディングOBB(当たり判定専用)
	* @details バウンディングOBB(当たり判定専用)を取得する関数
	*/
	const BoundingOrientedBox& Get_Collition();

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("BOUNDING", cereal::base_class<BOUNDING>(this)));
		archive(cereal::make_nvp("Radius", Radius));
		archive(cereal::make_nvp("Quaternion", Quaternion));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("BOUNDING", cereal::base_class<BOUNDING>(this)));
		archive(cereal::make_nvp("Radius", Radius));
		archive(cereal::make_nvp("Quaternion", Quaternion));
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_OBB)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_OBB)

#endif // !BOUNDING_OBB_H