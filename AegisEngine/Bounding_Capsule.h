#pragma once

#ifndef BOUNDING_CAPSULE_H
#define	BOUNDING_CAPSULE_H

#include	"Bounding.h"

// ��
class BOUNDING_CAPSULE : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer2;		// ��ڂ̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	UINT IndexNum;

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_Ring;		// �����O�̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer_Ring;		// �����O�̃C���f�b�N�X�o�b�t�@
	UINT IndexNum_Ring;

	float Radius;
	float Height;

	void Draw_Body(const XMFLOAT3& position, const XMFLOAT3& rotation);
	void Draw_Semicircle(const XMFLOAT3& position, const XMFLOAT3& rotation);
	void Draw_Ring(const XMFLOAT3& position, const XMFLOAT3& rotation);

	void Init_Body();
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
	void OverWrite() override {};

	void Set_Radius(const float radius);

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