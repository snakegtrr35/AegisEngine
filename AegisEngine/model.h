#pragma once

#ifndef MODEL_H
#define MODEL_H

#include	"Renderer.h"

// マテリアル構造体
struct MODEL_MATERIAL
{
	char				Name[256];
	MATERIAL			Material;
	char				TextureName[256];
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D*			VertexArray;
	unsigned short		VertexNum;
	unsigned short*		IndexArray;
	unsigned short		IndexNum;
	SUBSET*				SubsetArray;
	unsigned short		SubsetNum;
};

class CModel {
private:
	ID3D11Buffer*	pVertexBuffer = NULL;
	ID3D11Buffer*	pIndexBuffer = NULL;

	DX11_SUBSET*	SubsetArray = NULL;
	unsigned short	SubsetNum;

	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scaling;

	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum );

public:
	CModel();
	CModel(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scaling);
	~CModel();

	void Init(const string* file_name);
	void Draw();

	void Load( const char *FileName );
	void Unload();

	void SetPosition(const XMFLOAT3 position);
	void SetRotation(const XMFLOAT3 rotation);
};

#endif // !MODEL_H