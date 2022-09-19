#include "Scene.h"
#include "manager.h"
#include "Scene_Manager.h"

#include "Enemy.h"
#include "Player.h"
#include "Debug_Camera.h"
#include "Bounding_Aabb.h"

#include "Bullet.h"
#include "Axis.h"

#include "Model.h"
#include "Collision.h"

//#include "audio_clip.h"
#include "./include/engine/modules/audio/audio_clip.h"

#include "Math.h"
#include "include/engine/core/random/Random.h"

IMPLEMENT_OBJECT_TYPE_INFO(GameObject, ENEMY)

using namespace aegis;

static void Create_Bullet(Vector3& position, const Vector3& front);

ENEMY::ENEMY()
{
	Fire_Flag = true;
}

ENEMY::~ENEMY()
{
	Uninit();

	GameObject::Uninit();
}

void ENEMY::Init()
{
	GameObject::Init();

	Time = 0.f;

	// コンポーネント
	{
		auto scene = CManager::Get_Instance()->Get_Scene();

		//if (nullptr == ab)
		{
			//auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));
			auto aabb = this->AddComponent<BOUNDING_AABB>();
			
			aabb->Set_Radius(Vector3(0.5f, 1.0f, 0.5f));
			aabb->Set_Position(Vector3(0.f, 0.5f, 0.f));
		}

		aegis::string name("viranrifle.fbx");

		//auto model = Get_Component()->Add_Component<MODEL>(scene->Get_Game_Object(this));
		auto model = this->AddComponent<MODEL>();
		model->Set_Model_Name(name);
		
		model->Set_Position(Get_Transform().Get_Position());
		model->Set_Rotation(Get_Transform().Get_Rotation());
		model->Set_Scaling(Get_Transform().Get_Scaling());
	}

	GameObject::InitEnd();
}

void ENEMY::Draw()
{
	GameObject::Draw();
}

void ENEMY::Draw_Shadow()
{
	GameObject::Draw_Shadow();
}

void ENEMY::Draw_DPP()
{
	//GameObject::Draw_DPP();
}

void ENEMY::Update(float delta_time)
{
	Vector3 vec;
	{
		auto scene = CManager::Get_Instance()->Get_Scene();

		auto player = scene->Get_Game_Object<PLAYER>("player");

		Vector3 position(0.f, 0.f, 0.f);
		if (!player.expired())
		{
			position = player.lock()->Get_Transform().Get_Position();
		}

		vec = (position - Get_Transform().Get_Position());
		{

			Vector3 rotate(0.f, 0.f, 0.f);
			{
				if (vec.z >= 0.0f)
				{
					rotate.y = std::atan(vec.x / vec.z);
				}
				else
				{
					rotate.y = std::atan(vec.x / vec.z) + XM_PI;
				}
			}
			XMVECTOR vector = XMQuaternionRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);

			rotate.y = XMConvertToDegrees(rotate.y);

			Get_Transform().Set_Rotation(rotate);

			//Model->Set_Quaternion(vector);
		}
	}

	// 移動
	{
		Vector3 move(0.f, 0.f, 0.f);

		// プレイヤーとの距離を一定にする
		{
			const auto player = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<PLAYER>("player");

			float r = (Get_Transform().Get_Position().x - player.lock()->Get_Transform().Get_Position().x) * (Get_Transform().Get_Position().x - player.lock()->Get_Transform().Get_Position().x) + (Get_Transform().Get_Position().z - player.lock()->Get_Transform().Get_Position().z) * (Get_Transform().Get_Position().z - player.lock()->Get_Transform().Get_Position().z);
			float abr = 8.0f;

			// 離す
			if (r <= (abr * 1.2f) * (abr * 1.2f))
			{
				move.x -= vec.x * delta_time * 2.0f;
				move.z -= vec.z * delta_time * 2.0f;
			}

			// 引き寄せる
			if (r >= (abr * 2.0f) * (abr * 2.0f))
			{
				move.x += vec.x * delta_time * 2.0f;
				move.z += vec.z * delta_time * 2.0f;
			}
		}

		Vector3 pos = Get_Transform().Get_Position();
		pos.x += move.x;
		pos.z += move.z;

		Get_Transform().Set_Position(pos);
	}

	if (0.5f <= Time)
	{
		// 弾を撃つ
		if(aegis::Random::GetRandomfloat() <= 0.01f)
		{
			XMVECTOR vector = XMLoadFloat3(&vec);
			vector = XMVector3Normalize(vector);

			Vector3 rotate;
			XMStoreFloat3(&rotate, vector * 2.0f);

			Vector3 position = Get_Transform().Get_Position();

			position.x += rotate.x * 2;
			position.z += rotate.z * 2;


			//Create_Bullet(position, rotate);
			//
			//AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_SHOT, false);
		}
	}
	else
	{
		Time += delta_time;
	}

	GameObject::Update(delta_time);
}

void ENEMY::Uninit(void)
{
	GameObject::Uninit();
}

// ポジションの設定
void ENEMY::SetPosition(Vector3& position)
{
	Get_Transform().Set_Position(position);
}

// 回転の設定
void ENEMY::SetRotation(Vector3& rotation)
{
	Get_Transform().Set_Rotation(rotation);
}

// 拡大縮小の値の設定
void ENEMY::SetScaling(Vector3& scaling)
{
	Get_Transform().Set_Scaling(scaling);
}

void Create_Bullet(Vector3& position, const Vector3& front)
{
	auto scene = CManager::Get_Instance()->Get_Scene();

	auto bullets = scene->Get_Game_Objects<BULLET>();

	auto bullet = scene->Add_Game_Object<BULLET>( LAYER_NAME::GAMEOBJECT, aegis::string("bullet" + std::to_string(bullets.size() + 1)) );

	//bullet->Init();

	bullet->Get_Transform().Set_Position(position);
	bullet->Set_Move_Vector(front);
}