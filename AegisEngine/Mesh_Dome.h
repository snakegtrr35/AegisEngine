//***********************************************************************************************
//
//  スカイドーム [skydome.h]
//
//***********************************************************************************************
#pragma once	// インクルードガード

#include	"Game_Object.h"

class TEXTURE;

//***********************************************************************************************
//  スカイドームクラス
//***********************************************************************************************
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
	MESH_DOOM() :Radius(500.0f) {};
	~MESH_DOOM() {};

	void Init() override;
	void Uninit() override;
	void Update(float delta_time) override;
	void Draw() override;

};
