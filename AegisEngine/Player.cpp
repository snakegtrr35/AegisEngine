//#include	"GameObject.h"
#include	"Player.h"

#include	"manager.h"
#include	"Scene.h"

//#include	"ModelLoader.h"
//#include	"FBXmodel.h"
#include	"Model.h"

#include	"Input.h"
#include	"Collision.h"
#include	"Bounding_Aabb.h"
#include	"Bullet.h"
#include	"Debug_Camera.h"
#include	"audio_clip.h"

using namespace aegis;

static void Create_Bullet(Vector3& position, const Vector3& front);

PLAYER::PLAYER(void)
{
	//Model = std::make_unique<CMODEL>();
	//Model = new FBXmodel();
}

PLAYER::~PLAYER()
{
	Uninit();
}

void PLAYER::Init(void)
{
	auto scene = CManager::Get_Instance()->Get_Scene();

	{
		aegis::string name = "viranrifle.fbx";
		//string name = "asset/model/kakunin_joint.fbx";

		//Model->Load(name);

		//auto model = Get_Component()->Add_Component<MODEL>(scene->Get_Game_Object(this));
		auto model = this->AddComponent<MODEL>();
		
		model->Set_Model_Name(name);
	}

	//Position = Vector3(0.f, 0.f, 0.f);

	{
		//auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));
		auto aabb = this->AddComponent<BOUNDING_AABB>();
		
		GameObject::Init();
	}

	HP = 100.0f;
}

void PLAYER::Draw(void)
{
	//if (nullptr == Model)
	//	return;
	//
	//{
	//	Vector3 position = *Get_Transform().Get_Position();
	//	Vector3 rotate = *Get_Transform().Get_Rotation();
	//	Vector3 scale = *Get_Transform().Get_Scaling();
	//
	//	XMMATRIX matrix= XMMatrixScaling(scale.x, scale.y, scale.z);
	//	matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
	//	matrix *= XMMatrixTranslation(position.x, position.y, position.z);
	//
	//	Model->Draw();
	//	//Model->Draw(matrix);
	//}

	GameObject::Draw();
}

void PLAYER::Draw_Shadow(void)
{
	//if (nullptr == Model)
	//	return;
	//
	//{
	//	Vector3 position = *Get_Transform().Get_Position();
	//	Vector3 rotate = *Get_Transform().Get_Rotation();
	//	Vector3 scale = *Get_Transform().Get_Scaling();
	//
	//	XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	//	matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
	//	matrix *= XMMatrixTranslation(position.x, position.y, position.z);
	//
	//	Model->Draw_Shadow();
	//}

	GameObject::Draw_Shadow();
}

void PLAYER::Draw_DPP(void)
{
	//if (nullptr == Model)
	//	return;
	//
	//Vector3 position = *Get_Transform().Get_Position();
	//Vector3 rotate = *Get_Transform().Get_Rotation();
	//Vector3 scale = *Get_Transform().Get_Scaling();
	//
	//XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	//matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
	//matrix *= XMMatrixTranslation(position.x, position.y, position.z);
	//
	//Model->Draw_DPP();

	//GameObject::Draw_DPP();
}

void PLAYER::Update(float delta_time)
{
	const auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMVECTOR* vec = camera.lock()->Get_At();
	Vector3 rotate = camera.lock()->Get_Transform().Get_Rotation();

	XMVECTOR front_vec = *camera.lock()->Get_Front();
	Vector3 front;
	front_vec = XMVector3Normalize(front_vec);
	XMStoreFloat3(&front, front_vec);
	front.y = 0.0;

	Vector3 pos;

	XMStoreFloat3(&pos, *vec);

	//Get_Transform().Set_Position(pos);

	// カメラに合わせた回転
	//Get_Transform().Get_Rotation()->y = rotate.y + 0.0f;

	//// モデルの更新
	//{
	//	Model->Get_Transform().Set_Position(Get_Transform().Get_Position());
	//	Model->Get_Transform().Set_Rotation(Get_Transform().Get_Rotation());
	//	Model->Get_Transform().Set_Scaling(Get_Transform().Get_Scaling());
	//
	//	Model->Update(delta_time);
	//}

	if (KEYBOARD::Trigger_Keyboard(VK_SPACE))
	{
		Vector3 pos = Get_Transform().Get_Position() + front * 2.0f;
		pos.y += 1.0;
	
		Create_Bullet(pos, front * 2.0f);
	}

	GameObject::Update(delta_time);
}

void PLAYER::Uninit(void)
{
	GameObject::Uninit();
}

void PLAYER::SetPosition(Vector3& position)
{
	Get_Transform().Set_Position(position);
}

void PLAYER::SetScaling(Vector3& scaling)
{
	Get_Transform().Set_Scaling(scaling);
}

void Create_Bullet(Vector3& position, const Vector3& front)
{
	auto scene = CManager::Get_Instance()->Get_Scene();

	auto bullets = scene->Get_Game_Objects<BULLET>();

	aegis::string name;
	int cnt = 1;
	if (!bullets.empty())
	{
 		name = bullets.back()->Get_Object_Name();
 		ExtratNum(name);

		cnt = std::atoi(name.c_str());
		cnt++;
	}

	auto bullet = scene->Add_Game_Object<BULLET>(LAYER_NAME::GAMEOBJECT, aegis::string("bullet" + std::to_string(cnt)));

	bullet->Get_Transform().Set_Position(position);
	bullet->Set_Move_Vector(front);
}