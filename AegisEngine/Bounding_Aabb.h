/**
 * @file Bounding_Aabb.h
 * @brief AABBのヘッダーファイル
 */

#pragma once

#ifndef BOUNDING_AABB_H
#define	BOUNDING_AABB_H

#include	"Bounding.h"

 /**
 * @brief バウンディングAABBクラス
 * @details 全バウンディングの基底クラス
 * @note BOUNDINGクラス図を継承しています
 */
class BOUNDING_AABB : public BOUNDING {
private:
	//! BOXの頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;
	//! 各辺の長さ
	XMFLOAT3 Radius;

	BoundingBox Aabb;

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BOUNDING_AABB() : Radius(XMFLOAT3(0.5f, 0.5f, 0.5f)) {}

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~BOUNDING_AABB();

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
	* @brief 各辺の長さを設定する関数
	* @param XMFLOAT3& 長さ
	*/
	void Set_Radius(const XMFLOAT3& radius);

	/**
	* @brief 各辺の長さを設定する関数
	* @param XMFLOAT3* 長さ
	*/
	void Set_Radius(const XMFLOAT3* radius);

	/**
	* @brief 各辺の長さを取得する関数
	* @return XMFLOAT3* 長さ
	* @details 各辺の長さを取得する関数
	*/
	XMFLOAT3* Get_Radius();

	/**
	* @brief バウンディングAABB(当たり判定専用)を取得する関数
	* @return BoundingBox バウンディングAABB(当たり判定専用)
	* @details バウンディングAABB(当たり判定専用)を取得する関数
	*/
	const BoundingBox& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//}
};

CEREAL_REGISTER_TYPE(BOUNDING_AABB)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_AABB)

#endif // !BOUNDING_AABB_H