#include	"Game_Object.h"
#include	"Player.h"

#include	"manager.h"
#include	"Scene.h"

//#include	"ModelLoader.h"
#include	"FBXmodel.h"

#include	"Input.h"
#include	"Collision.h"
#include	"Bounding_Aabb.h"
#include	"Bullet.h"
#include	"Debug_Camera.h"
#include	"audio_clip.h"

static void Create_Bullet(XMFLOAT3& position, const XMFLOAT3& front);

PLAYER::PLAYER(void)
{
	//Model = new CMODEL();
	Model = new FBXmodel();
}

PLAYER::~PLAYER()
{
	Uninit();
}

void PLAYER::Init(void)
{
	{
		//string name = "asset/model/viranrifle.fbx";
		string name = "asset/model/kakunin_joint.fbx";

		Model->Load(name);
	}

	//Position = XMFLOAT3(0.f, 0.f, 0.f);

	{
		auto scene = CManager::Get_Instance()->Get_Scene();

		auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));

		GAME_OBJECT::Init();
	}

	HP = 100.0f;
}

void PLAYER::Draw(void)
{
	{
		XMMATRIX matrix= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		//Model->Draw();
		Model->Draw(matrix);
	}

	GAME_OBJECT::Draw();
}

void PLAYER::Draw_DPP(void)
{
	{
		XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		//Model->Draw_DPP();
		Model->Draw_DPP(matrix);
	}
}

void PLAYER::Update(float delta_time)
{
	const auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMVECTOR* vec = camera.lock()->Get_At();
	XMFLOAT3 rotate = *camera.lock()->Get_Rotation();

	XMVECTOR front_vec = *camera.lock()->Get_Front();
	XMFLOAT3 front;
	front_vec = XMVector3Normalize(front_vec);
	XMStoreFloat3(&front, front_vec);
	front.y = 0.0;

	XMFLOAT3 pos;

	XMStoreFloat3(&pos, *vec);

	//Position = pos;

	// カメラに合わせた回転
	//Rotation.y = rotate.y + 0.0f;

	// モデルの更新
	{
		//Model->Set_Position(Position);
		//Model->Set_Rotation(Rotation);
		//Model->Set_Scaling(Scaling);

		Model->Update(delta_time);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_SPACE))
	{
		XMFLOAT3 pos = Position + front * 2.0f;
		pos.y += 1.0;

		Create_Bullet(pos, front * 2.0f);
	}

	GAME_OBJECT::Update(delta_time);
}

void PLAYER::Uninit(void)
{
	SAFE_DELETE(Model);
}

void PLAYER::SetPosition(const XMFLOAT3 position)
{
	Position = position;
}

void PLAYER::SetScaling(const XMFLOAT3 scaling)
{
	Scaling = scaling;
}

void Create_Bullet(XMFLOAT3& position, const XMFLOAT3& front)
{
	auto scene = CManager::Get_Instance()->Get_Scene();

	auto bullets = scene->Get_Game_Objects<BULLET>();

	string name = "1";
	int cnt = 1;
	if (!bullets.empty())
	{
 		name = bullets.back()->Get_Object_Name();
 		ExtratNum(name);

		cnt = std::atoi(name.c_str());
		cnt++;
	}

	auto bullet = scene->Add_Game_Object<BULLET>(LAYER_NAME::GAMEOBJECT, "bullet" + to_string(cnt));

	bullet->Init();

	bullet->Set_Position(position);
	bullet->Set_Move_Vector(front);
}