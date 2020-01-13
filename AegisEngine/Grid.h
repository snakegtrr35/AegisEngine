#pragma once

#ifndef CUBE_H
#define CUBE_H

#include	"Renderer.h"
#include	"Game_Object.h"

class GRID : public GAME_OBJECT {
private:
	ID3D11Buffer* pVertexBuffer;	// ���_�o�b�t�@

public:
	GRID();
	~GRID();

	void Init() override;
	void Draw() override;
	void Update(float delta_time) override;
	void Uninit() override;
};

#endif // !CUBE_H