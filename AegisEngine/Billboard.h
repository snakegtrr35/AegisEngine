/**
 * @file Billboard.h
 * @brief ビルボード関係のヘッダーファイル
 */

#pragma once

#ifndef BILL_BOARD_H
#define	BILL_BOARD_H

#include	"Renderer.h"

class GAME_OBJECT;
class TEXTURE;

/**
* @brief ビルボードクラス
* @details ビルボードクラス
*/
class BILL_BOARD : public GAME_OBJECT {
protected:

	//! インデックスバッファ
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! 頂点バッファ
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! テクスチャ
	unique_ptr<TEXTURE> Texture;

	//! 幅と高さ
	XMFLOAT2 WH;

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BILL_BOARD();
	
	/**
	* @brief コンストラクタ
	* @param position 中心座標
	* @param wh サイズ
	* @details 引数付きコンストラクタ
	*/
	BILL_BOARD(XMFLOAT3 position, XMFLOAT2 wh);

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	virtual ~BILL_BOARD();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init() override;

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw() override;

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP() override;

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time) override;

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit() override;

	/**
	* @brief 幅と高さを設定する関数
	* @param wh サイズ
	* @details 幅と高さを設定する関数
	*/
	void SetWH(const XMFLOAT2& wh);
	
	/**
	* @brief テクスチャを設定する関数
	* @param file_name 使用するテクスチャ名
	* @details テクスチャ(クラス)を設定する関数
	*/
	void SetTexture(const string& const file_name);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, BILL_BOARD)

//////////////////////////////////////////////////

/**
* @brief ビルボードアニメーションクラス
* @details ビルボードアニメーションクラス
*/
class BILL_BOARD_ANIMATION : public BILL_BOARD {
private:

	//! フレームカウンタ
	static int FrameCount;

	//! 待ちフレーム(次のパターンに遷移するまでのカウント)
	WORD WaitFrame;

	//! 生成時のフレーム
	WORD CreatCount;

	//! 現在のアニメーションパターンカウント
	WORD PatternCount;

	//! 生成時からのフレーム
	WORD Age;

	//! テクスチャ切り取り座標(x)
	float Tx;

	//! テクスチャ切り取り座標(y)
	float Ty;

	//! テクスチャの切り取り幅
	float Tw;

	//! テクスチャの切り取り高さ
	float Th;

	//! インデックスバッファ
	BYTE  Pattern_Max_X;

	//! インデックスバッファ
	BYTE  Pattern_Max_Y;

	/**
	* @brief 描画処理関数
	* @details 実際に描画処理をする関数
	*/
	void Draw(float tx = -1.0f, float ty = -1.0f);

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details 実際にDepth-pre-passをする関数
	*/
	void Draw_DPP(float tx = -1.0f, float ty = -1.0f);

public:

	//! テクスチャ切り取り座標（手動）
	float Tx_Param, Ty_Param;	

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	BILL_BOARD_ANIMATION();

	/**
	* @brief コンストラクタ
	* @param position 中心座標
	* @param wh サイズ
	* @details 引数付きコンストラクタ
	*/
	BILL_BOARD_ANIMATION(XMFLOAT3 position, XMFLOAT2 wh);

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~BILL_BOARD_ANIMATION();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init() override;

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw() override;

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP() override;

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time) override;

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit() override;

	/**
	* @brief パラメーターの設定する関数
	* @param wait_frame 時間(フレーム単位)
	* @param x  アニメーションパターン数(横の数)
	* @param y  アニメーションパターン数(縦の数)
	* @details アニメーションのパラメーターの設定する関数
	*/
	void SetParam(const WORD wait_frame, const BYTE x, const BYTE y);

	/**
	* @brief テクスチャ切り取り座標の設定する関数
	* @param tx_param テクスチャ切り取り幅（手動）
	* @param ty_param テクスチャ切り取り高さ（手動）
	* @details アニメーションのテクスチャ切り取り座標の設定する関数
	*/
	void Set_Param_Txy(const float tx_param = -1.0f, const float ty_param = -1.0f);

	template<typename Archive>
	void serialize(Archive& ar) {}
};

CEREAL_REGISTER_TYPE(BILL_BOARD_ANIMATION)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BILL_BOARD, BILL_BOARD_ANIMATION)

#endif // ! BILL_BOARD_H