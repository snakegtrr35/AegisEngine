#pragma once

#ifndef BOUNDING_CAPSULE_H
#define	BOUNDING_CAPSULE_H

#include	"Bounding.h"

// ��
class BOUNDING_CAPSULE : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	UINT VertexNum;

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_Line;		// �����̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer_Line;		// �����̃C���f�b�N�X�o�b�t�@
	UINT Line_VertexNum;

	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_Ring;		// �����O�̒��_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer_Ring;		// �����O�̃C���f�b�N�X�o�b�t�@
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