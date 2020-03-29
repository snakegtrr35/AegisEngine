#pragma once

#ifndef MESH_DOOM_H
#define	MESH_DOOM_H

class GAME_OBJECT;
class TEXTURE;

class MESH_DOOM :public GAME_OBJECT
{
private:
	float			Radius;
	unsigned int	VertexNum;
	unsigned int	IndexNum;

	unique_ptr<ID3D11Buffer, Release>	VertexBuffer;	// 頂点バッファ
	unique_ptr<ID3D11Buffer, Release>	IndexBuffer;	// インデックスバッファ
	unique_ptr<TEXTURE> Texture;						// テクスチャ

public:
	MESH_DOOM() : Radius(500.0f) {}
	~MESH_DOOM() {}

	void Init() override;
	void Uninit() override;
	void Update(float delta_time) override;
	void Draw() override;
	void Draw_DPP() override;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Texture);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Texture);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Texture);
	//}
};

CEREAL_REGISTER_TYPE(MESH_DOOM)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, MESH_DOOM)

#endif // !MESH_DOOM_H