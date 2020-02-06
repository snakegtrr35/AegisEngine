/**
 * @file Sprite.h
 * @brief �X�v���C�g�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef  SPRITE_H
#define SPRITE_H

#include	"Game_Object.h"
#include	"texture.h"
#include	"Menu_Component.h"
#include	"Renderer.h"


/**
 * �q�X�v���C�g���
 */
typedef struct {
	//! �q�X�v���C�g�̃|�C���^
	SPRITE* Child = nullptr;

	//! �e�X�v���C�g�̃|�C���^
	SPRITE* Parent = nullptr;

	//! �q�X�v���C�g�̖��O
	string Name;

}CHILD_DATE;

/**
* @brief �X�v���C�g�N���X
* @details �X�v���C�g�N���X
*/
class SPRITE : public GAME_OBJECT {
private:

protected:
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		//!< ���_�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;		//!< �C���f�b�N�X�o�b�t�@

	VERTEX_3D Vertex[4];		//!< ���_�f�[�^

	unique_ptr<TEXTURE> Texture;		//!< �e�N�X�`��

	XMFLOAT2 Position;		//!< �|�W�V����

	/**
	* �T�C�Y
	* x : ��
	* y : �E
	* z : ��
	* w : ��
	*/
	XMFLOAT4 Size;

	COLOR Color;		//!< �J���[

	list<CHILD_DATE> Children;		//!< �q�X�v���C�g�̃��X�g

	list<MENU_COMPONENT*> MenuEvents;		//!< ���j���[�C�x���g(���X�g)

	bool Enable = true;		//!< �X�v���C�g�̗L�������t���O(�f�t�H���g�͗L��)

	ID3D11ShaderResourceView* ShaderResourceView = nullptr;

	/**
	* @brief �q�X�v���C�g�̕`��֐�
	* @details �q�X�v���C�g�̕`�悷��֐�
	*/
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

	/**
	* @brief �q�X�v���C�g�̕`��֐�
	* @details �q�X�v���C�g�̕`�悷��֐�
	*/
	void Draw_DPP_Child() {

		for (CHILD_DATE child : Children)
		{
			if (child.Child->Enable)
			{
				child.Child->Draw_DPP();

				child.Child->Draw_DPP_Child();
			}
		}
	};

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	SPRITE();

	/**
	* @brief �R���X�g���N�^
	* @param position ���S���W
	* @param size �T�C�Y
	* @details �����t���R���X�g���N�^
	*/
	SPRITE(XMFLOAT2 position, XMFLOAT4 size);

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~SPRITE();

	/**
	* @brief �������֐�
	* @details ����������֐�
	*/
	void Init();

	/**
	* @brief �`��֐�
	* @details �`�悷��֐�
	*/
	void Draw();

	/**
	* @brief �`��(Depth pre-pass)�֐�
	* @details Depth pre-pass���s���֐�
	*/
	void Draw_DPP();

	/**
	* @brief �X�V�֐�
	* @details �X�V����֐�
	*/
	void Update(float delta_time);

	/**
	* @brief �I�������֐�
	* @details �I������������֐�
	*/
	void Uninit();

	/**
	* @brief ���W��ݒ肷��֐�
	* @param position ���W(�񎟌����W)
	* @details ���W(�񎟌����W)��ݒ肷��֐�
	*/
	void SetPosition(const XMFLOAT2& position);

	/**
	* @brief ���W���擾����֐�
	* @return XMFLOAT2* ���W(�񎟌����W)(�|�C���^)
	* @details ���W(�񎟌����W)���擾����֐�
	*/
	XMFLOAT2* const GetPosition();

	/**
	* @brief �`��̗L��������ݒ肷��֐�
	* @param flag trur����false
	* @details �`��̗L��������ݒ肷��֐�
	*/
	void SetEnable(const bool flag);

	/**
	* @brief �`��̗L���������擾����֐�
	* @return bool �`��̗L�������̃t���O
	* @details �`��̗L���������擾����֐�
	*/
	const bool GetEnable();

	/**
	* @brief ���_�J���[��ݒ肷��֐�
	* @param color �J���[�\����
	* @details ���_�J���[(�J���[�\����)��ݒ肷��֐�
	*/
	void SetColor(COLOR& const color);

	/**
	* @brief ���_�J���[���擾����֐�
	* @return COLOR ���_�J���[(�J���[�\����)
	* @details ���_�J���[(�J���[�\����)���擾����֐�
	*/
	COLOR& const GetColor();

	/**
	* @brief �T�C�Y��ݒ肷��֐�
	* @param size x:�� y:�E z:�� w:��
	* @details �T�C�Y(4����)��ݒ肷��֐�
	*/
	void SetSize(const XMFLOAT4& size);

	/**
	* @brief �T�C�Y���擾����֐�
	* @return XMFLOAT4 �T�C�Y(4����)(x:�� y:�E z:�� w:��)
	* @details �T�C�Y(4����)�����擾����֐�
	*/
	XMFLOAT4* const GetSize();

	/**
	* @brief �e�N�X�`����ݒ肷��֐�
	* @param file_name �g�p����e�N�X�`����
	* @details �e�N�X�`��(�N���X)��ݒ肷��֐�
	*/
	void SetTexture(const string& const file_name);

	/**
	* @brief �e�N�X�`�����擾����֐�
	* @return TEXTURE �e�N�X�`���N���X
	* @details �e�N�X�`��(�N���X)���擾����֐�
	*/
	TEXTURE* const GetTexture();

	/**
	* @brief �q�X�v���C�g��ǉ�����֐�
	* @param name �q�X�v���C�g�̖��O
	* @param parent �e�X�v���C�g�̃|�C���^
	* @return SPRITE �q�X�v���C�g�̃|�C���^
	* @details �q�X�v���C�g����ǉ�����֐�
	*/
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

	/**
	* @brief �q�X�v���C�g���擾����֐�
	* @return list<CHILD_DATE> �q�X�v���C�g�̃��X�g
	* @details �q�X�v���C�g�̃��X�g���擾����֐�
	*/
	list<CHILD_DATE> const Get_Child_Sptite()
	{
		return Children;
	}

	/**
	* @brief ���j���[�C�x���g��ǉ�����֐�
	* @tparam T �ǉ����郁�j���[�C�x���g�̃N���X��
	* @return T �ǉ��������j���[�C�x���g�̃|�C���^
	* @details ���j���[�C�x���g����ǉ�����֐�
	*/
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

	/**
	* @brief ���j���[�C�x���g���擾����֐�
	* @tparam T �擾���������j���[�C�x���g�̃N���X��
	* @return T ���j���[�C�x���g
	* @details ���j���[�C�x���g���擾����֐�
	*/
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

	/**
	* @brief ���j���[�C�x���g��L����������֐�
	* @tparam T �L�����������������j���[�C�x���g�̃N���X��
	* @param flag trur����false
	* @details ����̃��j���[�C�x���g��L������������֐�
	*/
	template <typename T>
	void Menu_Event_Enable(const bool flag)
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

	/**
	* @brief �q�X�v���C�g�̍��W��ݒ肷��֐�
	* @param name �ݒ肷��q�X�v���C�g�̖��O
	* @param position ���W(�񎟌����W)
	* @details ����̎q�X�v���C�g�̍��W(�񎟌����W�A�e�X�v���C�g����I�t�Z�b�g�����)��ݒ肷��֐�
	*/
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

	/**
	* @brief �q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	* @param flag �`��̗L�������̃t���O
	* @details �S�Ă̎q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	*/
	void Set_Enable_Child(const bool flag) {

		for (CHILD_DATE child : Children)
		{
			child.Child->Enable = flag;
		}
	};

	/**
	* @brief �q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	* @param name �ݒ肷��q�X�v���C�g�̖��O
	* @param flag �`��̗L�������̃t���O
	* @details ����̎q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	*/
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

	/**
	* @brief �q�X�v���C�g�̕`��̗L���������擾����֐�
	* @param name �q�X�v���C�g�̖��O
	* @return bool �`��̗L�������̃t���O
	* @details ����̎q�X�v���C�g�̕`��̗L���������擾����֐�
	*/
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

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Texture);
		ar(SPRITE::Position);
		ar(Size);
		ar(Color);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Texture);
	//	ar(SPRITE::Position);
	//	ar(Size);
	//	ar(Color);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<GAME_OBJECT>(this));
	//	ar(Texture);
	//	ar(SPRITE::Position);
	//	ar(Size);
	//	ar(Color);
	//}
};

CEREAL_REGISTER_TYPE(SPRITE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, SPRITE)

////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* @brief �e�L�X�g�N���X
* @details ��������������߂̃N���X
* @details �X�v���C�g�N���X�̔h���N���X
*/
class TEXTS :public  SPRITE {
private:
	string Text;	//!< �\�����镶����

	/**
	* @brief �`��֐�
	* @details ���ۂɕ`�悷��֐�
	*/
	void Text_Draw(const string& text);

	/**
	* @brief �`��(Depth pre-pass)�֐�
	* @details Depth pre-pass���s���֐�
	*/
	void Text_Draw_DPP(const string& text);

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	TEXTS();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~TEXTS();

	/**
	* @brief �������֐�
	* @details ����������֐�
	*/
	void Init();

	/**
	* @brief �`��֐�
	* @details �`�悷��֐�
	*/
	void Draw();

	/**
	* @brief �`��(Depth pre-pass)�֐�
	* @details Depth pre-pass���s���֐�
	*/
	void Draw_DPP();

	/**
	* @brief �X�V�֐�
	* @details �X�V����֐�
	*/
	void Update(float delta_time);

	/**
	* @brief �I�������֐�
	* @details �I������������֐�
	*/
	void Uninit();

	/**
	* @brief �\�����镶�����ݒ肷��֐�
	* @param text �ݒ肷�镶����
	* @details �\�����镶�����ݒ肷��֐�
	*/
	void Edit(const string& text);

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<SPRITE>(this));
		ar(Text);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(Text);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(cereal::base_class<SPRITE>(this));
	//	ar(Text);
	//}
};

CEREAL_REGISTER_TYPE(TEXTS)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SPRITE, TEXTS)

#endif // ! SPRITE_H