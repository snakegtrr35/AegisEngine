#pragma once

#ifndef  SPRITE_H
#define SPRITE_H

#include	"Game_Object.h"
#include	"Menu_Component.h"
#include	"Renderer.h"
#include	"main.h"

class TEXTURE;

typedef struct {
	//! �q�X�v���C�g
	SPRITE* Child = nullptr;

	//! �e�X�v���C�g
	SPRITE* Parent = nullptr;

	//! �q�X�v���C�g�̖��O
	string Name;

}CHILD_DATE;

//==============================
// �X�v���C�g�N���X
//==============================
class SPRITE : public GAME_OBJECT {
private:

protected:
	//! ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! ���_�f�[�^
	VERTEX_3D Vertex[4];

	//! �e�N�X�`��
	TEXTURE* Texture;

	//! �|�W�V����
	XMFLOAT2 Position;

	// �T�C�Y
	// x : Top
	// y : Right
	// z : Bottom
	// w : Left
	XMFLOAT4 Size;

	COLOR Color;

	//! �q�X�v���C�g�̃��X�g
	list<CHILD_DATE> Children;

	//! ���j���[�C�x���g�̃��X�g
	list<MENU_COMPONENT*> MenuEvents;

	//! �X�v���C�g�̗L���t���O
	bool Enable = true;

	// �C���f�b�N�X�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	ID3D11ShaderResourceView* ShaderResourceView = nullptr;

	void Draw_Child() {

		for (CHILD_DATE child : Children)
		{
			if (child.Child->Enable)
			{
				child.Child->Draw();

				child.Child->Draw_Child();
			}
		}
	};

public:
	SPRITE();
	//==============================
	// position : ���S���W
	// size : �T�C�Y
	//==============================
	SPRITE(XMFLOAT2 position, XMFLOAT4 size);
	~SPRITE();

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;

	void SetPosition(const XMFLOAT2& position);
	XMFLOAT2* const GetPosition();

	void SetEnable(const bool flag);
	const bool GetEnable();

	void SetColor(COLOR& const color);
	COLOR& const GetColor();

	void SetSize(const XMFLOAT4& size);							// �T�C�Y�̐ݒ�
	XMFLOAT4* const GetSize();

	void SetTexture(const string& const file_name);				// �e�N�X�`���̐ݒ�
	TEXTURE* const GetTexture();								// �e�N�X�`���̎擾

	// ���X�g�ւ̎q�X�v���C�g�̒ǉ�
	SPRITE* Add_Child_Sptite(const string& name, SPRITE* const parent)
	{
		SPRITE* sprite = new SPRITE();

		sprite->Enable = parent->GetEnable();


		CHILD_DATE child;

		child.Child = sprite;

		child.Name = name;

		child.Parent = parent;

		Children.push_back(child);

		return child.Child;
	}

	// �q�X�v���C�g�̃��X�g�̎擾
	list<CHILD_DATE> const Get_Child_Sptite()
	{
		return Children;
	}

	// ���X�g�ւ̃��j���[�C�x���g�̒ǉ�
	template <typename T>
	T* Add_Menu_Event()
	{
		for (auto object : MenuEvents)
		{
			if (typeid(T) == typeid(*object))
			{
				return (T*)object;
			}

			T* component = new T();

			component->SetEnable(true);

			MenuEvents.push_back(component);

			return component;
		}
	}

	// ���X�g�̃��j���[�C�x���g�̎擾
	template <typename T>
	T* Get_Menu_Event()
	{
		for (auto component : MenuEvents)
		{
			if (typeid(T) == typeid(*component))
			{
				return (T*)component;
			}
		}
	}

	// ���X�g�̃��j���[�C�x���g�̗L��������
	template <typename T>
	void Menu_Event_Enable(const bool const flag)
	{
		for (auto component : MenuEvents)
		{
			if (typeid(T) == typeid(*component))
			{
				component->SetEnable(flag);
				return;
			}
		}
	}

	void Set_Position_Child(const string& const name, const XMFLOAT2 position) {

		for (CHILD_DATE child : Children)
		{
			if (name == child.Name)
			{
				XMFLOAT2 pos( child.Parent->GetPosition()->x + position.x, child.Parent->GetPosition()->y + position.y );

				child.Child->SetPosition(pos);
				return;
			}
		}
	};

	void Set_Enable_Child(const bool flag) {

		for (CHILD_DATE child : Children)
		{
			child.Child->Enable = flag;
		}
	};

	void Set_Enable_Child(const string& const name, const bool flag) {

		for (CHILD_DATE child : Children)
		{
			if (name == child.Name)
			{
				child.Child->Enable = flag;
				return;
			}
		}
	};

	const bool Get_Enable_Child(const string& const name) {

		for (CHILD_DATE child : Children)
		{
			if (name == child.Name)
			{
				return child.Child->Enable;
			}
		}
	};
	
	void Set(ID3D11ShaderResourceView* shader_resource_view) {
		ShaderResourceView = shader_resource_view;
	};
};

//////////////////////////////////////////////////

//==============================
// �X�v���C�g�A�j���[�V�����N���X
//==============================
class SPRITE_ANIMATION : public SPRITE {
private:
	float WaitFrame;
	unsigned short CreatCount;
	unsigned short PatternCount;
	unsigned short Age;

	static int FrameCount;

	float Tx, Ty;	// �e�N�X�`���؂�����W
	float Tw, Th;	// �e�N�X�`���̐؂��蕝�ƍ���

	unsigned char  Pattern_Max_X;
	unsigned char  Pattern_Max_Y;

	void Draw2(float tx = -1.0f, float ty = -1.0f);

protected:


public:
	float Tx_Param, Ty_Param;	// �e�N�X�`���؂�����W�i�蓮�j

	SPRITE_ANIMATION();
	//==============================
	// position : ���S���W
	// size : �T�C�Y
	//==============================
	SPRITE_ANIMATION(XMFLOAT2 position, XMFLOAT4 size);
	~SPRITE_ANIMATION();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	// �e�N�X�`���A�j���[�V�����̃p�����[�^�[�̐ݒ�
	//
	// ����:wait_frame ... �҂��t���[��
	//      x ... �e�N�X�`���؂��蕝
	//      y ... �e�N�X�`���؂��荂��
	void SetParam(const float& wait_frame, const unsigned char& x, const unsigned char& y);

	// �e�N�X�`���A�j���[�V�����̃e�N�X�`���؂�����W�̐ݒ�
	//
	// ����:tx_param ... �e�N�X�`���؂��蕝�i�蓮�j
	//      ty_param ... �e�N�X�`���؂��荂���i�蓮�j
	void Set_Param_Txy(const float& tx_param = -1.0f, const float& ty_param = -1.0f);

	const XMFLOAT2& Get_Twh() {
		return XMFLOAT2(Tw, Th);
	};
};

/**
* @brief �e�L�X�g�N���X
* @details ��������������߂̃N���X
* @details �X�v���C�g�N���X�̔h���N���X
*/
class TEXTS :public  SPRITE {
private:
	string Text;

	void Text_Draw(const string& text);

protected:


public:
	TEXTS();
	~TEXTS();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void Edit(const string& text);
};

#endif // ! SPRITE_H