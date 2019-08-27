#pragma once

#ifndef CIRCLE_SHADOW_H
#define	CIRCLE_SHADOW_H

#include	"Game_Object.h"
#include	"Renderer.h"

class TEXTURE;

class CIRCLE_SHADOW : public GAME_OBJECT {
private:
	ID3D11Buffer* pVertexBuffer;	// ���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	VERTEX_3D Vertex[4];			// ���_�f�[�^
	TEXTURE* Texture;				// �e�N�X�`��
	XMFLOAT2 WH;					// ���ƍ���

protected:


public:
	CIRCLE_SHADOW();
	//==============================
	// position : ���S���W
	// wh : ���ƍ���
	//==============================
	CIRCLE_SHADOW(XMFLOAT3 position, XMFLOAT2 wh);
	~CIRCLE_SHADOW();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void SetWH(const XMFLOAT2 wh);								// ���ƍ����̐ݒ�
	void SetTexture(const string& const file_name);			// �e�N�X�`���̐ݒ�
};

#endif // !CIRCLE_SHADOW_H
