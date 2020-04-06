/**
 * @file Field.h
 * @brief フィールドのヘッダーファイル
 */

#pragma once

#ifndef FIELD_H
#define FIELD_H

#include	"texture.h"
#include	"Renderer.h"
#include	"main.h"

class GAME_OBJECT;

/**
* @brief フィールドクラス
* @details フィールドクラス
*/
class FIELD : public GAME_OBJECT {

	//! 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! インスタンシング用のバッファ
	unique_ptr<ID3D11Buffer, Release> pInstanceBuffer;

	//! インデックスバッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! テクスチャ
	unique_ptr<TEXTURE> Texture;

	//! 幅と高さ
	XMFLOAT2 WH;

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
	FIELD(XMFLOAT3 position, XMFLOAT2 wh);

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
	void SetPosition(const XMFLOAT3 position);

	/**
	* @brief サイズの設定関数
	* @param wh サイズ
	* @details サイズの設定をする関数
	*/
	void SetWH(const XMFLOAT2 wh);

	/**
	* @brief テクスチャの設定関数
	* @param file_name テクスチャの設定(テクスチャの名前)
	* @details テクスチャの設定をする関数
	*/
	void SetTexture(const string& const file_name);

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Texture);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}*/
};

CEREAL_REGISTER_TYPE(FIELD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, FIELD)

#endif // !FIELD_H