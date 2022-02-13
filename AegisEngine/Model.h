#pragma once

#ifndef MODEL_H
#define MODEL_H

#include	"Component.h"
#include	"Mesh.h"

#undef min
#undef max

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

class MODEL : public COMPONENT {
private:

	//! ファイル名
	std::string FileName;

	//! ファイル名のハッシュ値
	size_t Key;

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	MODEL();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~MODEL() { Uninit(); };

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
	* @brief インスペクターでの描画関数
	* @details インスペクターでの描画をする関数
	*/
	void Draw_Inspector() override;

	// モデル名の設定
	void Set_Model_Name(const std::string& file_name);	

	// モデル名の取得
	const std::string& Get_Model_Name();

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<COMPONENT>(this));
		ar(FileName);
		ar(Key);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<COMPONENT>(this));
		ar(FileName);
		ar(Key);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<COMPONENT>(this));
		ar(FileName);
		ar(Key);
	}*/

};

CEREAL_REGISTER_TYPE(MODEL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT, MODEL)

#endif // !MODEL_H