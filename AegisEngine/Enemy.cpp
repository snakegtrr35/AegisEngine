#include	"ModelLoader.h"
#include	"Collision.h"
#include	"Player.h"
#include	"Enemy.h"
#include	"Scene.h"
#include	"manager.h"
#include	"Bounding_Aabb.h"
#include	"Bullet.h"
#include	"Axis.h"
#include	"audio_clip.h"
#include	"Math.h"

static void Create_Bullet(XMFLOAT3& position, XMFLOAT3& front);

ENEMY::ENEMY() {}

ENEMY::~ENEMY()
{
	Uninit();
}

void ENEMY::Init()
{
	/*Position = XMFLOAT3(3.0f, 1.0f, 3.0f);

	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);*/

	{
		//string name = "asset/model/Player.fbx";
		string name("asset/model/viranrifle.fbx");

		Model = new CMODEL();

		Model->Load(name);

		Model->Set_Position(Position);
		Model->Set_Rotation(Rotation);
		Model->Set_Scaling(Scaling);
	}

	// コンポーネント
	{
		auto scene = CManager::Get_Instance()->Get_Scene();

		auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));

		//aabb->Set_Position(Position);

		//aabb->Set_Radius(XMFLOAT3(10, 10, 10));

		/*auto scene = CManager::Get_Instance()->Get_Scene();

		auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));*/
	}

	GAME_OBJECT::Init();
}

void ENEMY::Draw()
{
	Model->Draw();

	GAME_OBJECT::Draw();
}

void ENEMY::Draw_DPP()
{
	Model->Draw_DPP();
}

void ENEMY::Update(float delta_time)
{
	// 移動
	//{
	//	static int i = 0;
	//	XMFLOAT3 move;
	//	XMVECTOR vec = Component.Get_Component<AXIS_COMPONENT>()->Get_Right();
	//	vec = XMVectorScale(vec, 0.2f);
	//	XMStoreFloat3(&move, vec);

	//	if (Random_Bool(Date.RL_Probability))
	//	{
	//		Date.RL = !Date.RL;
	//	}

	//	if (Date.RL)
	//	{
	//		XMStoreFloat3(&move, XMVectorScale(vec, -1.0f));
	//	}

	//	// プレイヤーとの距離を一定にする
	//	{
	//		XMVECTOR vec = Component.Get_Component<AXIS_COMPONENT>()->Get_Front();
	//		vec = XMVectorScale(vec, -0.2f);

	//		XMFLOAT3 pos;
	//		XMStoreFloat3(&pos, vec);

	//		const auto player = CManager::Get_Scene()->Get_Game_Object<PLAYER>("player");

	//		float r = (Position.x - player.lock()->Get_Position()->x) * (Position.x - player.lock()->Get_Position()->x) + (Position.z - player.lock()->Get_Position()->z) * (Position.z - player.lock()->Get_Position()->z);
	//		float abr = Date.Lenght * Date.Lenght;

	//		// 離す
	//		if (r <= abr * abr)
	//		{
	//			move.x += pos.x;
	//			move.z += pos.z;
	//		}

	//		// 引く
	//		abr = (Date.Lenght* 1.5f) * (Date.Lenght* 1.5f);

	//		if (r >= abr * abr)
	//		{
	//			vec = XMVectorScale(vec, -1.0f);
	//			XMStoreFloat3(&pos, vec);

	//			move.x += pos.x;
	//			move.z += pos.z;
	//		}

	//	}

	//	Position.x += move.x;
	//	Position.z += move.z;
	//}

	//// 弾を撃つ
	//if (Random_Bool(Date.Attack_Probability))
	//{
	//	XMFLOAT3 f;
	//	XMFLOAT3 pos = Position;

	//	//XMStoreFloat3(&f, Component.Get_Component<AXIS_COMPONENT>()->Get_Front());

	//	pos.x += f.x * 2;
	//	pos.y += f.y;
	//	pos.z += f.z * 2;

	//	Create_Bullet(pos, f);

	//	AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_SHOT, false);
	//}

	Model->Set_Position(Position);
	Model->Set_Rotation(Rotation);
	Model->Set_Scaling(Scaling);


	GAME_OBJECT::Update(delta_time);
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
	SCENE* scene = CManager::Get_Instance()->Get_Scene();

	BULLET* bullet = scene->Add_Game_Object<BULLET>(LAYER_NAME::GAMEOBJECT, "enemy");

	

	XMFLOAT3 Position = position;
	Position.y += 0.5f;

	XMFLOAT3 scaling = XMFLOAT3(0.1f, 0.1f, 0.1f);

	bullet->Set_Position(&Position);
	bullet->Set_Move_Vector(front);
	bullet->Set_Scaling(scaling);
}