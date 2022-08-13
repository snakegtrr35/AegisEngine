/**
 * @file Bounding_Capsule.h
 * @brief CAPSULEのヘッダーファイル
 */

#pragma once

#ifndef BOUNDING_CAPSULE_H
#define	BOUNDING_CAPSULE_H

#include "Bounding.h"

// 球
class BOUNDING_CAPSULE : public BOUNDING {

	OBJECT_TYPE_INFO(BOUNDING, BOUNDING_CAPSULE)

private:
	//! 一つ目の頂点バッファ
	aegis::uniquePtr<aegis::Buffer> VertexBuffer;

	//! 二つ目の頂点バッファ
	aegis::uniquePtr<aegis::Buffer> VertexBuffer2;

	//! インデックスバッファ
	aegis::uniquePtr<aegis::Buffer> IndexBuffer;

	//! インデックス数
	UINT IndexNum;

	//! リングの頂点バッファ
	aegis::uniquePtr<aegis::Buffer> VertexBufferRing;

	//! リングのインデックスバッファ
	aegis::uniquePtr<aegis::Buffer> IndexBufferRing;

	//! リングのインデックス数
	UINT IndexNum_Ring;

	//! 半径
	float Radius;

	//! 高さ
	float Height;

	//! 描画(実装)
	void Draw_Body(const aegis::Vector3& position, const aegis::Vector3& rotation);

	//! 本体の描画
	void Draw_Semicircle(const aegis::Vector3& position, const aegis::Vector3& rotation);

	//! 中心の円部分の描画
	void Draw_Ring(const aegis::Vector3& position, const aegis::Vector3& rotation);

	// !本体の初期化
	void Init_Body();

	//! 中心の円部分の初期化
	void Init_Ring();

public:
	BOUNDING_CAPSULE() {}
	~BOUNDING_CAPSULE() { Uninit(); }

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
	* @brief 半径を設定する関数
	* @param radius(半径) 半径
	*/
	void Set_Radius(const float radius);

	/**
	* @brief 半径を取得する関数
	* @return float 半径
	* @details 半径を取得する関数
	*/
	const float Get_Radius();

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("BOUNDING", cereal::base_class<BOUNDING>(this)));
		archive(cereal::make_nvp("Radius", Radius));
		archive(cereal::make_nvp("Height", Height));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("BOUNDING", cereal::base_class<BOUNDING>(this)));
		archive(cereal::make_nvp("Radius", Radius));
		archive(cereal::make_nvp("Height", Height));
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_CAPSULE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_CAPSULE)

#endif // !BOUNDING_CAPSULE_H