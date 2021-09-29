/**
 * @file Fade.h
 * @brief フェイド関係のヘッダーファイル
 */

#pragma once

#ifndef FADE_H
#define FADE_H

#include	"Sprite.h"

 /**
 * @brief フェイドクラス
 * @details フェイドクラス
 */
class FADE : public SPRITE {
private:
	
	//! フェイドが終了したかを判定するフラグ(使用していない時はfalse、フェイドが終了したらtrue)
	static bool FadeEnable;

	//! ファイドの色
	XMFLOAT4 Color;

	//! サイズ
	XMFLOAT2 WH;

	//! ファイドの長さ(フレーム単位)
	WORD Time;

	//! 生成時からのカウント
	WORD Cnt;

	//! ファイドの透明度
	float AdditionalAlpha;

	/**
	* @brief ファイドが終了するかを判定をする関数
	* @return bool 判定フラグ(終了するならtrue)
	* @details ファイドが終了するかのフラグを取得する関数
	*/
	bool Get_FadeEnable();

	/**
	* @brief 座標を設定する関数
	* @param position 座標(二次元座標)
	* @details 座標(二次元座標)を設定する関数
	*/
	void Set_AdditionalAlpha(const WORD time, const float sign);

public: 

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	FADE();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~FADE();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init();

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw();

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP() {};

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time);

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit();

	/**
	* @brief フェイドの時間を設定する関数
	* @param WORD 時間(フレーム単位)
	* @details フェイドにかかる時間を設定する関数
	*/
	void Set_Time(const WORD time);

	/**
	* @brief 色を設定する関数
	* @param XMFLOAT4 色
	* @details フェイドの色を設定する関数
	*/
	void Set_Color(const XMFLOAT4& color);

	/**
	* @brief フェイドアウトを開始する関数
	* @param time フェイドにかかる時間(フレーム単位)
	* @details フェイドアウトを開始する関数
	*/
	static void Start_FadeOut(const WORD time);

	/**
	* @brief フェイドインを開始する関数
	* @param time フェイドにかかる時間(フレーム単位)
	* @details フェイドインを開始する関数
	*/
	static void Start_FadeIn(const WORD time);

	/**
	* @brief フェイドが終了したかを取得する関数
	* @return bool フェイドが終了したかを判定するフラグ(フェイドが終了ならtrue)
	* @details フェイドが終了したかを判定するフラグを取得する関数
	*/
	static bool End_Fade();

	///**
	//* @brief 座標を設定する関数
	//* @param position 座標(二次元座標)
	//* @details 座標(二次元座標)を設定する関数
	//*/
	//static void Set_Enable(const bool flag);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(FADE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, FADE)

#endif // !FADE_H