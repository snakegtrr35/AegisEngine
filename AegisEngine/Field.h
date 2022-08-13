﻿/**
 * @file Field.h
 * @brief フィールドのヘッダーファイル
 */

#pragma once

#ifndef FIELD_H
#define FIELD_H

#include "GameObject.h"
#include "Renderer.h"

class TEXTURE;

/**
* @brief フィールドクラス
* @details フィールドクラス
*/
class FIELD : public GameObject {

	OBJECT_TYPE_INFO(GameObject, FIELD)

private:
	//! 頂点バッファ
	aegis::uniquePtr<aegis::Buffer> VertexBuffer;

	//! インスタンシング用のバッファ
	aegis::uniquePtr<aegis::Buffer> InstanceBuffer;

	//! インデックスバッファ
	aegis::uniquePtr<aegis::Buffer> IndexBuffer;

	//! テクスチャ
	std::unique_ptr<TEXTURE> Texture;

	//! 幅と高さ
	aegis::Vector2 WH;

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	FIELD();
	
	/**
	* @brief コンストラクタ
	* @param position 座標
	* @param wh サイズ
	* @details 引数無しコンストラクタ
	*/
	FIELD(aegis::Vector3 position, aegis::Vector2 wh);

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~FIELD();

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
	* @brief 描画(Shadow-pass)関数
	* @details Shadow-passをする関数
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


	/**
	* @brief 座標の設定関数
	* @param position 座標
	* @details 座標の設定をする関数
	*/
	void SetPosition(aegis::Vector3& position);

	/**
	* @brief サイズの設定関数
	* @param wh サイズ
	* @details サイズの設定をする関数
	*/
	void SetWH(const aegis::Vector2 wh);

	/**
	* @brief テクスチャの設定関数
	* @param file_name テクスチャの設定(テクスチャの名前)
	* @details テクスチャの設定をする関数
	*/
	void SetTexture(const aegis::string& const file_name);

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

CEREAL_REGISTER_TYPE(FIELD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, FIELD)

#endif // !FIELD_H