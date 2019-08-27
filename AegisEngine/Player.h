#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include	"Game_Object.h"

class CMODEL;
class BULLET;
class COLLISION;
class CIRCLE_SHADOW;

class PLAYER : public GAME_OBJECT {
private:
	CMODEL* Model;
	CIRCLE_SHADOW* shadow;
	COLLISION* Collision;

	float HP;

public:
	PLAYER();
	~PLAYER();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void SetPosition(const XMFLOAT3 position);					// �|�W�V�����̐ݒ�
	void SetScaling(const XMFLOAT3 scaling);					// �g��k���̒l�̐ݒ�

	COLLISION* const Get_Collision() {
		return Collision;
	}

	const float Get_HP() {
		return HP;
	}

	void Damage() {
		HP -= 2.0f;
	}
};

#endif // !PLAYER_H