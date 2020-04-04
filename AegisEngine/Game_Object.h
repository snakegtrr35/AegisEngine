/**
 * @file Game_Object.h
 * @brief Game_Object�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include	"Component_Manager.h"

 /**
 * @brief �Q�[���I�u�W�F�N�g�N���X
 * @details �Q�[���I�u�W�F�N�g�N���X
 */
class GAME_OBJECT {
private:
	//! �Q�[���I�u�W�F�N�g�̖��̈ꗗ(�ÓI�Ȃ���)
	static unordered_set<string> Object_Name_Map;

protected:

	//! �I�u�W�F�N�g�̖��O
	string Object_Name;

	//! �폜���邩�̃t���O
	bool DestroyFlag;

	//! ���W
	XMFLOAT3 Position;
	
	//! ��]��
	XMFLOAT3 Rotation;

	//! �g��k���l
	XMFLOAT3 Scaling;

	//! �R���|�[�l���g
	unique_ptr<COMPONENT_MANEGER, Delete> Component;

	////! �N�I�[�^�j�I��
	//XMVECTOR Quaternion;
	////! �f�o�b�O�p�̃N�I�[�^�j�I��
	//XMVECTOR Edit_Quaternion;
	////! �N�I�[�^�j�I���p�̓��ꕨ
	//XMFLOAT4 Q_num;

#ifdef _DEBUG
	//XMFLOAT3 Edit_Position;				//! �f�o�b�O�p�̍��W
	//XMFLOAT3 Edit_Rotation;				//! �f�o�b�O�p�̉�]��
	//XMFLOAT3 Edit_Scaling;				//! �f�o�b�O�p�̊g��k���l

	//XMFLOAT4 Edit_Q_num;				//! �f�o�b�O�p�̃N�I�[�^�j�I���p�̓��ꕨ
#endif // _DEBUG

public:
	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	GAME_OBJECT();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	virtual ~GAME_OBJECT();

	/**
	* @brief �������֐�
	* @details ���z�������֐�
	*/
	virtual void Init();

	/**
	* @brief �`��֐�
	* @details ���z�`��֐�
	*/
	virtual void Draw();

	/**
	* @brief �`��(Depth-pre-pass)�֐�
	* @details �������zDepth-pre-pass�֐�
	*/
	virtual void Draw_DPP() = 0;

	/**
	* @brief �X�V�֐�
	* @details ���z�X�V�֐�
	*/
	virtual void Update(float delta_time);

	/**
	* @brief �I�������֐�
	* @details �������z�I�������֐�
	*/
	virtual void Uninit() = 0;

	/**
	* @brief �폜�v���֐�
	* @details �폜�����Ă��炤�ׂ̊֐�(�폜�t���O�𗧂Ă�)
	*/
	void Set_Destroy();

	/**
	* @brief �폜����֐�
	* @return bool �폜����̃t���O
	* @details �폜�����邩���f����֐�(�蓮�ł͌Ă΂Ȃ��ŉ�����)
	*/
	bool Destroy();

	/**
	* @brief ���O���擾����֐�
	* @return string �I�u�W�F�N�g�̖��O
	* @details �I�u�W�F�N�g�̖��O���擾����֐�
	*/
	const string& Get_Object_Name() {
		return Object_Name;
	};

	/**
	* @brief ���O��ݒ肷��֐�
	* @param name ���O(string)
	* @details �I�u�W�F�N�g�̖��O��ݒ肷��֐�
	*/
	void Set_Object_Name(const string& name);

	/**
	* @brief ���W���擾����֐�
	* @return XMFLOAT3* ���W(XMFLOAT3*)
	* @details�@�I�u�W�F�N�g�̍��W���擾����֐�
	*/
	XMFLOAT3* const Get_Position() {
		return &Position;
	};

	/**
	* @brief ��]���擾����֐�
	* @return XMFLOAT3* ��](XMFLOAT3*)
	* @details�@�I�u�W�F�N�g�̉�]���擾����֐�
	*/
	XMFLOAT3* const Get_Rotation() {
		return &Rotation;
	};

	/**
	* @brief �X�P�[�g���擾����֐�
	* @return XMFLOAT3* �X�P�[�g(XMFLOAT3*)
	* @details�@�I�u�W�F�N�g�̃X�P�[�g���擾����֐�
	*/
	XMFLOAT3* const Get_Scaling() {
		return &Scaling;
	};

	/**
	* @brief ���W��ݒ肷��֐�
	* @param position ���W(XMFLOAT3*)
	* @details �I�u�W�F�N�g�̍��W��ݒ肷��֐�
	*/
	void Set_Position(XMFLOAT3* position) {
		Position = *position;
	}

	/**
	* @brief ���W��ݒ肷��֐�
	* @param position ���W(XMFLOAT3)
	* @details �I�u�W�F�N�g�̍��W��ݒ肷��֐�
	*/
	void Set_Position(XMFLOAT3& position) {
		Position = position;
	};

	/**
	* @brief ��]��ݒ肷��֐�
	* @param position ��]��(XMFLOAT3*)
	* @details �I�u�W�F�N�g�̉�]��ݒ肷��֐�
	*/
	void Set_Rotation(XMFLOAT3* rotation) {
		Rotation = *rotation;
	};

	/**
	* @brief ��]��ݒ肷��֐�
	* @param position ��]��(XMFLOAT3)
	* @details �I�u�W�F�N�g�̉�]��ݒ肷��֐�
	*/
	void Set_Rotation(XMFLOAT3& rotation) {
		Rotation = rotation;
	};

	/**
	* @brief �X�P�[����ݒ肷��֐�
	* @param position �X�P�[��(XMFLOAT3*)
	* @details �I�u�W�F�N�g�̃X�P�[����ݒ肷��֐�
	*/
	void Set_Scaling(XMFLOAT3* scaling) {
		Scaling = *scaling;
	};

	/**
	* @brief �X�P�[����ݒ肷��֐�
	* @param position �X�P�[��(XMFLOAT3)
	* @details �I�u�W�F�N�g�̃X�P�[����ݒ肷��֐�
	*/
	void Set_Scaling(XMFLOAT3& scaling) {
		Scaling = scaling;
	};

	/**
	* @brief �R���|�[�l���g���擾����֐�
	* @return COMPONENT_MANEGER* ���W(COMPONENT_MANEGER*)
	* @details �I�u�W�F�N�g�̃R���|�[�l���g���擾����֐�
	*/
	COMPONENT_MANEGER* const Get_Component() {
		return Component.get();
	}

	/**
	* @brief �Q�[���I�u�W�F�N�g�l�[���}�b�v���擾����֐�
	* @return unordered_set<string> �Q�[���I�u�W�F�N�g�l�[���}�b�v(unordered_set<string>)
	* @details �Q�[���I�u�W�F�N�g�l�[���}�b�v���擾����֐�
	*/
	static const unordered_set<string>& Get_Object_Name_Map() {
		return Object_Name_Map;
	}

	template<class Archive>
	void serialize( Archive& ar)
	{
		ar(Object_Name);

		ar(Position);

		ar(Rotation);

		ar(Scaling);

		Set_Object_Name(Object_Name);

		ar(Component);
	}

	//template<class Archive>
	//void save(Archive& ar) const
	//{
	//	ar(Object_Name);

	//	ar(Position);

	//	ar(Rotation);

	//	ar(Scaling);
	//}

	//template<class Archive>
	//void load(Archive& ar)
	//{
	//	ar(Object_Name);

	//	ar(Position);

	//	ar(Rotation);

	//	ar(Scaling);

	//	Set_Object_Name(Object_Name);
	//}
};

#endif // !GAME_OBJECT_H