#pragma once

#ifndef BOUNDING_SFRUSTUM_H
#define BOUNDING_SFRUSTUM_H

#include	"Bounding.h"

// 球
class BOUNDING_FRUSTUM : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// インデックスバッファ

	BoundingFrustum Frustum;

public:
	BOUNDING_FRUSTUM() {}
	~BOUNDING_FRUSTUM() { Uninit(); }

	void Init() override;
	void Draw() override;
	void Draw_DPP() override {}
	void Update(float delta_time) override;
	void Uninit() override;

	void OverWrite() override;

	const BoundingFrustum& Get_Collition();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<BOUNDING>(this));
	}
};

CEREAL_REGISTER_TYPE(BOUNDING_FRUSTUM)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_FRUSTUM)

#endif // !BOUNDING_FRUSTUM_H