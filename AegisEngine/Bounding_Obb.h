#pragma once

#ifndef BOUNDING_OBB_H
#define BOUNDING_OBB_H

#include	"Bounding.h"

// OBB
class BOUNDING_OBB : public BOUNDING {
private:

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		//! BOX�̒��_�o�b�t�@

	BoundingOrientedBox Obb;

	XMFLOAT3 Radius;

	XMFLOAT4 Quaternion;

public:
	BOUNDING_OBB() : Radius(XMFLOAT3(0.5f, 0.5f, 0.5f)), Quaternion(XMFLOAT4(0.f, 0.f, 0.f, 1.0f)) {}
	~BOUNDING_OBB();

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

	const BoundingOrientedBox& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
		ar(Quaternion);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//}
};

CEREAL_REGISTER_TYPE(BOUNDING_OBB)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_OBB)

#endif // !BOUNDING_OBB_H