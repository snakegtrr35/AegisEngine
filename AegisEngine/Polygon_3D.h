#pragma once

#ifndef  POLYGON_3D_H
#define POLYGON_3D_H

#include	"Renderer.h"

class GameObject;
class TEXTURE;

struct POLYGOM {
	VERTEX_3D Vertex[4 * 6];
};

class POLYGON_3D : public GameObject {
private:
	ComPtr<ID3D11Buffer> pVertexBuffer;		// 頂点バッファ
	std::unique_ptr<TEXTURE> Texture;		// テクスチャ

	aegis::Vector3 XYZ;						// 幅と高さと奥行き

protected:


public:
	POLYGON_3D();
	//========================================
	// position : 中心座標
	// wh : 幅と高さ
	//========================================
	POLYGON_3D(aegis::Vector3 position, aegis::Vector3 xyz);
	~POLYGON_3D();

	void Init(void) override;
	void Draw(void) override;
	void Draw_Shadow() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit(void) override;

	void SetPosition(aegis::Vector3& position);					// ポジションの設定
	void SetXYZ(const aegis::Vector3 xyz);							// 幅と高さの設定
	void SetScaling(aegis::Vector3& scaling);							// 拡大縮小の値の設定
	void SetTexture(const std::string& file_name);			// テクスチャの設定

	aegis::Vector3* const Get_Position();
	aegis::Vector3* const Get_Rotation();
	aegis::Vector3* const Get_Scaling();

	//template<typename Archive>
	//void serialize(Archive& ar)
	//{
	//	ar(cereal::base_class<GameObject>(this));
	//	ar(Texture);
	//}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<GameObject>(this));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<GameObject>(this));
	}*/
};

//CEREAL_REGISTER_TYPE(POLYGOM)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, POLYGOM)

#endif // ! POLYGON_3D_H