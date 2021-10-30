#pragma once

#ifndef XYZ_AXIS_H
#define XYZ_AXIS_H

#include	"Renderer.h"
#include	"GameObject.h"

class TEXTURE;

class AXIS : public GAME_OBJECT {
private:
	ComPtr<ID3D11Buffer> pVertexBuffer[3];	// 頂点バッファ
	ComPtr<ID3D11Buffer> pIndexBuffer;		// インデックスバッファ

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