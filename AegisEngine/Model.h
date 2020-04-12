#pragma once

#ifndef MODEL_H
#define MODEL_H

#include	"Component.h"
#include	"Mesh.h"

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

class MODEL : public COMPONENT {
private:

	//! �t�@�C����
	string FileName;

	//! �t�@�C�����̃n�b�V���l
	size_t Key;

public:

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	MODEL();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~MODEL() { Uninit(); };

	/**
	* @brief �������֐�
	* @details ����������֐�
	*/
	void Init() override;

	/**
	* @brief �`��֐�
	* @details �`�悷��֐�
	*/
	void Draw() override;

	/**
	* @brief �`��(Depth-pre-pass)�֐�
	* @details Depth-pre-pass������֐�
	*/
	void Draw_DPP() override;

	/**
	* @brief �X�V�֐�
	* @details �X�V����֐�
	*/
	void Update(float delta_time) override;

	/**
	* @brief �I�������֐�
	* @details �I������������֐�
	*/
	void Uninit() override;

	/**
	* @brief �C���X�y�N�^�[�ł̕`��֐�
	* @details �C���X�y�N�^�[�ł̕`�������֐�
	*/
	void Draw_Inspector() override;

	// ���f�����̐ݒ�
	void Set_Model_Name(const string& file_name);	

	// ���f�����̎擾
	const string& Get_Model_Name();

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