#pragma once

#ifndef TEXT_H
#define TEXT_H

#include	"Sprite.h"

/**
* @brief テキストクラス
* @details 文字列を扱うためのクラス
* @details スプライトクラスの派生クラス
*/
class TEXTS :public  SPRITE {
private:
	string Text;	//!< 表示する文字列

	/**
	* @brief 描画関数
	* @details 実際に描画する関数
	*/
	void Text_Draw(const string& text);

	/**
	* @brief 描画(Depth pre-pass)関数
	* @details Depth pre-passを行う関数
	*/
	void Text_Draw_DPP(const string& text);

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	TEXTS();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~TEXTS();

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
	* @brief 表示する文字列を設定する関数
	* @param text 設定する文字列
	* @details 表示する文字列を設定する関数
	*/
	void Edit(const string& text);

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<SPRITE>(this));
		ar(Text);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(Text);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(Text);
	//}
};

CEREAL_REGISTER_TYPE(TEXTS)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, TEXTS)

#endif //! TEXT_H