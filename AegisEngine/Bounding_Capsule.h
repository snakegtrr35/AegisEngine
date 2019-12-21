#pragma once

#ifndef BOUNDING_CAPSULE_H
#define	BOUNDING_CAPSULE_H

#include	"Bounding.h"

// 球
class BOUNDING_CAPSULE : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// インデックスバッファ
	UINT VertexNum;

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_Line;		// 線分の頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer_Line;		// 線分のインデックスバッファ
	UINT Line_VertexNum;

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_Ring;		// リングの頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer_Ring;		// リングのインデックスバッファ
	UINT IndexNum;

	float Radius;

	void Draw_Body(const XMFLOAT3& position, const XMFLOAT3& rotation);
	void Draw_Line(const XMFLOAT3& rotation);
	void Draw_Ring(const XMFLOAT3& rotation);

	void Init_Body();
	void Init_Line();
	void Init_Ring();

public:
	BOUNDING_CAPSULE() : Radius(0.f), IndexNum(0) {}
	~BOUNDING_CAPSULE() { Uninit(); }

	void Init() override;
	void Draw() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void OverWrite() override {}

	void Set_Radius(const float radius);

	const float Get_Radius();
};

CEREAL_REGISTER_TYPE(BOUNDING_CAPSULE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_CAPSULE)

#endif // !BOUNDING_CAPSULE_H