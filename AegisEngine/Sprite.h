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
struct CHILD_DATE {
	//! �q�X�v���C�g�̃|�C���^
	unique_ptr<SPRITE> Child;

	////! �I�t�Z�b�g�ʒu
	//XMFLOAT3 Offset;

	//! �q�X�v���C�g�̖��O
	string Name;

	CHILD_DATE() : /*Offset(XMFLOAT3(0.f, 0.f, 0.f)),*/ Name("") {
		Child.reset(nullptr);
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(Child);
		//ar(Offset);
		ar(Name);
	}

};

/**
* @brief �X�v���C�g�N���X
* @details �X�v���C�g�N���X
*/
class SPRITE : public GAME_OBJECT {
private:
protected:
	//! ���_�o�b�t�@
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer;

	//! �C���f�b�N�X�o�b�t�@
	static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;

	//! ���_�f�[�^
	VERTEX_3D Vertex[4];
	//! �e�N�X�`��
	unique_ptr<TEXTURE> Texture;

	//! �|�W�V����
	XMFLOAT2 Position;

	/**
	* �T�C�Y
	* x : ��
	* y : �E
	* z : ��
	* w : ��
	*/
	XMFLOAT4 Size;

	//!< �J���[
	COLOR Color;//!< �q�X�v���C�g�̃��X�g

	//!< �q�X�v���C�g�̃��X�g
	vector< unique_ptr<CHILD_DATE> > Children;

	//!< ���j���[�C�x���g(���X�g)
	list<MENU_COMPONENT*> MenuEvents;

	//!< �X�v���C�g�̗L�������t���O(�f�t�H���g�� true )
	bool Enable;


	ID3D11ShaderResourceView* ShaderResourceView = nullptr;

	/**
	* @brief �q�X�v���C�g�̕`��֐�
	* @details �q�X�v���C�g�̕`�悷��֐�
	*/
	void Draw_Child();

	/**
	* @brief �q�X�v���C�g�̕`��֐�
	* @details �q�X�v���C�g�̕`�悷��֐�
	*/
	void Draw_DPP_Child();

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
	* @brief �`��(Depth-pre-pass)�֐�
	* @details Depth-pre-pass������֐�
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
	SPRITE* Add_Child_Sptite(const string& name);

	/**
	* @brief �q�X�v���C�g���擾����֐�
	* @return list<CHILD_DATE> �q�X�v���C�g�̃��X�g
	* @details �q�X�v���C�g�̃��X�g���擾����֐�
	*/
	vector< unique_ptr<CHILD_DATE> >* const Get_Child_Sptite();

	/**
	* @brief �q�X�v���C�g���擾����֐�
	* @return list<CHILD_DATE> �q�X�v���C�g�̃��X�g
	* @details �q�X�v���C�g�̃��X�g���擾����֐�
	*/
	CHILD_DATE* const Get_Child_Sptite(const string& name);

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
	* @brief �q�X�v���C�g�̍��W��ݒ肷��֐�
	* @param name �ݒ肷��q�X�v���C�g�̖��O
	* @param position �e�̍��W(�񎟌����W)
	* @param offset �I�t�Z�b�g�ʒu(�񎟌����W)
	* @details ����̎q�X�v���C�g�̍��W(�񎟌����W�A�e�X�v���C�g����I�t�Z�b�g�����)��ݒ肷��֐�
	*/
	void Set_Position_Child(const string& const name, const XMFLOAT2& position, const XMFLOAT2& offset);

	/**
	* @brief �q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	* @param flag �`��̗L�������̃t���O
	* @details �S�Ă̎q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	*/
	void Set_Enable_Child(const bool flag);

	/**
	* @brief �q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	* @param name �ݒ肷��q�X�v���C�g�̖��O
	* @param flag �`��̗L�������̃t���O
	* @details ����̎q�X�v���C�g�̕`��̗L��������ݒ肷��֐�
	*/
	void Set_Enable_Child(const string& const name, const bool flag);

	/**
	* @brief �q�X�v���C�g�̕`��̗L���������擾����֐�
	* @param name �q�X�v���C�g�̖��O
	* @return bool �`��̗L�������̃t���O
	* @details ����̎q�X�v���C�g�̕`��̗L���������擾����֐�
	*/
	const bool Get_Enable_Child(const string& const name, vector< unique_ptr<CHILD_DATE> >* const children);
	
	void Set(ID3D11ShaderResourceView* shader_resource_view) {
		ShaderResourceView = shader_resource_view;
	};

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

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(Children);
		ar(Texture);
		ar(SPRITE::Position);
		ar(Size);
		//ar(Color);
		ar(Enable);
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

#endif // ! SPRITE_H