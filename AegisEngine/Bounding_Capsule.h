/**
 * @file Bounding_Capsule.h
 * @brief CAPSULE�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef BOUNDING_CAPSULE_H
#define	BOUNDING_CAPSULE_H

#include	"Bounding.h"

// ��
class BOUNDING_CAPSULE : public BOUNDING {
private:
	//! ��ڂ̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! ��ڂ̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer2;

	//! �C���f�b�N�X�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;	

	//! �C���f�b�N�X��
	UINT IndexNum;

	//! �����O�̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_Ring;

	//! �����O�̃C���f�b�N�X�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer_Ring;

	//! �����O�̃C���f�b�N�X��
	UINT IndexNum_Ring;

	//! ���a
	float Radius;

	//! ����
	float Height;

	//! �`��(����)
	void Draw_Body(const XMFLOAT3& position, const XMFLOAT3& rotation);

	//! �{�̂̕`��
	void Draw_Semicircle(const XMFLOAT3& position, const XMFLOAT3& rotation);

	//! ���S�̉~�����̕`��
	void Draw_Ring(const XMFLOAT3& position, const XMFLOAT3& rotation);

	// !�{�̂̏�����
	void Init_Body();

	//! ���S�̉~�����̏�����
	void Init_Ring();

public:
	BOUNDING_CAPSULE() {}
	~BOUNDING_CAPSULE() { Uninit(); }

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

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
		ar(Height);
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_CAPSULE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_CAPSULE)

#endif // !BOUNDING_CAPSULE_H