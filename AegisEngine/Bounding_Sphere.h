#pragma once

#ifndef BOUNDING_SHPERE_H
#define BOUNDING_SHPERE_H

#include	"Bounding.h"

// 球
class BOUNDING_SHPERE : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// インデックスバッファ

	float Radius;
	UINT IndexNum;

	BoundingSphere Sphere;

	void Draw_Ring(const XMFLOAT3& rotation);

public:
	BOUNDING_SHPERE() : Radius(1.0f), IndexNum(0) {}
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
	void Draw_Inspector() override;

	/**
	* @brief 頂点バッファの更新関数
	* @details 頂点バッファの更新をする関数
	*/
	void OverWrite() override;

	void Set_Radius(const float radius);

	const float Get_Radius();

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