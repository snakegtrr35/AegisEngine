/**
 * @file Mesh_Cylinder.h
 * @brief メッシュシリンダーのヘッダーファイル
 */

#pragma once

#ifndef MESH_CYLINDER_H
#define MESH_CYLINDER_H

#include	"texture.h"

class GameObject;

class MESH_CYlLINDER : public GameObject {
private:
	int				CylinderLength;
	float			Radius;
	unsigned int	VertexNum;
	unsigned int	IndexNum;

	ComPtr<ID3D11Buffer>	VertexBuffer;	// 頂点バッファ
	ComPtr<ID3D11Buffer>	IndexBuffer;	// インデックスバッファ
	std::unique_ptr<TEXTURE> Texture;						// テクスチャ

public:

	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	MESH_CYlLINDER() : Radius(45.0f), CylinderLength(7) {}

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~MESH_CYlLINDER() {}

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
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw_Shadow() override;

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

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
		archive(cereal::make_nvp("Texture", Texture));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
		archive(cereal::make_nvp("Texture", Texture));
	}
};

CEREAL_REGISTER_TYPE(MESH_CYlLINDER)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, MESH_CYlLINDER)

#endif // !MESH_CYLINDER_H