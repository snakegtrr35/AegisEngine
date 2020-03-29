#pragma once

#ifndef FIELD_H
#define FIELD_H

#include	"texture.h"
#include	"Renderer.h"
#include	"main.h"

class GAME_OBJECT;

class FIELD : public GAME_OBJECT {
	VERTEX_3D Vertex[4];
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;	// 頂点バッファ

	unique_ptr<ID3D11Buffer, Release> pInstanceBuffer;	// インスタンシング用のバッファ

	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;		// インデックスバッファ
	unique_ptr<TEXTURE> Texture;						// テクスチャ
	XMFLOAT2 WH;										// 幅と高さ

protected:


public:
	FIELD();
	//==============================
	// position : 中心座標
	// wh : 幅と高さ
	//==============================
	FIELD(XMFLOAT3 position, XMFLOAT2 wh);
	~FIELD();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void SetPosition(const XMFLOAT3 position);					// ポジションの設定
	void SetWH(const XMFLOAT2 wh);								// 幅と高さの設定
	void SetTexture(const string& const file_name);				// テクスチャの設定

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Texture);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}*/
};

CEREAL_REGISTER_TYPE(FIELD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, FIELD)

#endif // !FIELD_H