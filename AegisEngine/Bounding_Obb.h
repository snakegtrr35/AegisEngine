#pragma once

#ifndef BOUNDING_OBB_H
#define BOUNDING_OBB_H

#include	"Bounding.h"

// OBB
class BOUNDING_OBB : public BOUNDING {
private:

	//XMFLOAT3 Radius;

public:
	BOUNDING_OBB() {}
	~BOUNDING_OBB();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override {}
	void Update(float delta_time) override;
	void Uninit() override;

	void OverWrite() override;

	//void Set_Radius(const XMFLOAT3& radius);

	//XMFLOAT3& const Get_Radius();

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

CEREAL_REGISTER_TYPE(BOUNDING_OBB)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_OBB)

#endif // !BOUNDING_OBB_H