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

	OBJECT_TYPE_INFO(BOUNDING_AABB)

private:
	//! BOXの頂点バッファ
	ComPtr<ID3D11Buffer> pVertexBuffer;
	//! 各辺の長さ
	aegis::Vector3 Radius;

	BoundingBox Aabb;

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BOUNDING_AABB() : Radius(aegis::Vector3(0.5f, 0.5f, 0.5f)) {}

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
	* @param Vector3& 長さ
	*/
	void Set_Radius(const aegis::Vector3& radius);

	/**
	* @brief 各辺の長さを設定する関数
	* @param Vector3* 長さ
	*/
	void Set_Radius(const aegis::Vector3* radius);

	/**
	* @brief 各辺の長さを取得する関数
	* @return Vector3* 長さ
	* @details 各辺の長さを取得する関数
	*/
	aegis::Vector3* Get_Radius();

	/**
	* @brief バウンディングAABB(当たり判定専用)を取得する関数
	* @return BoundingBox バウンディングAABB(当たり判定専用)
	* @details バウンディングAABB(当たり判定専用)を取得する関数
	*/
	const BoundingBox& Get_Collition();

	/**
	* @brief 頂点バッファの更新関数
	* @param BoundingBox AABB
	*/
	void OverWrite(BoundingBox aabb);

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("BOUNDING", cereal::base_class<BOUNDING>(this)));
		archive(cereal::make_nvp("Radius", Radius));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("BOUNDING", cereal::base_class<BOUNDING>(this)));
		archive(cereal::make_nvp("Radius", Radius));
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_AABB)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_AABB)

#endif // !BOUNDING_AABB_H