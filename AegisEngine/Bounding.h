#pragma once

//Bounding Box

#ifndef BOUNDING_H
#define BOUNDING_H

#include	"Renderer.h"
#include	"Game_Object.h"

class BOUNDING : public GAME_OBJECT {
private:
protected:

	COLOR Color;

public:
	BOUNDING() : Color(COLOR(0.f, 1.0f, 0.f, 1.0f)) {}
	virtual ~BOUNDING() {}

	void Init() override {}
	void Draw() override {}
	void Update(float delta_time) override {}
	void Uninit() override {}

	void Set_Color(const COLOR& color) {
		Color = color;
	}
};

// 球
class BOUNDING_SHPERE : public BOUNDING {
private:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// インデックスバッファ

	float Radius;
	UINT IndexNum;

	void Draw_Ring(const XMFLOAT3& rotation);

	void Create_Buffer();

public:
	BOUNDING_SHPERE ();
	~BOUNDING_SHPERE ();

	void Init() override;
	void Draw() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void Set_Radius(const float radius);

	const float Get_Radius();
};

// AABB
class BOUNDING_AABB : public BOUNDING {
private:
	static unique_ptr<ID3D11Buffer, Release> pVertexBuffer;			// 頂点バッファ
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;			// インデックスバッファ
	static const char IndexNum;

	//XMFLOAT3 Radius;

public:
	BOUNDING_AABB();
	~BOUNDING_AABB();

	void Init() override;
	void Draw() override;
	void Update(float delta_time) override;
	void Uninit() override;

	//void Set_Radius(const XMFLOAT3& radius);

	//XMFLOAT3& const Get_Radius();
};

#endif // !BOUNDING_H