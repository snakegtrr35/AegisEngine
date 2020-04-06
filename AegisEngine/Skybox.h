#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H


class GAME_OBJECT;
//class TEXTURE;

class SKYBOX : public GAME_OBJECT {
private:
	//! ���_�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release>	VertexBuffer;

	//! �C���f�b�N�X�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release>	IndexBuffer;

	////! �e�N�X�`��
	//unique_ptr<TEXTURE>					Texture;

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	SKYBOX();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~SKYBOX();

	/**
	* @brief �������֐�
	* @details ����������֐�
	*/
	void Init();

	/**
	* @brief �`��֐�
	* @details �`�悷��֐�
	*/
	void Draw();

	/**
	* @brief �`��(Depth-pre-pass)�֐�
	* @details Depth-pre-pass������֐�
	*/
	void Draw_DPP();

	/**
	* @brief �X�V�֐�
	* @details �X�V����֐�
	*/
	void Update(float delta_time);

	/**
	* @brief �I�������֐�
	* @details �I������������֐�
	*/
	void Uninit();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		//ar(Texture);
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

CEREAL_REGISTER_TYPE(SKYBOX)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, SKYBOX)

#endif // !SKYBOX_H