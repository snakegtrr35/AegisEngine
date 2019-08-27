#pragma once

#ifndef BULLET_H
#define BULLET_H

class CMODEL;
class CIRCLE_SHADOW;
class COLLISION;

class GAME_OBJECT;

class BULLET : public GAME_OBJECT {
protected:
	XMFLOAT3 MoveVector;
	CMODEL* Model;

	CIRCLE_SHADOW* Shadow;
	COLLISION* Collision;
	short HP;

public:
	BULLET();
	BULLET(XMFLOAT3& position, XMFLOAT3& move_vector);
	~BULLET();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void Set_Move_Vector(const XMFLOAT3 move_vector);			// 

	COLLISION* const Get_Collison() {
		return Collision;
	}
};

#endif // !ENEMY_H