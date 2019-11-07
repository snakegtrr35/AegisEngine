#pragma once

#ifndef BALL_H
#define BALL_H

#include	"Game_Object.h"

class BALL : public GAME_OBJECT {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;	//!< 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;		//!< インデックスバッファ

	XMVECTOR Quaternion;

public:
	BALL();
	~BALL() { Uninit(); }

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;
};

#endif // !BALL_H