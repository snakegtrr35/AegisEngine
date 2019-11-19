#pragma once

#ifndef BALL_H
#define BALL_H

#include	"Game_Object.h"

class BALL : public GAME_OBJECT {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;	//!< ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;		//!< �C���f�b�N�X�o�b�t�@

	XMVECTOR Quaternion;

public:
	BALL();
	~BALL() { Uninit(); }

	void Init() override;
	void Draw() override;
	void Update(float delta_time) override;
	void Uninit() override;
};

#endif // !BALL_H