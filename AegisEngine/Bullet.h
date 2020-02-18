#pragma once

#ifndef BULLET_H
#define BULLET_H

class CMODEL;
class GAME_OBJECT;

class BULLET : public GAME_OBJECT {
protected:
	XMFLOAT3 MoveVector;
	CMODEL* Model;

	short HP;
	float fps = 0.f;

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

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BULLET)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, BULLET)

#endif // !ENEMY_H