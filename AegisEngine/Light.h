#pragma once

#ifndef LIGHT_H
#define LIGHT_H

static const constexpr UINT MAX_NUM_LIGHTS = 512;

enum class LIGHT_TYPE : UINT {
	POINT = 0,
	SPOT,
	NONE,
};

struct LIGHT_BUFFER {
	// ‹¤’Ê•”•ª
	UINT		Enable;
	XMFLOAT3	Position;

	COLOR		Color;

	UINT		Type;
	XMFLOAT3	Dummy;

	LIGHT_BUFFER();
};

template<typename Archive>
void serialize(Archive& ar, LIGHT_BUFFER& light)
{
	ar(light.Enable);
	ar(light.Position);
	ar(light.Color);
	ar(light.Type);
}

class LIGHTS {
private:

	static array<LIGHT_BUFFER, MAX_NUM_LIGHTS> Lights;

	static unique_ptr <ID3D11Buffer, Release>		LightBuffer;

public:
	LIGHTS();
	~LIGHTS() { Uninit(); }

	bool Init();
	void Draw();
	void Update();
	void Uninit();

	 static array<LIGHT_BUFFER, MAX_NUM_LIGHTS>* Get_Lights() {
		return &Lights;
	}

	 static ID3D11Buffer* Get_LightBuffer() { return LightBuffer.get(); }

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(Lights);
	}
};
#endif // !LIGHT_H