#pragma once

#ifndef BULLET_H
#define BULLET_H

class CMODEL;
class COLLISION;

class GAME_OBJECT;

class BULLET : public GAME_OBJECT {
protected:
	XMFLOAT3 MoveVector;
	CMODEL* Model;

	COLLISION* Collision;
	short HP;

public:
	BULLET();
	BULLET(XMFLOAT3& position, XMFLOAT3& move_vector);
	~BULLET();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void Set_Move_Vector(const XMFLOAT3 move_vector);			// 

	COLLISION* const Get_Collison() {
		return Collision;
	}
};

#endif // !ENEMY_H