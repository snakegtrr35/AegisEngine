#pragma once

#ifndef FIELD_H
#define FIELD_H

#include	"Game_Object.h"
#include	"texture.h"
#include	"Renderer.h"
#include	"main.h"

class FIELD : public GAME_OBJECT {
	VERTEX_3D Vertex[4];
	ID3D11Buffer* pVertexBuffer;	// ���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	TEXTURE* Texture;				// �e�N�X�`��
	XMFLOAT3 Position;				// �|�W�V����
	XMFLOAT2 WH;					// ���ƍ���

protected:


public:
	FIELD();
	//==============================
	// position : ���S���W
	// wh : ���ƍ���
	//==============================
	FIELD(XMFLOAT3 position, XMFLOAT2 wh);
	~FIELD();

	void Init(void);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	void SetPosition(const XMFLOAT3 position);					// �|�W�V�����̐ݒ�
	void SetWH(const XMFLOAT2 wh);								// ���ƍ����̐ݒ�
	void SetTexture(const string& const file_name);				// �e�N�X�`���̐ݒ�
};

#endif // !FIELD_H