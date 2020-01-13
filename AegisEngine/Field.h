#pragma once

#ifndef FIELD_H
#define FIELD_H

#include	"Game_Object.h"
#include	"texture.h"
#include	"Renderer.h"
#include	"main.h"

class FIELD : public GAME_OBJECT {
	VERTEX_3D Vertex[4];
	ID3D11Buffer* pVertexBuffer;	// 頂点バッファ
	ID3D11Buffer* pIndexBuffer;		// インデックスバッファ
	TEXTURE* Texture;				// テクスチャ
	XMFLOAT2 WH;					// 幅と高さ

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
	void Update(float delta_time) override;
	void Uninit() override;

	void SetPosition(const XMFLOAT3 position);					// ポジションの設定
	void SetWH(const XMFLOAT2 wh);								// 幅と高さの設定
	void SetTexture(const string& const file_name);				// テクスチャの設定
};

#endif // !FIELD_H