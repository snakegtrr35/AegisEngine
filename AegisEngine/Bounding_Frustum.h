/**
 * @file Bounding_Frustum.h
 * @brief FRUSTUM�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef BOUNDING_SFRUSTUM_H
#define BOUNDING_SFRUSTUM_H

#include	"Bounding.h"

// ��
class BOUNDING_FRUSTUM : public BOUNDING {
private:
	//! ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! �C���f�b�N�X�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! ������
	BoundingFrustum Frustum;

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	BOUNDING_FRUSTUM() {}

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~BOUNDING_FRUSTUM() { Uninit(); }

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
	* @brief �`��(Depth pre-pass)�֐�
	* @details Depth pre-pass���s���֐�
	*/
	void Draw_DPP() override {}

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
	void Draw_Inspector() override {};

	/**
	* @brief ���_�o�b�t�@�̍X�V�֐�
	* @details ���_�o�b�t�@�̍X�V������֐�
	*/
	void OverWrite() override;

	/**
	* @brief �o�E���f�B���O��������擾����֐�
	* @return BoundingFrustum& �o�E���f�B���O������(�����蔻���p)
	* @details �o�E���f�B���O������(�����蔻���p)���擾����֐�
	*/
	const BoundingFrustum& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_FRUSTUM)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_FRUSTUM)

#endif // !BOUNDING_FRUSTUM_H