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
	// 共通部分
	UINT			Enable;
	aegis::Vector3	Position;

	aegis::COLOR	Color;

	UINT			Type;
	aegis::Vector3	Dummy;

	LIGHT_BUFFER();

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Enable", Enable),
				cereal::make_nvp("Position", Position),
				cereal::make_nvp("Color", Color),
				cereal::make_nvp("Type", Type)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("Enable", Enable),
			cereal::make_nvp("Position", Position),
			cereal::make_nvp("Color", Color),
			cereal::make_nvp("Type", Type)
		);
	}
};



class LIGHTS {
private:

	static aegis::array<LIGHT_BUFFER, MAX_NUM_LIGHTS> Lights;

	static ComPtr<ID3D11Buffer>		LightBuffer;

public:
	LIGHTS();
	~LIGHTS() { Uninit(); }

	bool Init();
	void Draw();
	void Update();
	void Uninit();

	 static aegis::array<LIGHT_BUFFER, MAX_NUM_LIGHTS>* Get_Lights() {
		return &Lights;
	}

	 static ID3D11Buffer* Get_LightBuffer() { return LightBuffer.Get(); }

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Lights", Lights));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("Lights", Lights));
	}
};
#endif // !LIGHT_H