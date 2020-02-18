#pragma once

#ifndef BILL_BOARD_H
#define	BILL_BOARD_H

#include	"Game_Object.h"

#include	"Renderer.h"

class GAME_OBJECT;
class TEXTURE;

//==============================
// �r���{�[�h�N���X
//==============================
class BILL_BOARD : public GAME_OBJECT {
private:

protected:
	ID3D11Buffer* pVertexBuffer;			// ���_�o�b�t�@
	static ID3D11Buffer* pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	VERTEX_3D Vertex[4];					// ���_�f�[�^
	TEXTURE* Texture;						// �e�N�X�`��

	XMFLOAT2 WH;							// ���ƍ���

public:
	BILL_BOARD();
	//==============================
	// position : ���S���W
	// wh : ���ƍ���
	//==============================
	BILL_BOARD(XMFLOAT3 position, XMFLOAT2 wh);
	virtual ~BILL_BOARD();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	// ���ƍ����̐ݒ�
	void SetWH(const XMFLOAT2 wh);
	
	// �e�N�X�`���̐ݒ�
	void SetTexture(const string& const file_name);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, BILL_BOARD)

//////////////////////////////////////////////////

//==============================
// �r���{�[�h�A�j���[�V�����N���X
//==============================
class BILL_BOARD_ANIMATION : public BILL_BOARD {
private:
	float WaitFrame;
	unsigned short CreatCount;
	unsigned short PatternCount;
	unsigned short Age;

	static int FrameCount;

	float Tx, Ty;	// �e�N�X�`���؂�����W
	float Tw, Th;	// �e�N�X�`���̐؂��蕝�ƍ���

	unsigned char  Pattern_Max_X;
	unsigned char  Pattern_Max_Y;

	void Draw(float tx = -1.0f, float ty = -1.0f);
	void Draw_DPP(float tx = -1.0f, float ty = -1.0f);

protected:

public:

	float Tx_Param, Ty_Param;	// �e�N�X�`���؂�����W�i�蓮�j

	BILL_BOARD_ANIMATION();

	// �R���X�g���N�^
	// position : ���S���W
	// wh : ���ƍ���
	BILL_BOARD_ANIMATION(XMFLOAT3 position, XMFLOAT2 wh);
	~BILL_BOARD_ANIMATION();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	// �e�N�X�`���A�j���[�V�����̃p�����[�^�[�̐ݒ�
	//
	// ����:wait_frame ... �҂��t���[��
	//      x ... �e�N�X�`���؂���
	//      y ... �e�N�X�`���؂���
	void SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y);

	// �e�N�X�`���A�j���[�V�����̃e�N�X�`���؂�����W�̐ݒ�
	//
	// ����:tx_param ... �e�N�X�`���؂��蕝�i�蓮�j
	//      ty_param ... �e�N�X�`���؂��荂���i�蓮�j
	void Set_Param_Txy(const float& tx_param = -1.0f, const float& ty_param = -1.0f);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD_ANIMATION)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BILL_BOARD, BILL_BOARD_ANIMATION)

#endif // ! BILL_BOARD_H