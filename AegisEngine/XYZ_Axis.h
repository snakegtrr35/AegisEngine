#pragma once

#ifndef XYZ_AXIS_H
#define XYZ_AXIS_H

#include	"Renderer.h"
#include	"GameObject.h"

class TEXTURE;

class AXIS : public GAME_OBJECT {
private:
	ComPtr<ID3D11Buffer> pVertexBuffer[3];	// ���_�o�b�t�@
	ComPtr<ID3D11Buffer> pIndexBuffer;		// �C���f�b�N�X�o�b�t�@

public:
	AXIS();
	~AXIS();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override {}
	void Update(float delta_time) override;
	void Uninit() override;
};

#endif // !CUBE_H