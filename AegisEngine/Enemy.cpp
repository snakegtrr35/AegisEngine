#include	"ModelLoader.h"
#include	"Collision.h"
#include	"Enemy.h"
#include	"Scene.h"
#include	"manager.h"
#include	"Bounding.h"
#include	"Bullet.h"
#include	"Axis.h"
#include	"audio_clip.h"

static void Create_Bullet(XMFLOAT3& position, XMFLOAT3& front);

ENEMY::ENEMY(void)
{
	Position = XMFLOAT3(3.0f, 1.0f, 3.0f);

	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Model = new CMODEL(Position, Rotation, Scaling);

	{
		//string name("asset/model/miku_01.obj");
		string name("asset/model/viranrifle.fbx");

		Model = new CMODEL(Position, Rotation, Scaling);

		Model->Load(name);

		Model->SetPosition(Position);
		Model->SetRotation(Rotation);
		Model->SetScaling(Scaling);
	}

	Collision = new COLLISIION_AABB();

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Radius(XMFLOAT3(0.5f, 0.5f, 0.5f));

	Aabb = CManager::Get_Scene()->Add_Game_Object<BOUNDING_AABB>(LAYER_NAME::GAMEOBJECT);

	Aabb->Set_Position(Position);
	Aabb->Set_Radius(XMFLOAT3(0.5f, 0.5f, 0.5f));
}

ENEMY::~ENEMY()
{
	Uninit();
}

void ENEMY::Init(void)
{
}

void ENEMY::Draw(void)
{
	Model->Draw();
}

void ENEMY::Update(void)
{
	// Axisの更新
	{
		XMFLOAT3 pos = Position;
		XMVECTOR vec;

		PLAYER* player = CManager::Get_Scene()->Get_Game_Object<PLAYER>();

		pos.x = player->Get_Position()->x - pos.x;
		pos.z = player->Get_Position()->z - pos.z;

		vec = XMLoadFloat3(&pos);

		XMVector3Normalize(vec);

		auto axis = Component.Get_Component<AXIS_COMPONENT>();

		axis->Set_Front(vec);

		axis->Init();
	}

	// 移動
	{
		static int i = 0;
		XMFLOAT3 move;
		XMVECTOR vec = Component.Get_Component<AXIS_COMPONENT>()->Get_Right();
		vec = XMVectorScale(vec, 0.2f);
		XMStoreFloat3(&move, vec);

		if (Rand_Bool(Date.RL_Probability))
		{
			Date.RL = !Date.RL;
		}

		if (Date.RL)
		{
			XMStoreFloat3(&move, XMVectorScale(vec, -1.0f));
		}

		// プレイヤーとの距離を一定にする
		{
			XMVECTOR vec = Component.Get_Component<AXIS_COMPONENT>()->Get_Front();
			vec = XMVectorScale(vec, -0.2f);

			XMFLOAT3 pos;
			XMStoreFloat3(&pos, vec);

			PLAYER* player = CManager::Get_Scene()->Get_Game_Object<PLAYER>();

			float r = (Position.x - player->Get_Position()->x) * (Position.x - player->Get_Position()->x) + (Position.z - player->Get_Position()->z) * (Position.z - player->Get_Position()->z);
			float abr = Date.Lenght * Date.Lenght;

			// 離す
			if (r <= abr * abr)
			{
				move.x += pos.x;
				move.z += pos.z;
			}

			// 引く
			abr = (Date.Lenght* 1.5f) * (Date.Lenght* 1.5f);

			if (r >= abr * abr)
			{
				vec = XMVectorScale(vec, -1.0f);
				XMStoreFloat3(&pos, vec);

				move.x += pos.x;
				move.z += pos.z;
			}

		}

		Position.x += move.x;
		Position.z += move.z;
	}

	// 弾を撃つ
	if (Rand_Bool(Date.Attack_Probability))
	{
		XMFLOAT3 f;
		XMFLOAT3 pos = Position;

		XMStoreFloat3(&f, Component.Get_Component<AXIS_COMPONENT>()->Get_Front());

		pos.x += f.x * 2;
		pos.y += f.y;
		pos.z += f.z * 2;

		Create_Bullet(pos, f);

		AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_SHOT, false);
	}

	Model->SetPosition(Position);
	Model->SetRotation(Rotation);

	//dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Position(Position);
	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);
}

void ENEMY::Uninit(void)
{
	SAFE_DELETE(Model);
}

// ポジションの設定
void ENEMY::SetPosition(const XMFLOAT3& position)
{
	Position = position;
}

// 回転の設定
void ENEMY::SetRotation(const XMFLOAT3& rotation)
{
	Rotation = rotation;
}

// 拡大縮小の値の設定
void ENEMY::SetScaling(const XMFLOAT3& scaling)
{
	Scaling = scaling;
}

void Create_Bullet(XMFLOAT3& position, XMFLOAT3& front)
{
	SCENE* scene = CManager::Get_Scene();

	BULLET* bullet = scene->Add_Game_Object<BULLET>(LAYER_NAME::GAMEOBJECT);

	

	XMFLOAT3 Position = position;
	Position.y += 0.5f;

	XMFLOAT3 scaling = XMFLOAT3(0.1f, 0.1f, 0.1f);

	bullet->Set_Position(&Position);
	bullet->Set_Move_Vector(front);
	bullet->Set_Scaling(scaling);
}