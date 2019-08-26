#pragma once

#ifndef MESH_CYLINDER_H
#define MESH_CYLINDER_H

#include	"Game_Object.h"
#include	"texture.h"

class MESH_CYlLINDER : public GAME_OBJECT {
private:
	int				CylinderLength;
	float			Radius;
	unsigned int	VertexNum;
	unsigned int	IndexNum;

	unique_ptr<ID3D11Buffer, Release>	VertexBuffer;	// ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release>	IndexBuffer;	// �C���f�b�N�X�o�b�t�@
	unique_ptr<TEXTURE> Texture;						// �e�N�X�`��

public:
	MESH_CYlLINDER() : Radius(45.0f), CylinderLength(7){};
	~MESH_CYlLINDER() {};

	void Init();
	void Uninit();
	void Update();
	void Draw();

};

#endif // !MESH_CYLINDER_H