/**
 * @file Billboard.h
 * @brief �r���{�[�h�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef BILL_BOARD_H
#define	BILL_BOARD_H

#include	"Renderer.h"

class GAME_OBJECT;
class TEXTURE;

/**
* @brief �r���{�[�h�N���X
* @details �r���{�[�h�N���X
*/
class BILL_BOARD : public GAME_OBJECT {
protected:

	//! �C���f�b�N�X�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! �e�N�X�`��
	unique_ptr<TEXTURE> Texture;

	//! ���ƍ���
	XMFLOAT2 WH;

public:

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	BILL_BOARD();
	
	/**
	* @brief �R���X�g���N�^
	* @param position ���S���W
	* @param wh �T�C�Y
	* @details �����t���R���X�g���N�^
	*/
	BILL_BOARD(XMFLOAT3 position, XMFLOAT2 wh);

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	virtual ~BILL_BOARD();

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
	* @brief ���ƍ�����ݒ肷��֐�
	* @param wh �T�C�Y
	* @details ���ƍ�����ݒ肷��֐�
	*/
	void SetWH(const XMFLOAT2& wh);
	
	/**
	* @brief �e�N�X�`����ݒ肷��֐�
	* @param file_name �g�p����e�N�X�`����
	* @details �e�N�X�`��(�N���X)��ݒ肷��֐�
	*/
	void SetTexture(const string& const file_name);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, BILL_BOARD)

//////////////////////////////////////////////////

/**
* @brief �r���{�[�h�A�j���[�V�����N���X
* @details �r���{�[�h�A�j���[�V�����N���X
*/
class BILL_BOARD_ANIMATION : public BILL_BOARD {
private:

	//! �t���[���J�E���^
	static int FrameCount;

	//! �҂��t���[��(���̃p�^�[���ɑJ�ڂ���܂ł̃J�E���g)
	WORD WaitFrame;

	//! �������̃t���[��
	WORD CreatCount;

	//! ���݂̃A�j���[�V�����p�^�[���J�E���g
	WORD PatternCount;

	//! ����������̃t���[��
	WORD Age;

	//! �e�N�X�`���؂�����W(x)
	float Tx;

	//! �e�N�X�`���؂�����W(y)
	float Ty;

	//! �e�N�X�`���̐؂��蕝
	float Tw;

	//! �e�N�X�`���̐؂��荂��
	float Th;

	//! �C���f�b�N�X�o�b�t�@
	BYTE  Pattern_Max_X;

	//! �C���f�b�N�X�o�b�t�@
	BYTE  Pattern_Max_Y;

	/**
	* @brief �`�揈���֐�
	* @details ���ۂɕ`�揈��������֐�
	*/
	void Draw(float tx = -1.0f, float ty = -1.0f);

	/**
	* @brief �`��(Depth-pre-pass)�֐�
	* @details ���ۂ�Depth-pre-pass������֐�
	*/
	void Draw_DPP(float tx = -1.0f, float ty = -1.0f);

public:

	//! �e�N�X�`���؂�����W�i�蓮�j
	float Tx_Param, Ty_Param;	

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	BILL_BOARD_ANIMATION();

	/**
	* @brief �R���X�g���N�^
	* @param position ���S���W
	* @param wh �T�C�Y
	* @details �����t���R���X�g���N�^
	*/
	BILL_BOARD_ANIMATION(XMFLOAT3 position, XMFLOAT2 wh);

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~BILL_BOARD_ANIMATION();

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
	* @brief �p�����[�^�[�̐ݒ肷��֐�
	* @param wait_frame ����(�t���[���P��)
	* @param x  �A�j���[�V�����p�^�[����(���̐�)
	* @param y  �A�j���[�V�����p�^�[����(�c�̐�)
	* @details �A�j���[�V�����̃p�����[�^�[�̐ݒ肷��֐�
	*/
	void SetParam(const WORD wait_frame, const BYTE x, const BYTE y);

	/**
	* @brief �e�N�X�`���؂�����W�̐ݒ肷��֐�
	* @param tx_param �e�N�X�`���؂��蕝�i�蓮�j
	* @param ty_param �e�N�X�`���؂��荂���i�蓮�j
	* @details �A�j���[�V�����̃e�N�X�`���؂�����W�̐ݒ肷��֐�
	*/
	void Set_Param_Txy(const float tx_param = -1.0f, const float ty_param = -1.0f);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD_ANIMATION)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BILL_BOARD, BILL_BOARD_ANIMATION)

#endif // ! BILL_BOARD_H