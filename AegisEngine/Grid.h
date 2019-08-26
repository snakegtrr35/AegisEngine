#pragma once

#ifndef CUBE_H
#define CUBE_H

#include	"Renderer.h"
#include	"Game_Object.h"


class GRID : public GAME_OBJECT {
private:
	ID3D11Buffer* pVertexBuffer;	// 頂点バッファ

public:
	GRID();
	~GRID();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;
};

#endif // !CUBE_H