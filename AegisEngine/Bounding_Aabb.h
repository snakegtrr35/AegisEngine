#pragma once

#ifndef BOUNDING_AABB_H
#define	BOUNDING_AABB_H

#include	"Bounding.h"

// AABB
class BOUNDING_AABB : public BOUNDING {
private:

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		//! BOXの頂点バッファ

	XMFLOAT3 Radius;

public:
	BOUNDING_AABB() : Radius(XMFLOAT3(0.5f, 0.5f, 0.5f)) {};
	~BOUNDING_AABB();

	void Init() override;

	void Draw() override;
	void Draw_DPP() override {}
	void Update(float delta_time) override;
	void Uninit() override;

	void Draw_Inspector() override;

	void OverWrite() override;

	void Set_Radius(const XMFLOAT3& radius);
	void Set_Radius(const XMFLOAT3* radius);

	XMFLOAT3* Get_Radius() {
		return &Radius;
	}

	BoundingBox Aabb;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
		ar(Radius);
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