#pragma once

#ifndef FIELD_H
#define FIELD_H

#include	"texture.h"
#include	"Renderer.h"
#include	"main.h"

class GAME_OBJECT;

class FIELD : public GAME_OBJECT {
	VERTEX_3D Vertex[4];
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;	// ���_�o�b�t�@

	unique_ptr<ID3D11Buffer, Release> pInstanceBuffer;	// �C���X�^���V���O�p�̃o�b�t�@

	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	unique_ptr<TEXTURE> Texture;						// �e�N�X�`��
	XMFLOAT2 WH;										// ���ƍ���

protected:


public:
	FIELD();
	//==============================
	// position : ���S���W
	// wh : ���ƍ���
	//==============================
	FIELD(XMFLOAT3 position, XMFLOAT2 wh);
	~FIELD();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void SetPosition(const XMFLOAT3 position);					// �|�W�V�����̐ݒ�
	void SetWH(const XMFLOAT2 wh);								// ���ƍ����̐ݒ�
	void SetTexture(const string& const file_name);				// �e�N�X�`���̐ݒ�

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Texture);
	}

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

CEREAL_REGISTER_TYPE(FIELD)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, FIELD)

#endif // !FIELD_H