#pragma once

#ifndef BULLET_H
#define BULLET_H

//#include	"GameObject.h"

class GameObject;
class CMODEL;

class BULLET : public GameObject {
protected:
	Aegis::Vector3 MoveVector;
	unique_ptr<CMODEL> Model;

	short HP;
	float fps = 0.f;

public:
	BULLET();
	BULLET(Aegis::Vector3& position, Aegis::Vector3& move_vector);
	~BULLET();

	void Init() override;
	void Draw() override;
	void Draw_Shadow() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void Set_Move_Vector(const Aegis::Vector3 move_vector);			// 

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BULLET)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, BULLET)

#endif // !ENEMY_H