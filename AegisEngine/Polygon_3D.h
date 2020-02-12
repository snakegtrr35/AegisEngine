#pragma once

#ifndef  POLYGON_3D_H
#define POLYGON_3D_H

#include	"Game_Object.h"
#include	"Renderer.h"

class TEXTURE;

struct POLYGOM {
	VERTEX_3D Vertex[4 * 6];
};

class POLYGON_3D : public GAME_OBJECT {
private:
	ID3D11Buffer* pVertexBuffer;		// ���_�o�b�t�@
	unique_ptr<TEXTURE> Texture;		// �e�N�X�`��

	XMFLOAT3 XYZ;						// ���ƍ����Ɖ��s��

protected:


public:
	POLYGON_3D();
	//========================================
	// position : ���S���W
	// wh : ���ƍ���
	//========================================
	POLYGON_3D(XMFLOAT3 position, XMFLOAT3 xyz);
	~POLYGON_3D();

	void Init(void) override;
	void Draw(void) override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit(void) override;

	void SetPosition(const XMFLOAT3 position);					// �|�W�V�����̐ݒ�
	void SetXYZ(const XMFLOAT3 xyz);							// ���ƍ����̐ݒ�
	void SetScaling(XMFLOAT3 scaling);							// �g��k���̒l�̐ݒ�
	void SetTexture(const string& const file_name);			// �e�N�X�`���̐ݒ�

	XMFLOAT3* const Get_Position();
	XMFLOAT3* const Get_Rotation();
	XMFLOAT3* const Get_Scaling();

	//template<typename Archive>
	//void serialize(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Texture);
	//}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}*/
};

//CEREAL_REGISTER_TYPE(POLYGOM)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, POLYGOM)

#endif // ! POLYGON_3D_H