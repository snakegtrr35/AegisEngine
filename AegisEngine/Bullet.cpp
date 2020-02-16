#include	"manager.h"
#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Collision.h"
#include	"Score.h"
#include	"Bullet.h"
#include	"Player.h"
#include	"Enemy.h"
#include	"Billboard.h"

#include	"audio_clip.h"

BULLET::BULLET()
{
	HP = 300;

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Scaling = XMFLOAT3(0.3f, 0.3f, 0.3f);

	Model = new CMODEL();

	string name("asset/model/Bullet.obj");

	Model->Load(name);

	Collision = new COLLISIION_AABB();

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Radius(XMFLOAT3(0.5f, 0.5f, 0.5f));
}

BULLET::BULLET(XMFLOAT3& position, XMFLOAT3& move_vector)
{
	HP = 300;

	Position = position;

	MoveVector = move_vector;

	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Scaling = XMFLOAT3(0.3f, 0.3f, 0.3f);

	Model = new CMODEL();

	string name("asset/model/Bullet.obj");

	Model->Load(name);

	/*Collision = new COLLISIION_SPHERE();

	dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Position(Position);

	dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Radius(1.0f);*/

	Collision = new COLLISIION_AABB();

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Radius(XMFLOAT3(0.5f, 0.5f, 0.5f));
}

BULLET::~BULLET()
{
	Uninit();
}

void BULLET::Init()
{
}

void BULLET::Draw()
{
	Model->Draw();

	GAME_OBJECT::Draw();
}

void BULLET::Draw_DPP()
{
	Model->Draw_DPP();
}

void BULLET::Update(float delta_time)
{
	Position.x += MoveVector.x * 0.2f;
	Position.y += MoveVector.y * 0.2f;
	Position.z += MoveVector.z * 0.2f;

	Model->Set_Position(Position);
	Model->Set_Rotation(Rotation);
	Model->Set_Scaling(Scaling);

	//dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Position(Position);
	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);

	HP--;

	if (HP <= 0)
	{
		CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);
	}

	// 敵と弾の当たり判定
	{
		vector<ENEMY*> enemys = CManager::Get_Instance()->Get_Scene()->Get_Game_Objects<ENEMY>();

		for (ENEMY* enemy : enemys)
		{
			//if (Collision_HitSphere(this->Get_Collison(), enemy->Get_Collision()))
			//if (Collision_HitAABB(this->Get_Collison(), enemy->Get_Collision()))
			{
				// ビルボード
				{
					BILL_BOARD_ANIMATION* bba = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT, "test");
					bba->Set_Position(&Position);
					bba->SetWH(XMFLOAT2(1.0f, 1.0f));
					bba->SetParam(3.0f, 4, 4);
				}

				//SCORE* score = CManager::Get_Scene()->Get_Game_Object<SCORE>();

				//score->Add(100);

				CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);
				CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(enemy);

				AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);
			}
		}
	}

	// プレイヤーと弾の当たり判定
	{
		const auto player = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<PLAYER>("player");

		//if (Collision_HitAABB(this->Get_Collison(), player->Get_Collision()))
		//{
		//	// ビルボード
		//	{
		//		BILL_BOARD_ANIMATION* bba = CManager::Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT);
		//		bba->Set_Position(&Position);
		//		bba->SetWH(XMFLOAT2(1.0f, 1.0f));
		//		bba->SetParam(3.0f, 4, 4);
		//	}

		//	CManager::Get_Scene()->Destroy_Game_Object(this);

		//	AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);
		//}
	}

	GAME_OBJECT::Update(delta_time);
}

void BULLET::Uninit()
{
	SAFE_DELETE(Model);
}

void BULLET::Set_Move_Vector(const XMFLOAT3 move_vector)
{
	MoveVector = move_vector;
}