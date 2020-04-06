/**
 * @file Mesh_Cylinder.h
 * @brief ���b�V���V�����_�[�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef MESH_CYLINDER_H
#define MESH_CYLINDER_H

#include	"texture.h"

class GAME_OBJECT;

class MESH_CYlLINDER : public GAME_OBJECT {
private:
	int				CylinderLength;
	float			Radius;
	unsigned int	VertexNum;
	unsigned int	IndexNum;

	unique_ptr<ID3D11Buffer, Release>	VertexBuffer;	// ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release>	IndexBuffer;	// �C���f�b�N�X�o�b�t�@
	unique_ptr<TEXTURE> Texture;						// �e�N�X�`��

public:

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	MESH_CYlLINDER() : Radius(45.0f), CylinderLength(7) {}

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~MESH_CYlLINDER() {}

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

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Texture);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Texture);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Texture);
	//}
};

CEREAL_REGISTER_TYPE(MESH_CYlLINDER)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, MESH_CYlLINDER)

#endif // !MESH_CYLINDER_H