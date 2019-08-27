#pragma once

#ifndef CIRCLE_SHADOW_H
#define	CIRCLE_SHADOW_H

#include	"Game_Object.h"
#include	"Renderer.h"

class TEXTURE;

class CIRCLE_SHADOW : public GAME_OBJECT {
private:
	ID3D11Buffer* pVertexBuffer;	// 頂点バッファ
	ID3D11Buffer* pIndexBuffer;		// インデックスバッファ
	VERTEX_3D Vertex[4];			// 頂点データ
	TEXTURE* Texture;				// テクスチャ
	XMFLOAT2 WH;					// 幅と高さ

protected:


public:
	CIRCLE_SHADOW();
	//==============================
	// position : 中心座標
	// wh : 幅と高さ
	//==============================
	CIRCLE_SHADOW(XMFLOAT3 position, XMFLOAT2 wh);
	~CIRCLE_SHADOW();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void SetWH(const XMFLOAT2 wh);								// 幅と高さの設定
	void SetTexture(const string& const file_name);			// テクスチャの設定
};

#endif // !CIRCLE_SHADOW_H
