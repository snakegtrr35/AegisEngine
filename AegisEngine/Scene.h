#pragma once

#ifndef SCENE_H
#define SCENE_H

#include	"Game_Object.h"
#include	"Light.h"


template <typename T>
bool Empty_weak_ptr(const weak_ptr<T>& w)
{
	bool flag = (!w.owner_before(weak_ptr<T>{})) && (!weak_ptr<T>{}.owner_before(w));

	//if (nullptr == w.get())
	//{
	//	flag = false;
	//}

	return flag;
}

enum class LAYER_NAME {

	BACKGROUND = 0,		//! �w�i�̂��߂̃��C���[
	GAMEOBJECT,			//! �Q�[���I�u�W�F�N�g�̂��߂̃��C���[
	EFFECT,				//! �G�t�F�N�g�̂��߂̃��C���[
	//DEBUG,			//! �f�o�b�O�W���̂��߂̃��C���[
	UI,					//! UI�̂��߂̃��C���[
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

protected:
	static list<shared_ptr<GAME_OBJECT>> GameObjects[(int)LAYER_NAME::MAX_LAYER];

	static bool PauseEnable;

	static LIGHTS Light_Manager;

	static std::mutex Load_Mutex;

	static bool Load_Enable;

public:

	//// ���X�g�ւ̒ǉ�
	template <typename T>
	static T* Add_Game_Object(LAYER_NAME layer, const string& name)
	{
		//T* object = new T();
		shared_ptr<T> object(new T());

		object->Set_Object_Name(name);

		GameObjects[(int)layer].emplace_back(object);

		return object.get();
	}

	// ���X�g�������̖��O�̃I�u�W�F�N�g�̎擾
	template <typename T>
	static weak_ptr<T> Get_Game_Object(const string& name = "none")
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object : GameObjects[i])
			{
				if (!GameObjects[i].empty())
				{
					if (typeid(T) == typeid(*object.get()))
					{
						if (name == object.get()->Get_Object_Name())
						{
							weak_ptr<T> obj(static_pointer_cast<T>(object));

							return  obj;
						}
					}
				}
				else
				{
					continue;
				}
			}
		}

		weak_ptr<T> obj;
		return obj;
	}

	// ���X�g�������̃I�u�W�F�N�̎擾
	// ���� name �I�u�W�F�N�g��
	static GAME_OBJECT* Get_Game_Object(const string& name)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					if (name == object->get()->Get_Object_Name())
					{
						return object->get();
					}
				}
				else
				{
					continue;
				}
			}
		}
		return nullptr;
	}

	// ���X�g�������̃I�u�W�F�N�̎擾
	// ���� name �I�u�W�F�N�g��

	static weak_ptr<GAME_OBJECT> Get_Game_Object(const GAME_OBJECT* me)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					if (me == object->get())
					{
						weak_ptr<GAME_OBJECT> obj(*object);

						return  obj;
					}
				}
				else
				{
					continue;
				}
			}
		}
		weak_ptr<GAME_OBJECT> obj;
		return obj;
	}

	// ���X�g�������̃I�u�W�F�N�g�i�����j�̎擾
	template <typename T>
	static vector<T*> Get_Game_Objects()
	{
		vector<T*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					if (typeid(T) == typeid(*object->get()))
					{
						objects.emplace_back((T*)object->get());
					}
				}
				else
				{
					continue;
				}
			}
		}
		return objects;
	}

	// �S�I�u�W�F�N�g�̎擾
	static vector<GAME_OBJECT*> Get_All_Game_Object()
	{
		vector<GAME_OBJECT*> objects;
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (!GameObjects[i].empty())
				{
					objects.push_back(object->get());
				}
				else
				{
					continue;
				}
			}
		}
		return objects;
	}

	// �S�I�u�W�F�N�g�̎擾
	template <typename T>
	static void Delete_Game_Objects()
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			if (!GameObjects[i].empty())
			{
				for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
				{
					if (typeid(T) == typeid(*object->get()))
					{
						object->get()->Set_Destroy();
					}
				}
			}
		}

		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // ���X�g����폜
		}
	}

	SCENE();
	virtual ~SCENE() {
		Uninit();
	}

	/**
	* @brief �ȒP�Ȑ����i�`����֐��j
	* @param[in] a(������) �����̐���
	* @param[out] b(������) �����̐���
	* @return bool �߂�l�̐���
	* @details �ڍׂȐ���
	*/
	virtual void Init(void) {
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->get()->Init();
			}
			Light_Manager.Init();
		}

		End_Pause();
	};

	/**
	* @brief �ȒP�Ȑ����i�`����֐��j
	* @param[in] a(������) �����̐���
	* @param[out] b(������) �����̐���
	* @return bool �߂�l�̐���
	* @details �ڍׂȐ���
	*/
	virtual void Draw(void) {
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->get()->Draw();
			}
		}
	};

	/**
	* @brief �`��(Depth-pre-pass)�֐�
	* @details Depth-pre-pass������֐�
	*/
	virtual void Draw_DPP(void) {
		//for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		//{
		//	for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
		//	{
		//		object->get()->Draw_DPP();
		//	}
		//}

		for (int i = (int)LAYER_NAME::MAX_LAYER - 1; (int)LAYER_NAME::BACKGROUND <= i; i--)
		{
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->get()->Draw_DPP();
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
	virtual void Update(float delta_time) {
		if (true == PauseEnable)	// �|�[�Y��
		{
			for (auto object = GameObjects[(int)LAYER_NAME::UI].begin(); object != GameObjects[(int)LAYER_NAME::UI].end(); object++)
			{
					object->get()->Update(delta_time);
			}

			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // ���X�g����폜
			}
		}
		else	// �|�[�Y���ł͂Ȃ�
		{
			for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
			{
				//for (GAME_OBJECT* object : GameObjects[i])
				for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
				{
					object->get()->Update(delta_time);
				}

				GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // ���X�g����폜
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
			for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				object->reset();
			}
			GameObjects[i].clear();
		}
		Light_Manager.Uninit();
	};

	/**
	* @brief ���[�h�I����m�点��֐�
	* @details ���[�h�I����m�点��t���O���I������
	*/
	static void SetLockLoad();

	/**
	* @brief ���[�h�����𔻒肷��֐�
	* @return bool ���[�h�����𔻒肷��t���O( ���[�h���Ȃ� true )
	* @details ���[�h�����𔻒肷��t���O���擾����֐�
	*/
	static bool GetLockLoad();

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
		return (UINT)GameObjects[(int)layer].size();
	}

	/**
	* @brief �I�u�W�F�N�g�̌����擾����֐�
	* @details ���背�C���[�̃I�u�W�F�N�g�̌���Ԃ�
	* @return UINT �I�u�W�F�N�g�̌�
	*/
	static LIGHTS* Get_Light_Manager() {
		return &Light_Manager;
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
		{
			if(!GameObjects[i].empty()) GameObjects[i].remove_if([](auto& object) { return object->Destroy(); }); // ���X�g����폜
		}

		ar(GameObjects);
		ar(Light_Manager);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(GameObjects);
		ar(Light_Manager);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(GameObjects);
		ar(Light_Manager);
	}*/
};

#endif // !SCENE_H