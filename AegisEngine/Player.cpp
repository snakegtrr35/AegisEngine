#include	"manager.h"
#include	"Scene.h"

#include	"ModelLoader.h"
//#include	"FBXmodel.h"

#include	"Input.h"
#include	"Collision.h"
#include	"Player.h"
#include	"Bounding_Aabb.h"
#include	"Bullet.h"
#include	"Debug_Camera.h"

#include	"audio_clip.h"

static void Create_Bullet(XMFLOAT3& position, const XMFLOAT3& front);

PLAYER::PLAYER(void)
{
	Model = new CMODEL();
	//Model = new FBXmodel();

	HP = 100.0f;
}

PLAYER::~PLAYER()
{
	Uninit();
}

void PLAYER::Init(void)
{
	{
		//string name = "asset/model/Player.fbx";
		string name = "asset/model/viranrifle.fbx";
		//string name = "asset/model/kakunin_joint.fbx";

		Model->Load(name);
	}

	{
		auto scene = CManager::Get_Instance()->Get_Scene();

		auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));

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
	XMFLOAT3 rotate = *camera.lock()->Get_Rotation();

	XMVECTOR front_vec = *camera.lock()->Get_Front();
	XMFLOAT3 front;
	front_vec = XMVector3Normalize(front_vec);
	XMStoreFloat3(&front, front_vec);

	XMFLOAT3 pos;

	XMStoreFloat3(&pos, *vec);

	Position = pos;
	// メッシュフィールドとの当たり判定
	//MESH_FIELD* pfield = CManager::Get_Scene()->Get_Game_Object<MESH_FIELD>();
	//if (nullptr != pfield)
	//	Position.y = pfield->Get_Height(Position);

	// カメラに合わせた回転
	Rotation.y = rotate.y + 0.0f;

	// モデルの更新
	{
		Model->Set_Position(Position);
		Model->Set_Rotation(Rotation);
		Model->Set_Scaling(Scaling);

		Model->Update(delta_time);
	}

	if (KEYBOARD::Trigger_Keyboard(VK_SPACE))
	{
		XMFLOAT3 pos = Position + front * 2.0f;

		Create_Bullet(pos, front * 2.0f);
	}

	//HP = clamp(HP -= 0.1f, 0.f, 100.0f);

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

void Create_Bullet(XMFLOAT3& position, const XMFLOAT3& front)
{
	auto scene = CManager::Get_Instance()->Get_Scene();

	auto bullets = scene->Get_Game_Objects<BULLET>();

	auto bullet = scene->Add_Game_Object<BULLET>(LAYER_NAME::GAMEOBJECT, "bullet" + to_string(bullets.size() + 1));

	bullet->Init();

	bullet->Set_Position(position);
	bullet->Set_Move_Vector(front);
}