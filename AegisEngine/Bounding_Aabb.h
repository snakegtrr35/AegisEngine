/**
 * @file Bounding_Aabb.h
 * @brief AABB�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef BOUNDING_AABB_H
#define	BOUNDING_AABB_H

#include	"Bounding.h"

 /**
 * @brief �o�E���f�B���OAABB�N���X
 * @details �S�o�E���f�B���O�̊��N���X
 * @note BOUNDING�N���X�}���p�����Ă��܂�
 */
class BOUNDING_AABB : public BOUNDING {
private:
	//! BOX�̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;
	//! �e�ӂ̒���
	XMFLOAT3 Radius;

	BoundingBox Aabb;

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	BOUNDING_AABB() : Radius(XMFLOAT3(0.5f, 0.5f, 0.5f)) {}

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~BOUNDING_AABB();

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
	* @brief �e�ӂ̒�����ݒ肷��֐�
	* @param XMFLOAT3& ����
	*/
	void Set_Radius(const XMFLOAT3& radius);

	/**
	* @brief �e�ӂ̒�����ݒ肷��֐�
	* @param XMFLOAT3* ����
	*/
	void Set_Radius(const XMFLOAT3* radius);

	/**
	* @brief �e�ӂ̒������擾����֐�
	* @return XMFLOAT3* ����
	* @details �e�ӂ̒������擾����֐�
	*/
	XMFLOAT3* Get_Radius();

	/**
	* @brief �o�E���f�B���OAABB(�����蔻���p)���擾����֐�
	* @return BoundingBox �o�E���f�B���OAABB(�����蔻���p)
	* @details �o�E���f�B���OAABB(�����蔻���p)���擾����֐�
	*/
	const BoundingBox& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
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

CEREAL_REGISTER_TYPE(BOUNDING_AABB)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_AABB)

#endif // !BOUNDING_AABB_H