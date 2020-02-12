#pragma once

#ifndef MESH_FIELF_H
#define MESH_FIELF_H

#include	"Game_Object.h"

class TEXTURE;

//==============================
//  �t�B�[���h�N���X
//==============================
class MESH_FIELD :public GAME_OBJECT {
private:
	XMFLOAT3				GridSize;
	XMINT2					GridNum;
	unsigned int			IndexNum;
	VERTEX_3D*				VertexArray;

	ID3D11Buffer*			VertexBuffer;		// ���_�o�b�t�@
	ID3D11Buffer*			IndexBuffer;		// �C���f�b�N�X�o�b�t�@
	unique_ptr<TEXTURE>		Texture;			// �e�N�X�`��

public:
	MESH_FIELD();
	~MESH_FIELD() { Uninit(); }

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	const float Get_Height(const XMFLOAT3& position);

	void SetTexture(const string& const file_name);				// �e�N�X�`���̐ݒ�

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
//  �t�B�[���h�N���X
//==============================
class MESH_WALL :public GAME_OBJECT {
private:
	XMFLOAT3		GridSize;
	XMINT2			GridNum;
	unsigned int	IndexNum;
	VERTEX_3D* VertexArray;

	ID3D11Buffer* VertexBuffer;		// ���_�o�b�t�@
	ID3D11Buffer* IndexBuffer;		// �C���f�b�N�X�o�b�t�@
	TEXTURE* Texture;				// �e�N�X�`��

public:
	MESH_WALL();
	~MESH_WALL() { Uninit(); };

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	const float Get_Height(const XMFLOAT3& position);
};

#endif // !MESH_FIELF_H