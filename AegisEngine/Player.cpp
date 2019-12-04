#include	"manager.h"
#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"Circle_Shadow.h"
#include	"Collision.h"
#include	"Player.h"

#include	"Score.h"
#include	"Fade.h"

#include	"audio_clip.h"

#include	"Timer.h"

float radius = 3.0f;

PLAYER::PLAYER(void)
{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	{
		AnimType = true;
		Blend = 1.0f;

		string name = "asset/model/human01_Stop.fbx";

		Model = new CMODEL();

		Model->Load(name);

		Model->Set_Position(Position);
		Model->Set_Rotation(Rotation);
		Model->Set_Scaling(Scaling);
	}

	Collision = new COLLISIION_SPHERE();

	dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Position(Position);

	dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Radius(3.0f);

	{
		COLOR color(1.0f, 0.f, 0.f, 1.0f);

		Shpere = new BOUNDING_SHPERE();
		//Shpere = new BOUNDING_AABB();

		Shpere->Set_Position(Position);
		Shpere->Set_Radius(radius);
		Shpere->Set_Color(color);
		//Shpere->Init();
	}

	//shadow = new CIRCLE_SHADOW();
	//shadow->Set_Position(&Position);
	//shadow->SetWH(XMFLOAT2(0.8f, 0.8f));
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
	//shadow->Draw();

	Shpere->Draw();
}

void PLAYER::Update(float delta_time)
{
	//CCamera* camera = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	DEBUG_CAMERA* camera = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMVECTOR* vec = camera->Get_At();
	XMVECTOR* front = camera->Get_Front();
	*front = XMVector4Normalize(*front);
	XMVECTOR* right = camera->Get_Right();
	*right = XMVector4Normalize(*right);
	XMFLOAT3 pos;

	XMStoreFloat3(&pos, *vec);

	//Position = pos;

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

	AnimType = true;

	if (KEYBOARD::Press_Keyboard(VK_W))
	{
		Position.x += front_vec.x * delta_time * 10.0f;
		Position.z += front_vec.z * delta_time * 10.0f;

		AnimType = false;
	}

	if (KEYBOARD::Press_Keyboard(VK_S))
	{
		Position.x -= front_vec.x * delta_time * 10.0f;
		Position.z -= front_vec.z * delta_time * 10.0f;

		AnimType = false;
	}

	if (KEYBOARD::Press_Keyboard(VK_A))
	{
		Position.x -= right_vec.x * delta_time * 10.0f;
		Position.z -= right_vec.z * delta_time * 10.0f;

		AnimType = false;
	}

	if (KEYBOARD::Press_Keyboard(VK_D))
	{
		Position.x += right_vec.x * delta_time * 10.0f;
		Position.z += right_vec.z * delta_time * 10.0f;

		AnimType = false;
	}

	// メッシュフィールドとの当たり判定
	//MESH_FIELD* pfield = CManager::Get_Scene()->Get_Game_Object<MESH_FIELD>();
	//if (nullptr != pfield)
	//	Position.y = pfield->Get_Height(Position);

	// カメラに合わせた回転
	//Rotation.y = rotate + 90.0f;

	// モデルの更新
	{
		// 待機
		if (AnimType)
		{
			Blend = min(1.0f, Blend + (1.0f / 60.0f));
		}
		else  // 歩く
		{
			Blend = max(0.0f, Blend - (1.0f / 60.0f));
		}

		Model->Set_Position(Position);
		Model->Set_Rotation(Rotation);
		Model->Set_Scaling(Scaling);

		Model->Update(delta_time);

		Blend = Model->Get().Get_Ratio();
	}

	dynamic_cast<COLLISIION_SPHERE*>(Collision)->Set_Position(Position);
	Shpere->Set_Position(Position);
	Shpere->Set_Scaling(Scaling);
	Shpere->Set_Radius(radius);

	// 影の更新
	{
		//XMFLOAT3 pos = Position;

		//pos.y += 0.01f;
		
		//shadow->Set_Position(&pos);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_1))
	{
		//AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_LBUTTON))
	{
		//SCORE* score = CManager::Get_Scene()->Get_Game_Object<SCORE>();

		//if (nullptr != score)
		//	score->Add(10);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_RBUTTON))
	{
		//SCORE* score = CManager::Get_Scene()->Get_Game_Object<SCORE>();

		//if(nullptr != score)
		//	score->Add(-10);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_F1))
	{
		Model->Get().Change_Anime("Stop", 60);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_F2))
	{
		Model->Get().Change_Anime("Walk", 60);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_F3))
	{
		Model->Get().Change_Anime("Jump", 60);
	}
}

void PLAYER::Uninit(void)
{
	SAFE_DELETE(Model);
	//SAFE_DELETE(shadow);

	SAFE_DELETE(Collision);
	SAFE_DELETE(Shpere);
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