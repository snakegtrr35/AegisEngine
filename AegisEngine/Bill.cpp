#include	"GameObject.h"
#include	"Bill.h"
#include	"Model.h"
#include	"Component.h"

#include	"Bounding_Aabb.h"

#include	"manager.h"
#include	"Scene.h"
#include	"Scene_Manager.h"

#include	"Model.h"

using namespace aegis;

BILL::BILL()
{
}

BILL::~BILL()
{
	Uninit();
}

void BILL::Init()
{
	Vector3 pos(10, 0, 0);

	Get_Transform().Set_Position(pos);

	std::string name("bill01.fbx");

	auto scene = CManager::Get_Instance()->Get_Scene();

	auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));

	//aabb->Set_Position(Position);
	//aabb->Set_Radius(XMFLOAT3(10, 10, 10));

	{
		auto model = Get_Component()->Add_Component<MODEL>(scene->Get_Game_Object(this));

		model->Set_Model_Name(name);
	}

	GameObject::Init();
}

void BILL::Draw()
{
	GameObject::Draw();
}

void BILL::Draw_Shadow()
{
	GameObject::Draw_Shadow();
}

void BILL::Draw_DPP()
{
	//GameObject::Draw_DPP();
}

void BILL::Update(float delta_time)
{
	GameObject::Update(delta_time);
}

void BILL::Uninit()
{
}