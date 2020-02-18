#include	"manager.h"
#include	"Scene.h"

#include	"ModelLoader.h"
//#include	"FBXmodel.h"

#include	"Input.h"
#include	"Collision.h"
#include	"Player.h"
#include	"Bounding_Aabb.h"
#include	"Score.h"
#include	"Fade.h"

#include	"audio_clip.h"

#include	"Timer.h"

PLAYER::PLAYER(void)
{
	Model = new CMODEL();
	//Model = new FBXmodel();
}

PLAYER::~PLAYER()
{
	Uninit();
}

void PLAYER::Init(void)
{
	{
		AnimType = true;
		Blend = 1.0f;

		//string name = "asset/model/Player.fbx";
		string name = "asset/model/viranrifle.fbx";
		//string name = "asset/model/kakunin_joint.fbx";

		Model->Load(name);
	}

	{
		//auto scene = CManager::Get_Instance()->Get_Scene();

		//auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));

		//aabb->Set_Position(Position);

		//aabb->Set_Radius(XMFLOAT3(10, 10, 10));

		GAME_OBJECT::Init();
	}
}

void PLAYER::Draw(void)
{
	{
		XMMATRIX matrix= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		Model->Draw();
		//Model->Draw(matrix);
	}

	GAME_OBJECT::Draw();
}

void PLAYER::Draw_DPP(void)
{
	{
		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		Model->Draw_DPP();
		//Model->Draw_DPP(matrix);
	}
}

void PLAYER::Update(float delta_time)
{
	//CCamera* camera = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	const auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMVECTOR* vec = camera.lock()->Get_At();
	XMVECTOR* front = camera.lock()->Get_Front();
	*front = XMVector4Normalize(*front);
	XMVECTOR* right = camera.lock()->Get_Right();
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
		Model->Set_Position(Position);
		Model->Set_Rotation(Rotation);
		Model->Set_Scaling(Scaling);

		Model->Update(delta_time);
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

	//if (KEYBOARD::Trigger_Keyboard(VK_F1))
	//{
	//	Model->Get().Change_Anime("Stop", 60);
	//}

	//if (KEYBOARD::Trigger_Keyboard(VK_F2))
	//{
	//	Model->Get().Change_Anime("Walk", 60);
	//}

	//if (KEYBOARD::Trigger_Keyboard(VK_F3))
	//{
	//	Model->Get().Change_Anime("Jump", 60);
	//}

	GAME_OBJECT::Update(delta_time);
}

void PLAYER::Uninit(void)
{
	SAFE_DELETE(Model);
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