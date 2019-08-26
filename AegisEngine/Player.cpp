#include	"main.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"Circle_Shadow.h"
#include	"Collision.h"
#include	"Bullet.h"
#include	"Player.h"

#include	"Score.h"
#include	"Fade.h"

#include	"audio_clip.h"

static void Create_Bullet(XMFLOAT3& position, XMFLOAT3& front);

static unsigned char cnt = 1;
static unsigned char rate;

PLAYER::PLAYER(void)
{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Scaling = XMFLOAT3(0.5f, 0.5f, 0.5f);

	{
		string name = MAIN_MENU::Model_Name;

		Model = new CMODEL(Position, Rotation, Scaling);

		Model->Load(name);

		Model->SetPosition(Position);
		Model->SetRotation(Rotation);
		Model->SetScaling(Scaling);
	}

	Collision = new COLLISIION_AABB();

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Radius(XMFLOAT3(1.0f, 1.0f, 1.0f));

	shadow = new CIRCLE_SHADOW();
	shadow->Set_Position(&Position);

	shadow->SetWH(XMFLOAT2(0.8f, 0.8f));

	if ("asset/model/herorifle.fbx" == MAIN_MENU::Model_Name)
	{
		rate = 10;
	}
	else
	{
		rate = 60;
	}

	cnt = 0;

	HP = 100.0f;
}

PLAYER::~PLAYER()
{
	Uninit();
}

void PLAYER::Init(void)
{
}

void PLAYER::Draw(void)
{
	Model->Draw();
	shadow->Draw();
}

void PLAYER::Update(void)
{
	CCamera* camera = CManager::Get_Scene()->Get_Game_Object<CCamera>();

	XMVECTOR* vec = camera->Get_At();
	XMVECTOR* front = camera->Get_Front();
	XMVECTOR* right = camera->Get_Right();
	XMFLOAT3 pos;
	float rotate = camera->Get();

	XMStoreFloat3(&pos, *vec);

	Position = pos;

	XMVECTOR f(*front);
	XMFLOAT3 front_vec;
	XMStoreFloat3(&front_vec, f);
	front_vec.y = 0.0f;
	f = XMLoadFloat3(&front_vec);

	XMVECTOR r(*right);
	XMFLOAT3 right_vec;
	XMStoreFloat3(&right_vec, r);
	right_vec.y = 0.0f;
	r = XMLoadFloat3(&right_vec);

	if (KEYBOARD::Press_Keyboard(VK_W))
	{
		Position.x += front_vec.x * 0.1f;
		Position.z += front_vec.z * 0.1f;
	}

	if (KEYBOARD::Press_Keyboard(VK_S))
	{
		Position.x -= front_vec.x * 0.1f;
		Position.z -= front_vec.z * 0.1f;
	}

	if (KEYBOARD::Press_Keyboard(VK_A))
	{
		Position.x -= right_vec.x * 0.1f;
		Position.z -= right_vec.z * 0.1f;
	}

	if (KEYBOARD::Press_Keyboard(VK_D))
	{
		Position.x += right_vec.x * 0.1f;
		Position.z += right_vec.z * 0.1f;
	}

	// メッシュフィールドとの当たり判定
	MESH_FIELD* pfield = CManager::Get_Scene()->Get_Game_Object<MESH_FIELD>();
	if (nullptr != pfield)
		Position.y = pfield->Get_Height(Position);

	// カメラに合わせた回転
	Rotation.y = rotate + 90.0f;

	{
		Model->SetPosition(Position);
		Model->SetRotation(Rotation);
		Model->SetScaling(Scaling);
	}

	dynamic_cast<COLLISIION_AABB*>(Collision)->Set_Position(Position);

	// 影の更新
	{
		XMFLOAT3 pos = Position;

		pos.y += 0.01f;
		
		shadow->Set_Position(&pos);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_1))
	{
		AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_LBUTTON))
	{
		SCORE* score = CManager::Get_Scene()->Get_Game_Object<SCORE>();

		if (nullptr != score)
			score->Add(10);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_RBUTTON))
	{
		SCORE* score = CManager::Get_Scene()->Get_Game_Object<SCORE>();

		if(nullptr != score)
			score->Add(-10);
	}

	// 弾の生成
	if (KEYBOARD::Press_Keyboard(VK_SPACE))
	{
		if (0 == (cnt % rate))
		{

			XMFLOAT3 front;
			XMStoreFloat3(&front, f);

			XMFLOAT3 right;
			XMVECTOR vec = XMVectorScale(r, -0.3f);
			XMStoreFloat3(&right, vec);

			XMFLOAT3 pos = Position;


			pos.x += front.x * 2;
			pos.z += front.z * 2;

			pos.x += right.x;
			pos.z += right.z;

			Create_Bullet(pos, front);

			cnt = 1;

			AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_SHOT, false);
		}

		cnt++;
	}

	// 死亡
	if(HP <= 0)
	{
		RESULT::Clear_Flag = false;
		FADE::Start_FadeOut(60);
	}
}

void PLAYER::Uninit(void)
{
	SAFE_DELETE(Model);
	SAFE_DELETE(shadow);
}

// ポジションの設定
void PLAYER::SetPosition(const XMFLOAT3 position)
{
	Position = position;
}

// 拡大縮小の値の設定
void PLAYER::SetScaling(const XMFLOAT3 scaling)
{
	Scaling = scaling;
}

void Create_Bullet(XMFLOAT3& position, XMFLOAT3& front)
{
	SCENE* scene = CManager::Get_Scene();

	BULLET* bullet = scene->Add_Game_Object<BULLET>(LAYER_NAME::GAMEOBJECT);

	position.y += 1.0f;

	XMFLOAT3 scaling = XMFLOAT3(0.1f, 0.1f, 0.1f);

	bullet->Set_Position(&position);
	bullet->Set_Move_Vector(front);
	bullet->Set_Scaling(scaling);
}