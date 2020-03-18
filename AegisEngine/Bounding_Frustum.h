/**
 * @file Bounding_Frustum.h
 * @brief FRUSTUMのヘッダーファイル
 */

#pragma once

#ifndef BOUNDING_SFRUSTUM_H
#define BOUNDING_SFRUSTUM_H

#include	"Bounding.h"

// 球
class BOUNDING_FRUSTUM : public BOUNDING {
private:
	//! 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! インデックスバッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! 視錐台
	BoundingFrustum Frustum;

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BOUNDING_FRUSTUM() {}

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~BOUNDING_FRUSTUM() { Uninit(); }

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
	* @brief 描画(Depth pre-pass)関数
	* @details Depth pre-passを行う関数
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
	void Draw_Inspector() override {};

	/**
	* @brief 頂点バッファの更新関数
	* @details 頂点バッファの更新をする関数
	*/
	void OverWrite() override;

	/**
	* @brief バウンディング視錐台を取得する関数
	* @return BoundingFrustum& バウンディング視錐台(当たり判定専用)
	* @details バウンディング視錐台(当たり判定専用)を取得する関数
	*/
	const BoundingFrustum& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_FRUSTUM)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_FRUSTUM)

#endif // !BOUNDING_FRUSTUM_H