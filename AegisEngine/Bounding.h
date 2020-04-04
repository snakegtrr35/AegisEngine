/**
 * @file Bounding.h
 * @brief バウンディング関係のヘッダーファイル
 */

#pragma once

#ifndef BOUNDING_H
#define BOUNDING_H

#include	"Renderer.h"
#include	"Component.h"

 /**
 * @brief バウンディングクラス
 * @details 全バウンディングの基底クラス
 */
class BOUNDING : public COMPONENT {
private:
protected:
	//! BOXのインデックスバッファ
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer_BOX;
	//! BOXのインデックス数
	static const char IndexNum_Box;
	//! デフォルトのカラー
	static COLOR Default_Color;
	//! 各バウンディングのカラー
	COLOR Color;

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BOUNDING() : Color(Default_Color) {}

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	virtual ~BOUNDING() {}

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init() override {}

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw() override {}

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP() override {}

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time) override {}

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit() override {}

	/**
	* @brief インスペクターでの描画関数
	* @details インスペクターでの描画をする関数
	*/
	void Draw_Inspector() override {};

	/**
	* @brief 頂点バッファの更新関数
	* @details 頂点バッファの更新をする関数
	*/
	virtual void OverWrite() = 0;

	/**
	* @brief デフォルトカラーを設定する関数
	* @param COLOR カラー
	* @details デフォルトカラーを設定する関数
	*/
	static void Set_Default_Color(const COLOR& color) {
		Default_Color = color;
	}

	/**
	* @brief デフォルトカラーを取得する関数
	* @return COLOR デフォルトカラー
	* @details デフォルトカラーを取得する関数
	*/
	static COLOR Get_Default_Color() {
		return Default_Color;
	}

	/**
	* @brief カラーを設定する関数
	* @return COLOR カラー
	* @details 個別の色を設定する関数
	*/
	void Set_Color(const COLOR& color) {
		Color = color;
	}


	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<COMPONENT>(this));
		ar(Default_Color);
		ar(Color);
	}
};

CEREAL_REGISTER_TYPE(BOUNDING)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT, BOUNDING)

#endif // !BOUNDING_H