/**
 * @file Mesh_Field.h
 * @brief メッシュフィールド関係のヘッダーファイル
 */

#pragma once

#ifndef MESH_FIELF_H
#define MESH_FIELF_H

#include	"Renderer.h"

class GAME_OBJECT;
class TEXTURE;

//==============================
//  フィールドクラス
//==============================
class MESH_FIELD : public GAME_OBJECT {
private:

	//! グリッドサイズ
	Aegis::Vector3						GridSize;

	//! グリッド数
	Aegis::Int2							GridNum;

	//! インデックス数
	UINT								IndexNum;

	// 頂点データ
	vector<VERTEX_3D>					VertexArray;

	//! 頂点バッファ
	unique_ptr<ID3D11Buffer, Release>	VertexBuffer;

	//! インデックスバッファ
	unique_ptr<ID3D11Buffer, Release>	IndexBuffer;

	//! テクスチャ
	unique_ptr<TEXTURE>					Texture;

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	MESH_FIELD();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~MESH_FIELD() { Uninit(); }

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
	* @brief メッシュフィールドの高さを取得する関数
	* @param position メッシュフィールドの座標
	* @return float メッシュフィールドの高さ
	* @details 引数の座標のメッシュフィールドの高さを取得する関数
	*/
	const float Get_Height(const Aegis::Vector3& position);

	/**
	* @brief テクスチャを設定する関数
	* @param file_name 使用するテクスチャ名
	* @details テクスチャ(クラス)を設定する関数
	*/
	void SetTexture(const string& const file_name);

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
//  ウォールクラス
//==============================
class MESH_WALL :public GAME_OBJECT {
private:
	Aegis::Vector3		GridSize;
	Aegis::Int2			GridNum;
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

	const float Get_Height(const Aegis::Vector3& position);
};

#endif // !MESH_FIELF_H