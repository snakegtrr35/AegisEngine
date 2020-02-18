#pragma once

#ifndef BILL_BOARD_H
#define	BILL_BOARD_H

#include	"Game_Object.h"

#include	"Renderer.h"

class GAME_OBJECT;
class TEXTURE;

//==============================
// ビルボードクラス
//==============================
class BILL_BOARD : public GAME_OBJECT {
private:

protected:
	ID3D11Buffer* pVertexBuffer;			// 頂点バッファ
	static ID3D11Buffer* pIndexBuffer;		// インデックスバッファ
	VERTEX_3D Vertex[4];					// 頂点データ
	TEXTURE* Texture;						// テクスチャ

	XMFLOAT2 WH;							// 幅と高さ

public:
	BILL_BOARD();
	//==============================
	// position : 中心座標
	// wh : 幅と高さ
	//==============================
	BILL_BOARD(XMFLOAT3 position, XMFLOAT2 wh);
	virtual ~BILL_BOARD();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	// 幅と高さの設定
	void SetWH(const XMFLOAT2 wh);
	
	// テクスチャの設定
	void SetTexture(const string& const file_name);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, BILL_BOARD)

//////////////////////////////////////////////////

//==============================
// ビルボードアニメーションクラス
//==============================
class BILL_BOARD_ANIMATION : public BILL_BOARD {
private:
	float WaitFrame;
	unsigned short CreatCount;
	unsigned short PatternCount;
	unsigned short Age;

	static int FrameCount;

	float Tx, Ty;	// テクスチャ切り取り座標
	float Tw, Th;	// テクスチャの切り取り幅と高さ

	unsigned char  Pattern_Max_X;
	unsigned char  Pattern_Max_Y;

	void Draw(float tx = -1.0f, float ty = -1.0f);
	void Draw_DPP(float tx = -1.0f, float ty = -1.0f);

protected:

public:

	float Tx_Param, Ty_Param;	// テクスチャ切り取り座標（手動）

	BILL_BOARD_ANIMATION();

	// コンストラクタ
	// position : 中心座標
	// wh : 幅と高さ
	BILL_BOARD_ANIMATION(XMFLOAT3 position, XMFLOAT2 wh);
	~BILL_BOARD_ANIMATION();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	// テクスチャアニメーションのパラメーターの設定
	//
	// 引数:wait_frame ... 待ちフレーム
	//      x ... テクスチャ切り取り
	//      y ... テクスチャ切り取り
	void SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y);

	// テクスチャアニメーションのテクスチャ切り取り座標の設定
	//
	// 引数:tx_param ... テクスチャ切り取り幅（手動）
	//      ty_param ... テクスチャ切り取り高さ（手動）
	void Set_Param_Txy(const float& tx_param = -1.0f, const float& ty_param = -1.0f);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD_ANIMATION)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BILL_BOARD, BILL_BOARD_ANIMATION)

#endif // ! BILL_BOARD_H