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

	OBJECT_TYPE_INFO(MODEL)

private:

	//! ファイル名
	aegis::string FileName;

	//! ファイル名のハッシュ値
	size_t Key;

	aegis::Vector3 Position;
	aegis::Vector3 Rotation;
	aegis::Vector3 Scaling;

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
	void Set_Model_Name(const aegis::string& file_name);	

	// モデル名の取得
	const aegis::string& Get_Model_Name();

	aegis::Vector3* const GetPosition() { return &Position; };
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

		archive(cereal::make_nvp("FileName", std::string(FileName)));

		archive(cereal::make_nvp("Key", Key));
		archive(cereal::make_nvp("Position", Position));
		archive(cereal::make_nvp("Rotation", Rotation));
		archive(cereal::make_nvp("Scaling", Scaling));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("COMPONENT", cereal::base_class<COMPONENT>(this)));
		
		std::string s;
		archive(cereal::make_nvp("FileName", s));
		FileName.reserve(s.size());
		FileName = s;

		archive(cereal::make_nvp("Key", Key));
		archive(cereal::make_nvp("Position", Position));
		archive(cereal::make_nvp("Rotation", Rotation));
		archive(cereal::make_nvp("Scaling", Scaling));
	}

};

CEREAL_REGISTER_TYPE(MODEL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT, MODEL)

#endif // !MODEL_H