/**
* @file SPrite_Animation.h
* @brief スプライトアニメーションのクラスがあるヘッダーファイル
*/

#pragma once

#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include	"Sprite.h"

/**
* @brief スプライトアニメーションクラス
* @details スプライトでアニメーションができるクラス
*/
class SPRITE_ANIMATION : public SPRITE {

	OBJECT_TYPE_INFO(SPRITE_ANIMATION)

private:
	float WaitFrame;
	WORD CreatCount;
	WORD PatternCount;
	WORD Age;

	float FrameCount;

	float Tx, Ty;	//!< テクスチャ切り取り座標
	float Tw, Th;	//!< テクスチャの切り取り幅と高さ

	float Tx_Param, Ty_Param;	//!< テクスチャ切り取り座標（手動）

	unsigned char  Pattern_Max_X;	//!< テクスチャの分割数(横)
	unsigned char  Pattern_Max_Y;	//!< テクスチャの分割数(縦)

	/**
	* @brief 描画関数
	* @details 実際に描画する関数
	*/
	void Draw2(float tx = -1.0f, float ty = -1.0f);

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP2(float tx = -1.0f, float ty = -1.0f);

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	SPRITE_ANIMATION();

	/**
	* @brief コンストラクタ
	* @param  position 中心座標(二次元座標)
	* @param size サイズ
	* @details 引数付きコンストラクタ
	*/
	SPRITE_ANIMATION(aegis::Vector2 position, aegis::Vector4 size);

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~SPRITE_ANIMATION();

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
	* @brief テクスチャアニメーションのパラメーターの設定する関数
	* @param wait_frame 待ちフレーム
	* @param x テクスチャ切り取り幅
	* @param y テクスチャ切り取り高さ
	* @details テクスチャアニメーションのパラメーターの設定する関数
	*/
	void SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y);

	/**
	* @brief テクスチャアニメーションのテクスチャ切り取り座標を設定する関数
	* @param tx_param テクスチャ切り取り幅（手動）
	* @param ty_param テクスチャ切り取り高さ（手動）
	* @details テクスチャアニメーションのテクスチャ切り取り座標を設定する関数
	*/
	void Set_Param_Txy(const float& tx_param = -1.0f, const float& ty_param = -1.0f);

	/**
	* @brief テクスチャの切り取り幅と高さを取得する関数
	* @return aegis::Vector2 テクスチャの切り取り幅と高さ
	* @details テクスチャの切り取り幅と高さを取得する関数
	*/
	const aegis::Vector2 Get_Twh() {
		return aegis::Vector2(Tw, Th);
	}

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("SPRITE", cereal::base_class<SPRITE>(this)));

		archive(cereal::make_nvp("WaitFrame", WaitFrame));
		archive(cereal::make_nvp("Pattern_Max_X", Pattern_Max_X));
		archive(cereal::make_nvp("Pattern_Max_Y", Pattern_Max_Y));
		archive(cereal::make_nvp("Tx", Tx));
		archive(cereal::make_nvp("Ty", Ty));
		archive(cereal::make_nvp("Tw", Tw));
		archive(cereal::make_nvp("Th", Th));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("SPRITE", cereal::base_class<SPRITE>(this)));

		archive(cereal::make_nvp("WaitFrame", WaitFrame));
		archive(cereal::make_nvp("Pattern_Max_X", Pattern_Max_X));
		archive(cereal::make_nvp("Pattern_Max_Y", Pattern_Max_Y));
		archive(cereal::make_nvp("Tx", Tx));
		archive(cereal::make_nvp("Ty", Ty));
		archive(cereal::make_nvp("Tw", Tw));
		archive(cereal::make_nvp("Th", Th));
	}
};

CEREAL_REGISTER_TYPE(SPRITE_ANIMATION)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, SPRITE_ANIMATION)

#endif // !SPRITE_ANIMATION_H