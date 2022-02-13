/**
 * @file Bounding_Sphere.h
 * @brief SHPEREのヘッダーファイル
 */

#pragma once

#ifndef BOUNDING_SHPERE_H
#define BOUNDING_SHPERE_H

#include	"Bounding.h"

// 球
class BOUNDING_SHPERE : public BOUNDING {
private:
	//! 頂点バッファ
	ComPtr<ID3D11Buffer> pVertexBuffer;
	//! インデックスバッファ
	ComPtr<ID3D11Buffer> pIndexBuffer;

	//! 半径
	float Radius;

	//! インデックス数
	UINT IndexNum;

	//! SHPERE
	BoundingSphere Sphere;

	//! 実際の描画
	void Draw_Ring(const aegis::Vector3& rotation);

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BOUNDING_SHPERE() : Radius(1.0f), IndexNum(0) {}

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~BOUNDING_SHPERE() { Uninit(); }

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

	/**
	* @brief バウンディング球を取得する関数
	* @return BoundingSphere& バウンディング球(当たり判定専用)
	* @details バウンディング球(当たり判定専用)を取得する関数
	*/
	const BoundingSphere& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_SHPERE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_SHPERE)

#endif // !BOUNDING_SHPERE_H