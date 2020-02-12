#pragma once

#ifndef MESH_FIELF_H
#define MESH_FIELF_H

#include	"Game_Object.h"

class TEXTURE;

//==============================
//  フィールドクラス
//==============================
class MESH_FIELD :public GAME_OBJECT {
private:
	XMFLOAT3				GridSize;
	XMINT2					GridNum;
	unsigned int			IndexNum;
	VERTEX_3D*				VertexArray;

	ID3D11Buffer*			VertexBuffer;		// 頂点バッファ
	ID3D11Buffer*			IndexBuffer;		// インデックスバッファ
	unique_ptr<TEXTURE>		Texture;			// テクスチャ

public:
	MESH_FIELD();
	~MESH_FIELD() { Uninit(); }

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	const float Get_Height(const XMFLOAT3& position);

	void SetTexture(const string& const file_name);				// テクスチャの設定

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

CEREAL_REGISTER_TYPE(MESH_FIELD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, MESH_FIELD)

//==============================
//  フィールドクラス
//==============================
class MESH_WALL :public GAME_OBJECT {
private:
	XMFLOAT3		GridSize;
	XMINT2			GridNum;
	unsigned int	IndexNum;
	VERTEX_3D* VertexArray;

	ID3D11Buffer* VertexBuffer;		// 頂点バッファ
	ID3D11Buffer* IndexBuffer;		// インデックスバッファ
	TEXTURE* Texture;				// テクスチャ

public:
	MESH_WALL();
	~MESH_WALL() { Uninit(); };

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	const float Get_Height(const XMFLOAT3& position);
};

#endif // !MESH_FIELF_H