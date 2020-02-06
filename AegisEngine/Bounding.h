#pragma once

//Bounding Box

#ifndef BOUNDING_H
#define BOUNDING_H

#include	"Renderer.h"
#include	"Component.h"

class BOUNDING : public COMPONENT {
private:
protected:

	static unique_ptr<ID3D11Buffer, Release> pVertexBuffer_BOX;		//! BOXの頂点バッファ
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer_BOX;		//! BOXのインデックスバッファ
	static const char IndexNum_Box;									//! BOXのインデックス数
	
	static COLOR Default_Color;

	COLOR Color;

public:
	BOUNDING() : Color(Default_Color) {}
	virtual ~BOUNDING() {}

	virtual void Init() {}
	virtual void Draw() {}
	virtual void Draw_DPP() {}
	virtual void Update(float delta_time) {}
	virtual void Uninit() {}

	virtual void OverWrite() = 0;

	static void Set_Default_Color(const COLOR& color) {
		Default_Color = color;
	}

	static COLOR Get_Default_Color() {
		return Default_Color;
	}

	void Set_Color(const COLOR& color) {
		Color = color;
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<COMPONENT>(this));
		ar(Default_Color);
		ar(Color);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Default_Color);
	//	ar(Color);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Default_Color);
	//	ar(Color);
	//}
};

CEREAL_REGISTER_TYPE(BOUNDING)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT, BOUNDING)

#endif // !BOUNDING_H