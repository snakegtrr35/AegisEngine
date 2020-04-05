/**
 * @file Mesh_Dome.h
 * @brief ���b�V���h�[���w�b�_�[�t�@�C��
 */

#pragma once

#ifndef MESH_DOOM_H
#define	MESH_DOOM_H

class GAME_OBJECT;
class TEXTURE;

class MESH_DOOM :public GAME_OBJECT {
private:

	//! ���a
	float								Radius;

	//! ���_��
	UINT	VertexNum;

	//! �C���f�b�N�X��
	UINT								IndexNum;

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
	MESH_DOOM();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~MESH_DOOM() { Uninit(); }

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

CEREAL_REGISTER_TYPE(MESH_DOOM)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, MESH_DOOM)

#endif // !MESH_DOOM_H