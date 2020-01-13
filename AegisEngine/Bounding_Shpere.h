#pragma once

#ifndef BOUNDING_SHPERE_H
#define BOUNDING_SHPERE_H

#include	"Bounding.h"

// ��
class BOUNDING_SHPERE : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// �C���f�b�N�X�o�b�t�@

	float Radius;
	UINT IndexNum;

	void Draw_Ring(const XMFLOAT3& rotation);

	void Create_Buffer();

public:
	BOUNDING_SHPERE() : Radius(0.f), IndexNum(0) {}
	~BOUNDING_SHPERE() { Uninit(); }

	void Init() override;
	void Draw() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void OverWrite() override;

	void Set_Radius(const float radius);

	const float Get_Radius();
};

CEREAL_REGISTER_TYPE(BOUNDING_SHPERE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BOUNDING, BOUNDING_SHPERE)

#endif // !BOUNDING_SHPERE_H