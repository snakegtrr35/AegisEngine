/**
 * @file Mesh_Dome.h
 * @brief メッシュドームヘッダーファイル
 */

#pragma once

#ifndef MESH_DOOM_H
#define	MESH_DOOM_H

class GAME_OBJECT;
class TEXTURE;

class MESH_DOOM :public GAME_OBJECT {
private:
	//! 半径
	float								Radius;
	//! 頂点数
	UINT	VertexNum;
	//! インデックス数
	UINT								IndexNum;
	//! 頂点バッファ
	ComPtr<ID3D11Buffer>	VertexBuffer;
	//! インデックスバッファ
	ComPtr<ID3D11Buffer>	IndexBuffer;
	//! テクスチャ
	unique_ptr<TEXTURE>					Texture;						

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	MESH_DOOM();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~MESH_DOOM() { Uninit(); }

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