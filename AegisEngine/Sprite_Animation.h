/**
* @file SPrite_Animation.h
* @brief �X�v���C�g�A�j���[�V�����̃N���X������w�b�_�[�t�@�C��
*/

#pragma once

#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include	"Sprite.h"

/**
* @brief �X�v���C�g�A�j���[�V�����N���X
* @details �X�v���C�g�ŃA�j���[�V�������ł���N���X
*/
class SPRITE_ANIMATION : public SPRITE {
private:
	WORD WaitFrame;
	WORD CreatCount;
	WORD PatternCount;
	WORD Age;

	float FrameCount;

	float Tx, Ty;					//!< �e�N�X�`���؂�����W
	float Tw, Th;					//!< �e�N�X�`���̐؂��蕝�ƍ���

	float Tx_Param, Ty_Param;		//!< �e�N�X�`���؂�����W�i�蓮�j

	unsigned char  Pattern_Max_X;	//!< �e�N�X�`���̕�����(��)
	unsigned char  Pattern_Max_Y;	//!< �e�N�X�`���̕�����(�c)

	bool Loop_Enable;				//!< �A�j���[�V���������[�v���邩�̃t���O

	/**
	* @brief �`��֐�
	* @details ���ۂɕ`�悷��֐�
	*/
	void Draw2(float tx = -1.0f, float ty = -1.0f);

	/**
	* @brief �`��(Depth pre-pass)�֐�
	* @details ���ۂ�Depth pre-pass���s���֐�
	*/
	void Draw_DPP2(float tx = -1.0f, float ty = -1.0f);

public:

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	SPRITE_ANIMATION();

	/**
	* @brief �R���X�g���N�^
	* @param  position ���S���W(�񎟌����W)
	* @param size �T�C�Y
	* @details �����t���R���X�g���N�^
	*/
	SPRITE_ANIMATION(XMFLOAT2 position, XMFLOAT4 size);

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~SPRITE_ANIMATION();

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
	* @brief �`��(Depth pre-pass)�֐�
	* @details Depth pre-pass���s���֐�
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

	/**
	* @brief �e�N�X�`���A�j���[�V�����̃p�����[�^�[�̐ݒ肷��֐�
	* @param wait_frame �҂��t���[��
	* @param x �e�N�X�`���؂��蕝
	* @param y �e�N�X�`���؂��荂��
	* @details �e�N�X�`���A�j���[�V�����̃p�����[�^�[�̐ݒ肷��֐�
	*/
	void SetParam(const WORD wait_frame, const BYTE x, const BYTE y);

	/**
	* @brief �e�N�X�`���A�j���[�V�����̃e�N�X�`���؂�����W��ݒ肷��֐�
	* @param tx_param �e�N�X�`���؂��蕝�i�蓮�j
	* @param ty_param �e�N�X�`���؂��荂���i�蓮�j
	* @details �e�N�X�`���A�j���[�V�����̃e�N�X�`���؂�����W��ݒ肷��֐�
	*/
	void Set_Param_Txy(const float tx_param = -1.0f, const float ty_param = -1.0f);

	/**
	* @brief �A�j���[�V���������[�v���邩�̃t���O��ݒ肷��֐�
	* @param flag �A�j���[�V���������[�v�Đ����邩�̃t���O
	* @details �e�N�X�`���A�j���[�V�����̃A�j���[�V���������[�v�Đ����邩��ݒ肷��֐�
	*/
	void Set_Loop_Enable(const bool flag);

	/**
	* @brief �A�j���[�V���������[�v���邩�̃t���O���擾����֐�
	* @return bool ���[�v���邩�̃t���O
	* @details �e�N�X�`���A�j���[�V�����̃A�j���[�V���������[�v�Đ����邩�̃t���O���擾����֐�
	*/
	const bool Get_Loop_Enable();

	/**
	* @brief �e�N�X�`���̐؂��蕝�ƍ������擾����֐�
	* @return XMFLOAT2 �e�N�X�`���̐؂��蕝�ƍ���
	* @details �e�N�X�`���̐؂��蕝�ƍ������擾����֐�
	*/
	const XMFLOAT2 Get_Twh() {
		return XMFLOAT2(Tw, Th);
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<SPRITE>(this));
		ar(WaitFrame);
		ar(Pattern_Max_X);
		ar(Pattern_Max_Y);
		ar(Tx);
		ar(Ty);
		ar(Tw);
		ar(Th);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(WaitFrame);
	//	ar(Pattern_Max_X);
	//	ar(Pattern_Max_Y);
	//	ar(Tx);
	//	ar(Ty);
	//	ar(Tw);
	//	ar(Th);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(WaitFrame);
	//	ar(Pattern_Max_X);
	//	ar(Pattern_Max_Y);
	//	ar(Tx);
	//	ar(Ty);
	//	ar(Tw);
	//	ar(Th);
	//}
};

CEREAL_REGISTER_TYPE(SPRITE_ANIMATION)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, SPRITE_ANIMATION)

#endif // !SPRITE_ANIMATION_H