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

	OBJECT_TYPE_INFO(TEXTS)

private:
	aegis::string Text;	//!< 表示する文字列

	/**
	* @brief 描画関数
	* @details 実際に描画する関数
	*/
	void Text_Draw(const aegis::string& text);

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Text_Draw_DPP(const aegis::string& text);

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
	void Draw_DPP();

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
	* @brief 表示する文字列を設定する関数
	* @param text 設定する文字列
	* @details 表示する文字列を設定する関数
	*/
	void Edit(const aegis::string& text);

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("SPRITE", cereal::base_class<SPRITE>(this)));

		archive(cereal::make_nvp("Text", std::string(Text)));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("SPRITE", cereal::base_class<SPRITE>(this)));

		std::string s;
		archive(cereal::make_nvp("Text", s));
		Text.reserve(s.size());
		Text = s;
	}
};

CEREAL_REGISTER_TYPE(TEXTS)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, TEXTS)

#endif //! TEXT_H