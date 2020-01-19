#pragma once

#ifndef BOUNDING_AABB_H
#define	BOUNDING_AABB_H

#include	"Bounding.h"

// AABB
class BOUNDING_AABB : public BOUNDING {
private:

	//XMFLOAT3 Radius;

public:
	BOUNDING_AABB();
	~BOUNDING_AABB();

	void Init() override;

	void Draw() override;
	void Draw_DPP() override {}

	void Update(float delta_time) override;

	void Uninit() override;

	void OverWrite() override;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//}
};

CEREAL_REGISTER_TYPE(BOUNDING_AABB)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_AABB)

#endif // !BOUNDING_AABB_H