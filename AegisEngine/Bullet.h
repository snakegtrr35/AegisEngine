#pragma once

#ifndef BULLET_H
#define BULLET_H

//#include	"GameObject.h"

class GameObject;

class BULLET : public GameObject {

	OBJECT_TYPE_INFO(BULLET)

protected:
	aegis::Vector3 MoveVector;

	short HP;
	float fps = 0.f;

public:
	BULLET();
	BULLET(aegis::Vector3& position, aegis::Vector3& move_vector);
	~BULLET();

	void Init() override;
	void Draw() override;
	void Draw_Shadow() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void Set_Move_Vector(const aegis::Vector3 move_vector);			// 

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BULLET)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, BULLET)

#endif // !ENEMY_H