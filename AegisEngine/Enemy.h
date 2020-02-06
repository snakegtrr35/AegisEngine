#pragma once

#ifndef ENEMY_H
#define ENEMY_H

class CMODEL;
class COLLISION;
class BOUNDING_AABB;
class GAME_OBJECT;

typedef struct
{
	float Attack_Probability;	// �U������m��
	float Lenght;				// �v���C���[�Ƃ̋���
	float Move_Lenght;			// �ړ���
	float RL_Probability;		// ���E�ړ���؂�ւ���m��
	bool RL;
}PROBABILITY_DATE;

class ENEMY : public GAME_OBJECT {
private:
	CMODEL* Model;
	COLLISION* Collision;

	BOUNDING_AABB* Aabb;

	PROBABILITY_DATE Date;

public:
	ENEMY();
	~ENEMY();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

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