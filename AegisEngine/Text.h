#pragma once

#ifndef TEXT_H
#define TEXT_H

#include	"Sprite.h"

/**
* @brief �e�L�X�g�N���X
* @details ��������������߂̃N���X
* @details �X�v���C�g�N���X�̔h���N���X
*/
class TEXTS :public  SPRITE {
private:
	string Text;	//!< �\�����镶����

	/**
	* @brief �`��֐�
	* @details ���ۂɕ`�悷��֐�
	*/
	void Text_Draw(const string& text);

	/**
	* @brief �`��(Depth pre-pass)�֐�
	* @details Depth pre-pass���s���֐�
	*/
	void Text_Draw_DPP(const string& text);

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	TEXTS();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~TEXTS();

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
	* @brief �\�����镶�����ݒ肷��֐�
	* @param text �ݒ肷�镶����
	* @details �\�����镶�����ݒ肷��֐�
	*/
	void Edit(const string& text);

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<SPRITE>(this));
		ar(Text);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(Text);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(Text);
	//}
};

CEREAL_REGISTER_TYPE(TEXTS)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, TEXTS)

#endif //! TEXT_H