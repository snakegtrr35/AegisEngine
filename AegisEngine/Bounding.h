﻿/**
 * @file Bounding.h
 * @brief バウンディング関係のヘッダーファイル
 */

#pragma once

#ifndef BOUNDING_H
#define BOUNDING_H

#include "Component.h"
#include "Renderer.h"

 /**
 * @brief バウンディングクラス
 * @details 全バウンディングの基底クラス
 */
class BOUNDING : public COMPONENT {

	ABSTRACT_OBJECT_TYPE_INFO(COMPONENT, BOUNDING)

private:
protected:
	//! BOXのインデックスバッファ
	static aegis::uniquePtr<aegis::Buffer> IndexBufferBox;
	//! BOXのインデックス数
	static const char IndexNum_Box;
	//! デフォルトのカラー
	static aegis::COLOR Default_Color;
	//! 各バウンディングのカラー
	aegis::COLOR Color;

	aegis::Vector3 Position;
	aegis::Vector3 Rotation;
	aegis::Vector3 Scaling = aegis::Vector3(1.0f, 1.0f, 1.0f);

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
	static void Set_Default_Color(const aegis::COLOR& color) {
		Default_Color = color;
	}

	/**
	* @brief デフォルトカラーを取得する関数
	* @return COLOR デフォルトカラー
	* @details デフォルトカラーを取得する関数
	*/
	static aegis::COLOR Get_Default_Color() {
		return Default_Color;
	}

	/**
	* @brief カラーを設定する関数
	* @return COLOR カラー
	* @details 個別の色を設定する関数
	*/
	void Set_Color(const aegis::COLOR& color) {
		Color = color;
	}

	aegis::Vector3* const Get_Position() { return &Position; };
	aegis::Vector3* const Get_Rotation() { return &Rotation; };
	aegis::Vector3* const Get_Scaling() { return &Scaling; };

	void Set_Position(aegis::Vector3* const position) { Position = *position; }
	void Set_Position(const aegis::Vector3& position) { Position = position; };

	void Set_Rotation(aegis::Vector3* const rotation) { Rotation = *rotation; };
	void Set_Rotation(const aegis::Vector3& rotation) { Rotation = rotation; };

	void Set_Scaling(aegis::Vector3* scaling) { Scaling = *scaling; };
	void Set_Scaling(const aegis::Vector3& scaling) { Scaling = scaling; };

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("COMPONENT", cereal::base_class<COMPONENT>(this)));

		archive(cereal::make_nvp("Default_Color", Default_Color));
		archive(cereal::make_nvp("Color", Color));
		archive(cereal::make_nvp("Position", Position));
		archive(cereal::make_nvp("Rotation", Rotation));
		archive(cereal::make_nvp("Scaling", Scaling));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("COMPONENT", cereal::base_class<COMPONENT>(this)));

		archive(cereal::make_nvp("Default_Color", Default_Color));
		archive(cereal::make_nvp("Color", Color));
		archive(cereal::make_nvp("Position", Position));
		archive(cereal::make_nvp("Rotation", Rotation));
		archive(cereal::make_nvp("Scaling", Scaling));
	}
};

CEREAL_REGISTER_TYPE(BOUNDING)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT, BOUNDING)

#endif // !BOUNDING_H