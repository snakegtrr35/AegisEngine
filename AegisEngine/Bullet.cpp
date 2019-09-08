#include	"manager.h"
#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Circle_Shadow.h"
#include	"Collision.h"
#include	"Score.h"
#include	"Bullet.h"

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

	Shadow = new CIRCLE_SHADOW();
	Shadow->Set_Position(&Position);

	Shadow->SetWH(XMFLOAT2(0.3f, 0.3f));
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

void BULLET::Init(void)
{
}

void BULLET::Draw(void)
{
	Model->Draw();
	Shadow->Draw();
}

void BULLET::Update(void)
{
	Position.x += MoveVector.x * 0.2f;
	Position.y += MoveVector.y * 0.2f;
	Position.z += MoveVector.z * 0.2f;

	// 影の更新
	{
		XMFLOAT3 pos = Position;

		pos.y = 0.01f;

		Shadow->Set_Position(&pos);
	}

	Model->SetPosition(Position);
	Model->SetRotation(Rotation);
	Model->SetScaling(Scaling);

	//dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Position(Position);
	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);

	HP--;


	if (HP <= 0)
	{
		CManager::Get_Scene()->Destroy_Game_Object(this);
	}

	// 敵と弾の当たり判定
	{
		vector<ENEMY*> enemys = CManager::Get_Scene()->Get_Game_Objects<ENEMY>();

		for (ENEMY* enemy : enemys)
		{
			//if (Collision_HitSphere(this->Get_Collison(), enemy->Get_Collision()))
			if (Collision_HitAABB(this->Get_Collison(), enemy->Get_Collision()))
			{
				// ビルボード
				{
					BILL_BOARD_ANIMATION* bba = CManager::Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT);
					bba->Set_Position(&Position);
					bba->SetWH(XMFLOAT2(1.0f, 1.0f));
					bba->SetParam(3.0f, 4, 4);
				}

				//SCORE* score = CManager::Get_Scene()->Get_Game_Object<SCORE>();

				//score->Add(100);

				CManager::Get_Scene()->Destroy_Game_Object(this);
				CManager::Get_Scene()->Destroy_Game_Object(enemy);

				AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);
			}
		}
	}

	// プレイヤーと弾の当たり判定
	{
		PLAYER* player = CManager::Get_Scene()->Get_Game_Object<PLAYER>();

		if (Collision_HitAABB(this->Get_Collison(), player->Get_Collision()))
		{
			// ビルボード
			{
				BILL_BOARD_ANIMATION* bba = CManager::Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT);
				bba->Set_Position(&Position);
				bba->SetWH(XMFLOAT2(1.0f, 1.0f));
				bba->SetParam(3.0f, 4, 4);
			}

			CManager::Get_Scene()->Destroy_Game_Object(this);
			player->Damage();

			AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);
		}
	}
}

void BULLET::Uninit(void)
{
	SAFE_DELETE(Model);
	SAFE_DELETE(Shadow);
}

void BULLET::Set_Move_Vector(const XMFLOAT3 move_vector)
{
	MoveVector = move_vector;
}