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

	//friend class GAME_OBJECT;

protected:
	static list<shared_ptr<GAME_OBJECT>> GameObjects[(int)LAYER_NAME::MAX_LAYER];

	static bool PauseEnable;

	static LIGHTS Light_Manager;

public:

	//// ���X�g�ւ̒ǉ�
	template <typename T>
	static T* Add_Game_Object(LAYER_NAME layer, const string& name = "none")
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
			/*for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
			{
				if (typeid(T) == typeid(*object->get()))
				{
					if (name == object->get()->Get_Object_Name())
					{
						return object->get();
					}
				}
			}*/

			for (auto object : GameObjects[i])
			{
				if ( typeid(T) == typeid( *object.get() ) )
				{
					if (name == object.get()->Get_Object_Name())
					{
						weak_ptr<T> obj( static_pointer_cast<T>(object) );

						return  obj;
					}
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
				if (name == object->get()->Get_Object_Name())
				{
					return object->get();
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
				if (me == object->get())
				{
					weak_ptr<GAME_OBJECT> obj( *object );

					return  obj;
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
				if (typeid(T) == typeid( *object->get()) )
				{
					objects.emplace_back( (T*)object->get() );
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
				objects.push_back(object->get());
			}
		}
		return objects;
	}

	SCENE() {}
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
		}
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
	* @brief �`��(Depth pre-pass)�֐�
	* @details Depth pre-pass���s���֐�
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


///**
//* @brief �^�C�g���V�[���N���X
//* @details �V�[���N���X�̔h���N���X
//*/
//class TITLE : public SCENE {
//private:
//
//protected:
//
//public:
//	TITLE() {}
//	~TITLE() {
//		Uninit();
//	}
//
//	void Init() override;
//	void Draw() override;
//	void Draw_DPP() override;
//	void Update(float delta_time) override;
//	void Uninit() override;
//
//	template<typename Archive>
//	void serialize(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//	/*template<class Archive>
//	void save(Archive& ar) const
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//	template<class Archive>
//	void load(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}*/
//};
//
//CEREAL_REGISTER_TYPE(TITLE)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, TITLE)

///**
//* @brief �Q�[���V�[���N���X
//* @details �V�[���N���X�̔h���N���X
//*/
//class GAME : public SCENE {
//private:
//
//protected:
//
//public:
//	GAME() {}
//	~GAME() {
//		Uninit();
//	}
//
//	void Init() override;
//	void Draw() override;
//	void Draw_DPP() override;
//	void Update(float delta_time) override;
//	void Uninit() override;
//
//	template<typename Archive>
//	void serialize(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//
//	/*template<class Archive>
//	void save(Archive& ar) const
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//	template<class Archive>
//	void load(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}*/
//};
//
//CEREAL_REGISTER_TYPE(GAME)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, GAME)

///**
//* @brief ���U���g�V�[���N���X
//* @details �V�[���N���X�̔h���N���X
//*/
//class RESULT : public SCENE {
//private:
//
//protected:
//
//public:
//	RESULT() {}
//	~RESULT() {
//		Uninit();
//	}
//
//	void Init() override;
//	void Draw() override;
//	void Draw_DPP() override;
//	void Update(float delta_time) override;
//	void Uninit() override;
//
//	static bool Clear_Flag;
//
//	template<typename Archive>
//	void serialize(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//	/*template<class Archive>
//	void save(Archive& ar) const
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//	template<class Archive>
//	void load(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}*/
//};
//
//CEREAL_REGISTER_TYPE(RESULT)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, RESULT)

///**
//* @brief ���C�����j���[�V�[���N���X
//* @details �V�[���N���X�̔h���N���X
//* @details �^�C�g���ƃQ�[���̊Ԃ̃V�[��
//*/
//class MAIN_MENU : public SCENE {
//private:
//
//protected:
//
//public:
//	MAIN_MENU() {}
//	~MAIN_MENU() {
//		Uninit();
//	}
//
//	void Init() override;
//	void Draw() override;
//	void Draw_DPP() override;
//	void Update(float delta_time) override;
//	void Uninit() override;
//
//	void Init(bool);
//
//	static string Model_Name;
//
//	template<typename Archive>
//	void serialize(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//	/*template<class Archive>
//	void save(Archive& ar) const
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}
//
//	template<class Archive>
//	void load(Archive& ar)
//	{
//		ar(cereal::base_class<SCENE>(this));
//	}*/
//};
//
//CEREAL_REGISTER_TYPE(MAIN_MENU)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, MAIN_MENU)

///**
//* @brief �V�[���Ǘ��N���X
//* @details �V�[���N���X���Ǘ����邽�߂̃N���X
//*/
//class SCENE_MANAGER {
//private:
//	static SCENE* pScene;
//
//protected:
//
//public:
//	SCENE_MANAGER() {
//		//if (nullptr == pScene)
//		//	pScene = new SCENE();
//	};
//
//	~SCENE_MANAGER() {
//		Uninit();
//	};
//
//	void Init() {
//		if(nullptr != pScene) pScene->Init();
//	};
//
//	void Draw() {
//		pScene->Draw();
//	};
//
//	void Draw_DPP() {
//		pScene->Draw_DPP();
//	};
//
//	void Update(float delta_time) {
//		pScene->Update(delta_time);
//	};
//
//	void Uninit() {
//		//pScene->Uninit();
//		SAFE_DELETE(pScene);
//	};
//
//	static SCENE* const Get_Scene() {
//		return pScene;
//	};
//
//	template <typename T>
//	static void Set_Scene() {
//		T* scene = new T();
//
//		SAFE_DELETE(pScene);
//
//		pScene = scene;
//
//		pScene->Init();
//	};
//};

#endif // !SCENE_H