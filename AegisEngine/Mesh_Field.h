/**
 * @file Mesh_Field.h
 * @brief ���b�V���t�B�[���h�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef MESH_FIELF_H
#define MESH_FIELF_H

#include	"Renderer.h"

class GAME_OBJECT;
class TEXTURE;

//==============================
//  �t�B�[���h�N���X
//==============================
class MESH_FIELD : public GAME_OBJECT {
private:

	//! �O���b�h�T�C�Y
	XMFLOAT3							GridSize;

	//! �O���b�h��
	XMINT2								GridNum;

	//! �C���f�b�N�X��
	UINT								IndexNum;

	// ���_�f�[�^
	vector<VERTEX_3D>					VertexArray;

	//! ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release>	VertexBuffer;

	//! �C���f�b�N�X�o�b�t�@
	unique_ptr<ID3D11Buffer, Release>	IndexBuffer;

	//! �e�N�X�`��
	unique_ptr<TEXTURE>					Texture;

public:

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	MESH_FIELD();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~MESH_FIELD() { Uninit(); }

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
	* @brief ���b�V���t�B�[���h�̍������擾����֐�
	* @param position ���b�V���t�B�[���h�̍��W
	* @return float ���b�V���t�B�[���h�̍���
	* @details �����̍��W�̃��b�V���t�B�[���h�̍������擾����֐�
	*/
	const float Get_Height(const XMFLOAT3& position);

	/**
	* @brief �e�N�X�`����ݒ肷��֐�
	* @param file_name �g�p����e�N�X�`����
	* @details �e�N�X�`��(�N���X)��ݒ肷��֐�
	*/
	void SetTexture(const string& const file_name);

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

CEREAL_REGISTER_TYPE(MESH_FIELD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, MESH_FIELD)

//==============================
//  �E�H�[���N���X
//==============================
class MESH_WALL :public GAME_OBJECT {
private:
	XMFLOAT3		GridSize;
	XMINT2			GridNum;
	unsigned int	IndexNum;
	VERTEX_3D* VertexArray;

	ID3D11Buffer* VertexBuffer;		// ���_�o�b�t�@
	ID3D11Buffer* IndexBuffer;		// �C���f�b�N�X�o�b�t�@
	TEXTURE* Texture;				// �e�N�X�`��

public:
	MESH_WALL();
	~MESH_WALL() { Uninit(); };

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	const float Get_Height(const XMFLOAT3& position);
};

#endif // !MESH_FIELF_H