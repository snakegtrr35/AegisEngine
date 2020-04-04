/**
 * @file Field.h
 * @brief �t�B�[���h�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef FIELD_H
#define FIELD_H

#include	"texture.h"
#include	"Renderer.h"
#include	"main.h"

class GAME_OBJECT;

/**
* @brief �t�B�[���h�N���X
* @details �t�B�[���h�N���X
*/
class FIELD : public GAME_OBJECT {

	//! ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! �C���X�^���V���O�p�̃o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pInstanceBuffer;

	//! �C���f�b�N�X�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! �e�N�X�`��
	unique_ptr<TEXTURE> Texture;

	//! ���ƍ���
	XMFLOAT2 WH;

public:

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	FIELD();
	
	/**
	* @brief �R���X�g���N�^
	* @param position ���W
	* @param wh �T�C�Y
	* @details ���������R���X�g���N�^
	*/
	FIELD(XMFLOAT3 position, XMFLOAT2 wh);

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~FIELD();

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
	* @brief ���W�̐ݒ�֐�
	* @param position ���W
	* @details ���W�̐ݒ������֐�
	*/
	void SetPosition(const XMFLOAT3 position);

	/**
	* @brief �T�C�Y�̐ݒ�֐�
	* @param wh �T�C�Y
	* @details �T�C�Y�̐ݒ������֐�
	*/
	void SetWH(const XMFLOAT2 wh);

	/**
	* @brief �e�N�X�`���̐ݒ�֐�
	* @param file_name �e�N�X�`���̐ݒ�(�e�N�X�`���̖��O)
	* @details �e�N�X�`���̐ݒ������֐�
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