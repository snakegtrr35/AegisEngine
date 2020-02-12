#pragma once

#ifndef BOUNDING_OBB_H
#define BOUNDING_OBB_H

#include	"Bounding.h"

// OBB
class BOUNDING_OBB : public BOUNDING {
private:

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		//! BOXの頂点バッファ

	BoundingOrientedBox Obb;

	XMFLOAT3 Radius;

	XMFLOAT4 Quaternion;

public:
	BOUNDING_OBB() : Radius(XMFLOAT3(0.5f, 0.5f, 0.5f)), Quaternion(XMFLOAT4(0.f, 0.f, 0.f, 1.0f)) {}
	~BOUNDING_OBB();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override {}
	void Update(float delta_time) override;
	void Uninit() override;

	void Draw_Inspector() override;

	void OverWrite() override;

	//void Set_Radius(const XMFLOAT3& radius);

	//XMFLOAT3& const Get_Radius();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
		ar(Quaternion);
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