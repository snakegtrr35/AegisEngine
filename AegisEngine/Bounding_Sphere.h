/**
 * @file Bounding_Sphere.h
 * @brief SHPERE�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef BOUNDING_SHPERE_H
#define BOUNDING_SHPERE_H

#include	"Bounding.h"

// ��
class BOUNDING_SHPERE : public BOUNDING {
private:
	//! ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! �C���f�b�N�X�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! ���a
	float Radius;

	//! �C���f�b�N�X��
	UINT IndexNum;

	//! SHPERE
	BoundingSphere Sphere;

	//! ���ۂ̕`��
	void Draw_Ring(const XMFLOAT3& rotation);

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	BOUNDING_SHPERE() : Radius(1.0f), IndexNum(0) {}

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~BOUNDING_SHPERE() { Uninit(); }

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
	void Draw_Inspector() override;

	/**
	* @brief ���_�o�b�t�@�̍X�V�֐�
	* @details ���_�o�b�t�@�̍X�V������֐�
	*/
	void OverWrite() override;

	/**
	* @brief ���a��ݒ肷��֐�
	* @param radius(���a) ���a
	*/
	void Set_Radius(const float radius);

	/**
	* @brief ���a���擾����֐�
	* @return float ���a
	* @details ���a���擾����֐�
	*/
	const float Get_Radius();

	/**
	* @brief �o�E���f�B���O�����擾����֐�
	* @return BoundingSphere& �o�E���f�B���O��(�����蔻���p)
	* @details �o�E���f�B���O��(�����蔻���p)���擾����֐�
	*/
	const BoundingSphere& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_SHPERE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_SHPERE)

#endif // !BOUNDING_SHPERE_H