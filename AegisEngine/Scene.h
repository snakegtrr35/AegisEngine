#pragma once

#ifndef SCENE_H
#define SCENE_H

#include	"main.h"
#include	"Renderer.h"

#include	"camera.h"
#include	"Field.h"
#include	"Player.h"
#include	"Enemy.h"
#include	"model.h"
#include	"Sprite.h"
#include	"Polygon_3D.h"
#include	"Billboard.h"
#include	"Score.h"
#include	"Circle_Shadow.h"

#include	"Grid.h"
#include	"XYZ_Axis.h"
#include	"Bounding.h"

#include	"Mesh_Field.h"
#include	"Mesh_Cylinder.h"
#include	"Mesh_Dome.h"

class GAME_OBJECT;

enum class LAYER_NAME {

	BACKGROUND = 0,
	GAMEOBJECT,
	EFFECT,
	UI,
	MAX_LAYER
};

enum class SCENE_INDEX {
	TITLE = 0,
	GAME,
	RESULT,
	MAX_INDEX
};

/**
* @brief �V�[���N���X
* @details �V�[���̃N���X�̊��N���X
*/
class SCENE {
private:

	friend class GAME_OBJECT;

protected:
	static list<GAME_OBJECT*> GameObjects[(int)LAYER_NAME::MAX_LAYER];

	static bool PauseEnable;

public:

	// ���X�g�ւ̒ǉ�
	template <typename T>
	static T* Add_Game_Object(LAYER_NAME layer)
	{
		T* object = new T();

		object->Init();

		GameObjects[(int)layer].push_back(object);

		return object;
	}

	// ���X�g�ւ̒ǉ�
	template <typename T>
	static T* Add_Game_Object(LAYER_NAME layer, const string& name)
	{
		T* object = new T();

		object->Init();
			
		object->Set_Object_Name(name);

		GameObjects[(int)layer].push_back(object);

		return object;
	}

	// ���X�g�������̃I�u�W�F�N�g�̎擾
	template <typename T>
	static T* Get_Game_Object()
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	// ���X�g�������̖��O�̃I�u�W�F�N�g�̎擾
	template <typename T>
	static T* Get_Game_Object(const string& name)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					if (name == object->Get_Object_Name())
					{
						return (T*)object;
					}
				}
			}
		}
		return nullptr;
	}

	// ���X�g�������̃I�u�W�F�N�g�i�����j�̎擾
	template <typename T>
	static vector<T*> Get_Game_Objects()
	{
		vector<T*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}

	// ���X�g�������̃I�u�W�F�N�g�i�����j�̎擾
	// ���� T* �^�� name �I�u�W�F�N�g��
	template <typename T>
	static vector<T*> Get_Game_Objects(const string& name)
	{
		vector<T*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (typeid(T) == typeid(*object))
				{
					if (name == object->Get_Object_Name())
					{
						objects.push_back((T*)object);
					}
				}
			}
		}
		return objects;
	}

	// ���X�g�������̃I�u�W�F�N�g�i�����j�̎擾
	// ���� name �I�u�W�F�N�g��
	static GAME_OBJECT* Get_Game_Object(const string& name)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				if (name == object->Get_Object_Name())
				{
					return object;
				}
			}
		}
		return nullptr;
	}

	// �S�I�u�W�F�N�g�̎擾
	static vector<GAME_OBJECT*> Get_All_Game_Object()
	{
		vector<GAME_OBJECT*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				objects.push_back(object);
			}
		}
		return objects;
	}

	SCENE() {};
	virtual ~SCENE() {
		Uninit();
	};

	/**
	* @brief �ȒP�Ȑ����i�`����֐��j
	* @param[in] a(������) �����̐���
	* @param[out] b(������) �����̐���
	* @return bool �߂�l�̐���
	* @details �ڍׂȐ���
	*/
	virtual void Init(void) {
	};

	/**
	* @brief �ȒP�Ȑ����i�`����֐��j
	* @param[in] a(������) �����̐���
	* @param[out] b(������) �����̐���
	* @return bool �߂�l�̐���
	* @details �ڍׂȐ���
	*/
	virtual void Draw(void) {
	};

	/**
	* @brief �ȒP�Ȑ����i�`����֐��j
	* @param[in] a(������) �����̐���
	* @param[out] b(������) �����̐���
	* @return bool �߂�l�̐���
	* @details �ڍׂȐ���
	*/
	virtual void Update(void) {
		if (true == PauseEnable)	// �|�[�Y��
		{
			for (GAME_OBJECT* object : GameObjects[(int)LAYER_NAME::UI])
			{
					object->Update();
			}

			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				GameObjects[i].remove_if([](GAME_OBJECT* object) { return object->Destroy(); }); // ���X�g����폜
			}
		}
		else	// �|�[�Y���ł͂Ȃ�
		{
			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				for (GAME_OBJECT* object : GameObjects[i])
				{
					object->Update();
				}

				GameObjects[i].remove_if([](GAME_OBJECT* object) { return object->Destroy(); }); // ���X�g����폜
			}
		}
	};

	/**
	* @brief �ȒP�Ȑ����i�`����֐��j
	* @param[in] a(������) �����̐���
	* @param[out] b(������) �����̐���
	* @return bool �߂�l�̐���
	* @details �ڍׂȐ���
	*/
	virtual void Uninit(void) {
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (GAME_OBJECT* object : GameObjects[i])
			{
				object->Uninit();
				SAFE_DELETE(object);
			}
			GameObjects[i].clear();
		}
	};

	/**
	* @brief �ȒP�Ȑ����i�`����֐��j
	* @param[in] a(������) �����̐���
	* @param[out] b(������) �����̐���
	* @return bool �߂�l�̐���
	* @details �ڍׂȐ���
	*/
	static void Destroy_Game_Object(GAME_OBJECT* game_object) {
		game_object->Set_Destroy();
	};

	/**
	* @brief �|�[�Y�����s����֐�
	* @details UI����Update���s���܂�
	* @details Draw�͑S�čs���܂�
	*/
	static void Start_Pause() {
		PauseEnable = true;
	};

	/**
	* @brief �|�[�Y���I������֐�
	* @details �|�[�Y���I������
	*/
	static void End_Pause() {
		PauseEnable = false;
	};

	/**
	* @brief �|�[�Y���L������������Ԃ��֐�
	* @details �|�[�Y�ϐ���Ԃ�
	* @return bool �|�[�Y�ϐ�
	*/
	static const bool Get_PauseEnable() {
		return PauseEnable;
	};

	/**
	* @brief �I�u�W�F�N�g�̌����擾����֐�
	* @details ���背�C���[�̃I�u�W�F�N�g�̌���Ԃ�
	* @return UINT �I�u�W�F�N�g�̌�
	*/
	static const UINT Get_Game_Object_Count(const LAYER_NAME layer) {
		return GameObjects[(int)layer].size();
	}
};

/**
* @brief �^�C�g���V�[���N���X
* @details �V�[���N���X�̔h���N���X
*/
class TITLE : public SCENE {
private:

protected:

public:
	TITLE() {};
	~TITLE() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;
};

/**
* @brief �Q�[���V�[���N���X
* @details �V�[���N���X�̔h���N���X
*/
class GAME : public SCENE {
private:

protected:

public:
	GAME() {};
	~GAME() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;
};

/**
* @brief ���U���g�V�[���N���X
* @details �V�[���N���X�̔h���N���X
*/
class RESULT : public SCENE {
private:

protected:

public:
	RESULT() {};
	~RESULT() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;

	static bool Clear_Flag;
};

/**
* @brief ���C�����j���[�V�[���N���X
* @details �V�[���N���X�̔h���N���X
* @details �^�C�g���ƃQ�[���̊Ԃ̃V�[��
*/
class MAIN_MENU : public SCENE {
private:

protected:

public:
	MAIN_MENU() {};
	~MAIN_MENU() {
		Uninit();
	};

	void Init() override;
	void Draw() override;
	void Update() override;
	void Uninit() override;

	static string Model_Name;
};



/**
* @brief �V�[���Ǘ��N���X
* @details �V�[���N���X���Ǘ����邽�߂̃N���X
*/
class SCENE_MANAGER {
private:
	static SCENE* pScene;

protected:

public:
	SCENE_MANAGER() {
		if (nullptr == pScene)
			pScene = new SCENE();
	};

	~SCENE_MANAGER() {
		Uninit();
	};

	void Init() {
		pScene->Init();
	};

	void Draw() {
		pScene->Draw();
	};

	void Update() {
		pScene->Update();
	};

	void Uninit() {
		pScene->Uninit();
		SAFE_DELETE(pScene);
	};

	static SCENE* const Get_Scene() {
		return pScene;
	};

	template <typename T>
	static void Set_Scene() {
		T* scene = new T();

		SAFE_DELETE(pScene);

		pScene = scene;

		pScene->Init();
	};
};

#endif // !SCENE_H