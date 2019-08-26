#pragma once

//Bounding Box

#ifndef BOUNDING_H
#define BOUNDING_H

#include	"main.h"
#include	"Renderer.h"
#include	"Game_Object.h"

class BOUNDING : public GAME_OBJECT {
private:

protected:
	VERTEX_3D* Vertex;					// 頂点データ
	ID3D11Buffer* pVertexBuffer;		// 頂点バッファ
	ID3D11Buffer* pIndexBuffer;			// インデックスバッファ

public:
	BOUNDING() {
		Position = XMFLOAT3(0.f, 0.f, 0.f);

		Vertex = nullptr;

		pIndexBuffer = pVertexBuffer = nullptr;
	};
	virtual ~BOUNDING() {
		SAFE_RELEASE(pVertexBuffer);
		SAFE_RELEASE(pIndexBuffer);
		SAFE_DELETE(Vertex);
	};

	void Init(void) override {};
	void Draw(void) override {};
	void Update(void) override {};
	void Uninit(void) override {};

	void Set_Position(const XMFLOAT3& position) {
		Position = position;
	};

	XMFLOAT3& const Get_Position() {
		return Position;
	};
};

// 球
class BOUNDING_SHPERE : public BOUNDING {
private:
	float Radius;
	unsigned char Cnt;

	ID3D11Buffer* pVertexBuffer[2];		// 頂点バッファ

public:
	BOUNDING_SHPERE ();
	~BOUNDING_SHPERE ();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void Set_Radius(const float radius);

	const float Get_Radius();

	void Set_Cnt(const unsigned char& cnt);
};

// AABB
class BOUNDING_AABB : public BOUNDING {
private:
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