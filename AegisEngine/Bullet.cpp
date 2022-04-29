//#include	"manager.h"
//#include	"Scene_Manager.h"
//#include	"Scene.h"

#include	"GameObject.h"
#include	"Bullet.h"

#include	"manager.h"
#include	"Scene.h"

#include	"Component_Manager.h"
#include	"Bounding_Aabb.h"
#include	"Bounding_Sphere.h"
#include	"Score.h"
#include	"Player.h"
#include	"Enemy.h"
#include	"Billboard.h"
#include	"Model.h"

#include	"audio_clip.h"

IMPLEMENT_OBJECT_TYPE_INFO(GameObject, BULLET)

using namespace aegis;

BULLET::BULLET() : MoveVector(Vector3(0.0f, 0.0f, 0.0f))
{
	Vector3 Scaling = Vector3(0.1f, 0.1f, 0.1f);
	Get_Transform().Set_Scaling(Scaling);

	HP = 200;
}

BULLET::BULLET(Vector3& position, Vector3& move_vector) : MoveVector(move_vector)
{
	Get_Transform().Set_Position(position);

	Vector3 Scaling = Vector3(0.1f, 0.1f, 0.1f);
	Get_Transform().Set_Scaling(Scaling);

	HP = 200;
}

BULLET::~BULLET()
{
	Uninit();
}

void BULLET::Init()
{
	auto scene = CManager::Get_Instance()->Get_Scene();

	aegis::string name("bullet.fbx");

	//auto model = Get_Component()->Add_Component<MODEL>(scene->Get_Game_Object(this));
	auto model = this->AddComponent<MODEL>();
	model->Set_Model_Name(name);
	model->Set_Scaling(Get_Transform().Get_Scaling());

	//auto sphere = Get_Component()->Add_Component<BOUNDING_SHPERE>(scene->Get_Game_Object(this));
	auto sphere = this->AddComponent<BOUNDING_SHPERE>();
	
	sphere->Set_Radius(1.0f);
	sphere->Set_Scaling(Vector3(0.11f, 0.11f, 0.11f));

	GameObject::Init();
}

void BULLET::Draw()
{
	GameObject::Draw();
}

void BULLET::Draw_Shadow()
{
	GameObject::Draw_Shadow();
}

void BULLET::Draw_DPP()
{
	//GameObject::Draw_DPP();
}

void BULLET::Update(float delta_time)
{
	Vector3 pos = Get_Transform().Get_Position();

	pos.x += MoveVector.x * delta_time * 5.0f;
	pos.y += MoveVector.y * delta_time * 5.0f;
	pos.z += MoveVector.z * delta_time * 5.0f;

	Get_Transform().Set_Position(pos);

	if (HP <= 0)
	{
		CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);

		// ビルボード
		{
			aegis::string str = this->Get_Object_Name();
			ExtratNum(str);
			if (false == str.empty())
			{
				const int x = std::stoi(str.c_str());
		
				BILL_BOARD_ANIMATION* bba = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT, aegis::string("explosion" + std::to_string(x)));
				bba->Get_Transform().Set_Position(pos);
				bba->SetWH(Vector2(1.0f, 1.0f));
				bba->SetParam(6, 4, 4);
				//bba->Init();
			}
		}

		return;
	}

	fps += delta_time;

	if (ANIMETION_FRAME_60 <= fps)
	{
		HP--;

		fps = 0.0f;
	}
	
	auto scene = CManager::Get_Instance()->Get_Scene();

	// 敵と弾の当たり判定
	{
		vector<ENEMY*> enemys = scene->Get_Game_Objects<ENEMY>();

		for (auto enemy : enemys)
		{
			//auto bullet_collision = this->Get_Component()->Get_Component<BOUNDING_SHPERE>();
			//auto enemy_collision = enemy->Get_Component()->Get_Component<BOUNDING_AABB>();
			auto bullet_collision = this->AddComponent<BOUNDING_SHPERE>();
			auto enemy_collision = this->AddComponent<BOUNDING_AABB>();


			if (ContainmentType::DISJOINT != bullet_collision->Get_Collition().Contains(enemy_collision->Get_Collition()))
			{
				// ビルボード
				{
					auto name = this->Get_Object_Name();

					aegis::string str(name);
					ExtratNum(str);
					if (!str.empty())
					{
						const int x = std::stoi(str.c_str());

						BILL_BOARD_ANIMATION* bba = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT, aegis::string("explosion" + std::to_string(x)));
						bba->Get_Transform().Set_Position(pos);
						bba->SetWH(Vector2(1.0f, 1.0f));
						bba->SetParam(6, 4, 4);
						//bba->Init();
					}
				}

				CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);
				CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(enemy);

				AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);

				return;
			}

			// プレイヤーと弾の当たり判定
			{
				auto player = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<PLAYER>("player");
			
				//if (ContainmentType::DISJOINT != bullet_collision->Get_Collition().Contains(player.lock()->Get_Component()->Get_Component<BOUNDING_AABB>()->Get_Collition()))
				if (ContainmentType::DISJOINT != bullet_collision->Get_Collition().Contains(player.lock()->GetComponent<BOUNDING_AABB>()->Get_Collition()))
				{
					auto billboards = scene->Get_Game_Objects<BILL_BOARD_ANIMATION>();
			
					// ビルボード
					{
						auto name = this->Get_Object_Name();
			
						aegis::string str(name);
						ExtratNum(str);
			
						if (!str.empty())
						{
							const int x = std::stoi(str.c_str());
			
							BILL_BOARD_ANIMATION* bba = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT, aegis::string("explosion" + std::to_string(x)));
							bba->Get_Transform().Set_Position(pos);
							bba->SetWH(Vector2(1.0f, 1.0f));
							bba->SetParam(6, 4, 4);
						}
					}
			
					player.lock()->Add_HP(-1.0f);
			
					CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);
			
					AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);
			
					return;
				}
			}
		}
	}

	GameObject::Update(delta_time);
}

void BULLET::Uninit()
{
	GameObject::Uninit();
}

void BULLET::Set_Move_Vector(const Vector3 move_vector)
{
	MoveVector = move_vector;
}