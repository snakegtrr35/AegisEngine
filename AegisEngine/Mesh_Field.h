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
	XMFLOAT3		GridSize;
	XMINT2			GridNum;
	unsigned int	IndexNum;
	VERTEX_3D*		VertexArray;

	ID3D11Buffer*	VertexBuffer;		// 頂点バッファ
	ID3D11Buffer*	IndexBuffer;		// インデックスバッファ
	TEXTURE*		Texture;			// テクスチャ

public:
	MESH_FIELD();
	~MESH_FIELD() { Uninit(); };

	void Init();
	void Draw();
	void Update();
	void Uninit();

	const float Get_Height(const XMFLOAT3& position);

	void SetTexture(const string& const file_name);				// テクスチャの設定
};


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

	void Init();
	void Draw();
	void Update();
	void Uninit();

	const float Get_Height(const XMFLOAT3& position);
};

#endif // !MESH_FIELF_H