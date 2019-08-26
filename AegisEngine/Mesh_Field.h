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
	XMFLOAT3		GridSize;
	XMINT2			GridNum;
	unsigned int	IndexNum;
	VERTEX_3D*		VertexArray;

	ID3D11Buffer*	VertexBuffer;		// ���_�o�b�t�@
	ID3D11Buffer*	IndexBuffer;		// �C���f�b�N�X�o�b�t�@
	TEXTURE*		Texture;			// �e�N�X�`��

public:
	MESH_FIELD();
	~MESH_FIELD() { Uninit(); };

	void Init();
	void Draw();
	void Update();
	void Uninit();

	const float Get_Height(const XMFLOAT3& position);

	void SetTexture(const string& const file_name);				// �e�N�X�`���̐ݒ�
};


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

	void Init();
	void Draw();
	void Update();
	void Uninit();

	const float Get_Height(const XMFLOAT3& position);
};

#endif // !MESH_FIELF_H