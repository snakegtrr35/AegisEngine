/**
 * @file Bounding.h
 * @brief �o�E���f�B���O�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef BOUNDING_H
#define BOUNDING_H

#include	"Renderer.h"
#include	"Component.h"

 /**
 * @brief �o�E���f�B���O�N���X
 * @details �S�o�E���f�B���O�̊��N���X
 */
class BOUNDING : public COMPONENT {
private:
protected:
	//! BOX�̃C���f�b�N�X�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer_BOX;
	//! BOX�̃C���f�b�N�X��
	static const char IndexNum_Box;
	//! �f�t�H���g�̃J���[
	static COLOR Default_Color;
	//! �e�o�E���f�B���O�̃J���[
	COLOR Color;

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	BOUNDING() : Color(Default_Color) {}

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	virtual ~BOUNDING() {}

	/**
	* @brief �������֐�
	* @details ����������֐�
	*/
	void Init() override {}

	/**
	* @brief �`��֐�
	* @details �`�悷��֐�
	*/
	void Draw() override {}

	/**
	* @brief �`��(Depth-pre-pass)�֐�
	* @details Depth-pre-pass������֐�
	*/
	void Draw_DPP() override {}

	/**
	* @brief �X�V�֐�
	* @details �X�V����֐�
	*/
	void Update(float delta_time) override {}

	/**
	* @brief �I�������֐�
	* @details �I������������֐�
	*/
	void Uninit() override {}

	/**
	* @brief �C���X�y�N�^�[�ł̕`��֐�
	* @details �C���X�y�N�^�[�ł̕`�������֐�
	*/
	void Draw_Inspector() override {};

	/**
	* @brief ���_�o�b�t�@�̍X�V�֐�
	* @details ���_�o�b�t�@�̍X�V������֐�
	*/
	virtual void OverWrite() = 0;

	/**
	* @brief �f�t�H���g�J���[��ݒ肷��֐�
	* @param COLOR �J���[
	* @details �f�t�H���g�J���[��ݒ肷��֐�
	*/
	static void Set_Default_Color(const COLOR& color) {
		Default_Color = color;
	}

	/**
	* @brief �f�t�H���g�J���[���擾����֐�
	* @return COLOR �f�t�H���g�J���[
	* @details �f�t�H���g�J���[���擾����֐�
	*/
	static COLOR Get_Default_Color() {
		return Default_Color;
	}

	/**
	* @brief �J���[��ݒ肷��֐�
	* @return COLOR �J���[
	* @details �ʂ̐F��ݒ肷��֐�
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