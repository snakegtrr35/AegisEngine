#pragma once

#ifndef BOUNDING_SFRUSTUM_H
#define BOUNDING_SFRUSTUM_H

#include	"Bounding.h"

// ��
class BOUNDING_FRUSTUM : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// �C���f�b�N�X�o�b�t�@

	BoundingFrustum Frustum;

public:
	BOUNDING_FRUSTUM() {}
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