#pragma once

#ifndef XYZ_AXIS_H
#define XYZ_AXIS_H

#include	"Renderer.h"
#include	"Game_Object.h"

class TEXTURE;

class AXIS : public GAME_OBJECT {
private:
	ID3D11Buffer* pVertexBuffer[3];	// ���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer;		// �C���f�b�N�X�o�b�t�@

public:
	AXIS();
	~AXIS();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;
};

#endif // !CUBE_H