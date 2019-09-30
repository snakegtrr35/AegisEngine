#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include	"Game_Object.h"

class CMODEL;
class BULLET;
class COLLISION;
class BOUNDING_SHPERE;
//class CIRCLE_SHADOW;

class PLAYER : public GAME_OBJECT {
private:
	CMODEL* Model;
	//CIRCLE_SHADOW* shadow;
	COLLISION* Collision;
	BOUNDING_SHPERE* Shpere;

	// true = Stop false = Walk
	bool AnimType;
	float Blend;

public:
	PLAYER();
	~PLAYER();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void SetPosition(const XMFLOAT3 position);					// ポジションの設定
	void SetScaling(const XMFLOAT3 scaling);					// 拡大縮小の値の設定

	COLLISION* const Get_Collision() {
		return Collision;
	}

	float* Get() {
		return &Blend;
	}
};

#endif // !PLAYER_H