//***********************************************************************************************
//
//  �X�J�C�h�[�� [skydome.h]
//
//***********************************************************************************************
#pragma once	// �C���N���[�h�K�[�h

#include	"Game_Object.h"

class TEXTURE;

//***********************************************************************************************
//  �X�J�C�h�[���N���X
//***********************************************************************************************
class MESH_DOOM :public GAME_OBJECT
{
private:
	float			Radius;
	unsigned int	VertexNum;
	unsigned int	IndexNum;

	unique_ptr<ID3D11Buffer, Release>	VertexBuffer;	// ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release>	IndexBuffer;	// �C���f�b�N�X�o�b�t�@
	unique_ptr<TEXTURE> Texture;						// �e�N�X�`��

public:
	MESH_DOOM() :Radius(50.0f) {};
	~MESH_DOOM() {};

	void Init();
	void Uninit();
	void Update();
	void Draw();

};
