#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include	"Game_Object.h"

typedef struct
{
	float Attack_Probability;	// �U������m��
	float Lenght;				// �v���C���[�Ƃ̋���
	float Move_Lenght;			// �ړ���
	float RL_Probability;		// ���E�ړ���؂�ւ���m��
	bool RL;
}PROBABILITY_DATE;

class CMODEL;
class COLLISION;

class BOUNDING_AABB;

class ENEMY : public GAME_OBJECT {
private:
	CMODEL* Model;
	COLLISION* Collision;

	BOUNDING_AABB* Aabb;

	PROBABILITY_DATE Date;

public:
	ENEMY();
	~ENEMY();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void SetPosition(const XMFLOAT3& position);					// �|�W�V�����̐ݒ�
	void SetRotation(const XMFLOAT3& position);					// ��]�̐ݒ�
	void SetScaling(const XMFLOAT3& scaling);					// �g��k���̒l�̐ݒ�

	COLLISION* const Get_Collision() {
		return Collision;
	}

	void Set_Date(const PROBABILITY_DATE& date) {
		Date = date;
	}
};

#endif // !ENEMY_H