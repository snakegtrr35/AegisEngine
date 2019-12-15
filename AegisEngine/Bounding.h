#pragma once

//Bounding Box

#ifndef BOUNDING_H
#define BOUNDING_H

#include	"Renderer.h"
#include	"Game_Object.h"

class BOUNDING : public GAME_OBJECT {
private:
protected:

	static unique_ptr<ID3D11Buffer, Release> pVertexBuffer_BOX;		//! BOXの頂点バッファ
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer_BOX;		//! BOXのインデックスバッファ
	static const char IndexNum_Box;									//! BOXのインデックス数

	COLOR Color;

public:
	BOUNDING() : Color(COLOR(1.0f, 0.f, 0.f, 1.0f)) {}
	virtual ~BOUNDING() {}

	void Init() override {}
	void Draw() override {}
	void Update(float delta_time) override {}
	void Uninit() override {}

	void Set_Color(const COLOR& color) {
		Color = color;
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Color);
	}
};

CEREAL_REGISTER_TYPE(BOUNDING)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, BOUNDING)

#endif // !BOUNDING_H