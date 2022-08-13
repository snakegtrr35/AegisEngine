/**
 * @file Mesh_Dome.h
 * @brief メッシュドームヘッダーファイル
 */

#pragma once

#ifndef MESH_DOOM_H
#define	MESH_DOOM_H

#include "GameObject.h"
#include "renderer.h"

class TEXTURE;

class MESH_DOOM :public GameObject {
private:
	//! 半径
	float						Radius;
	//! 頂点数
	UINT						VertexNum;
	//! インデックス数
	UINT						IndexNum;
	//! 頂点バッファ
	aegis::uniquePtr<aegis::Buffer>		VertexBuffer;
	//! インデックスバッファ
	aegis::uniquePtr<aegis::Buffer>		IndexBuffer;
	//! テクスチャ
	std::unique_ptr<TEXTURE>	Texture;						

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	MESH_DOOM();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~MESH_DOOM() { Uninit(); }

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
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw_Shadow() override;

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP() override;

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

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
		archive(cereal::make_nvp("Texture", Texture));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
		archive(cereal::make_nvp("Texture", Texture));
	}
};

CEREAL_REGISTER_TYPE(MESH_DOOM)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, MESH_DOOM)

#endif // !MESH_DOOM_H