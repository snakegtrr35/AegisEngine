/**
 * @file Fade.h
 * @brief �t�F�C�h�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef FADE_H
#define FADE_H

#include	"Sprite.h"

 /**
 * @brief �t�F�C�h�N���X
 * @details �t�F�C�h�N���X
 */
class FADE : public SPRITE {
private:
	
	//! �t�F�C�h���I���������𔻒肷��t���O(�g�p���Ă��Ȃ�����false�A�t�F�C�h���I��������true)
	static bool FadeEnable;

	//! �t�@�C�h�̐F
	XMFLOAT4 Color;

	//! �T�C�Y
	XMFLOAT2 WH;

	//! �t�@�C�h�̒���(�t���[���P��)
	WORD Time;

	//! ����������̃J�E���g
	WORD Cnt;

	//! �t�@�C�h�̓����x
	float AdditionalAlpha;

	/**
	* @brief �t�@�C�h���I�����邩�𔻒������֐�
	* @return bool ����t���O(�I������Ȃ�true)
	* @details �t�@�C�h���I�����邩�̃t���O���擾����֐�
	*/
	bool Get_FadeEnable();

	/**
	* @brief ���W��ݒ肷��֐�
	* @param position ���W(�񎟌����W)
	* @details ���W(�񎟌����W)��ݒ肷��֐�
	*/
	void Set_AdditionalAlpha(const WORD time, const float sign);

public: 

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	FADE();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~FADE();

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
	void Draw_DPP() {};

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

	/**
	* @brief �t�F�C�h�̎��Ԃ�ݒ肷��֐�
	* @param WORD ����(�t���[���P��)
	* @details �t�F�C�h�ɂ����鎞�Ԃ�ݒ肷��֐�
	*/
	void Set_Time(const WORD time);

	/**
	* @brief �F��ݒ肷��֐�
	* @param XMFLOAT4 �F
	* @details �t�F�C�h�̐F��ݒ肷��֐�
	*/
	void Set_Color(const XMFLOAT4& color);

	/**
	* @brief �t�F�C�h�A�E�g���J�n����֐�
	* @param time �t�F�C�h�ɂ����鎞��(�t���[���P��)
	* @details �t�F�C�h�A�E�g���J�n����֐�
	*/
	static void Start_FadeOut(const WORD time);

	/**
	* @brief �t�F�C�h�C�����J�n����֐�
	* @param time �t�F�C�h�ɂ����鎞��(�t���[���P��)
	* @details �t�F�C�h�C�����J�n����֐�
	*/
	static void Start_FadeIn(const WORD time);

	/**
	* @brief �t�F�C�h���I�����������擾����֐�
	* @return bool �t�F�C�h���I���������𔻒肷��t���O(�t�F�C�h���I���Ȃ�true)
	* @details �t�F�C�h���I���������𔻒肷��t���O���擾����֐�
	*/
	static bool End_Fade();

	///**
	//* @brief ���W��ݒ肷��֐�
	//* @param position ���W(�񎟌����W)
	//* @details ���W(�񎟌����W)��ݒ肷��֐�
	//*/
	//static void Set_Enable(const bool flag);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(FADE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, FADE)

#endif // !FADE_H