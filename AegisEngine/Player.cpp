#include	"GameObject.h"
#include	"Player.h"

#include	"manager.h"
#include	"Scene.h"

#include	"ModelLoader.h"
//#include	"FBXmodel.h"

#include	"Input.h"
#include	"Collision.h"
#include	"Bounding_Aabb.h"
#include	"Bullet.h"
#include	"Debug_Camera.h"
#include	"audio_clip.h"

static void Create_Bullet(XMFLOAT3& position, const XMFLOAT3& front);

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
		string name = "asset/model/viranrifle.fbx";
		//string name = "asset/model/kakunin_joint.fbx";

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
		XMFLOAT3 position = *Get_Transform().Get_Position();
		XMFLOAT3 rotate = *Get_Transform().Get_Rotation();
		XMFLOAT3 scale = *Get_Transform().Get_Scaling();

		XMMATRIX matrix= XMMatrixScaling(scale.x, scale.y, scale.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		matrix *= XMMatrixTranslation(position.x, position.y, position.z);

		Model->Draw();
		//Model->Draw(matrix);
	}

	GAME_OBJECT::Draw();
}

void PLAYER::Draw_DPP(void)
{
	{
		XMFLOAT3 position = *Get_Transform().Get_Position();
		XMFLOAT3 rotate = *Get_Transform().Get_Rotation();
		XMFLOAT3 scale = *Get_Transform().Get_Scaling();

		XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		matrix *= XMMatrixTranslation(position.x, position.y, position.z);

		Model->Draw_DPP();
		//Model->Draw_DPP(matrix);
	}
}

void PLAYER::Update(float delta_time)
{
	const auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMVECTOR* vec = camera.lock()->Get_At();
	XMFLOAT3 rotate = *camera.lock()->Get_Transform().Get_Rotation();

	XMVECTOR front_vec = *camera.lock()->Get_Front();
	XMFLOAT3 front;
	front_vec = XMVector3Normalize(front_vec);
	XMStoreFloat3(&front, front_vec);
	front.y = 0.0;

	XMFLOAT3 pos;

	XMStoreFloat3(&pos, *vec);

	Get_Transform().Set_Position(pos);

	// カメラに合わせた回転
	Get_Transform().Get_Rotation()->y = rotate.y + 0.0f;

	// モデルの更新
	{
		Model->Get_Transform().Set_Position(Get_Transform().Get_Position());
		Model->Get_Transform().Set_Rotation(Get_Transform().Get_Rotation());
		Model->Get_Transform().Set_Scaling(Get_Transform().Get_Scaling());

		Model->Update(delta_time);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_SPACE))
	{
		XMFLOAT3 pos = *Get_Transform().Get_Position() + front * 2.0f;
		pos.y += 1.0;

		Create_Bullet(pos, front * 2.0f);
	}

	GAME_OBJECT::Update(delta_time);
}

void PLAYER::Uninit(void)
{
	SAFE_DELETE(Model);
}

void PLAYER::SetPosition(XMFLOAT3& position)
{
	Get_Transform().Set_Position(position);
}

void PLAYER::SetScaling(XMFLOAT3& scaling)
{
	Get_Transform().Set_Scaling(scaling);
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

	bullet->Get_Transform().Set_Position(position);
	bullet->Set_Move_Vector(front);
}