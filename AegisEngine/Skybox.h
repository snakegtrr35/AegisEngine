#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H


class GameObject;
//class TEXTURE;

class SKYBOX : public GameObject {
private:
	//! 頂点バッファ
	static ComPtr<ID3D11Buffer>	VertexBuffer;

	//! インデックスバッファ
	static ComPtr<ID3D11Buffer>	IndexBuffer;

	////! テクスチャ
	//std::unique_ptr<TEXTURE>					Texture;

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	SKYBOX();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	~SKYBOX();

	/**
	* @brief 初期化関数
	* @details 初期化する関数
	*/
	void Init();

	/**
	* @brief 描画関数
	* @details 描画する関数
	*/
	void Draw();

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details Depth-pre-passをする関数
	*/
	void Draw_DPP();

	/**
	* @brief 更新関数
	* @details 更新する関数
	*/
	void Update(float delta_time);

	/**
	* @brief 終了処理関数
	* @details 終了処理をする関数
	*/
	void Uninit();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GameObject>(this));
		//ar(Texture);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GameObject>(this));
	//	ar(Texture);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GameObject>(this));
	//	ar(Texture);
	//}
};

CEREAL_REGISTER_TYPE(SKYBOX)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, SKYBOX)

#endif // !SKYBOX_H