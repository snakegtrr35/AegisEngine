#pragma once

//Bounding Box

#ifndef BOUNDING_H
#define BOUNDING_H

#include	"Renderer.h"
#include	"Game_Object.h"

class BOUNDING : public GAME_OBJECT {
private:
protected:
public:
	BOUNDING() {}
	virtual ~BOUNDING() {}

	void Init(void) override {}
	void Draw(void) override {}
	void Update(void) override {}
	void Uninit(void) override {}
};

// ��
class BOUNDING_SHPERE : public BOUNDING {
private:
	static unique_ptr<ID3D11Buffer, Release> pVertexBuffer;			// ���_�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// �C���f�b�N�X�o�b�t�@

	float Radius;
	UINT IndexNum;

	void Draw_Ring(const XMFLOAT3& rotation);

public:
	BOUNDING_SHPERE ();
	~BOUNDING_SHPERE ();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override {}

	void Set_Radius(const float radius);

	const float Get_Radius();
};

// AABB
class BOUNDING_AABB : public BOUNDING {
private:
	static unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// ���_�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// �C���f�b�N�X�o�b�t�@

	XMFLOAT3 Radius;

public:
	BOUNDING_AABB();
	~BOUNDING_AABB();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void Set_Radius(const XMFLOAT3& radius);

	XMFLOAT3& const Get_Radius();
};

#endif // !BOUNDING_H