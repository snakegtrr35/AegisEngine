#include	"Bill.h"
#include	"ModelLoader.h"
#include	"Component.h"

#include	"Bounding_Aabb.h"

#include	"manager.h"
#include	"Scene.h"
#include	"Scene_Manager.h"

BILL::BILL()
{
}

BILL::~BILL()
{
	Uninit();
}

void BILL::Init()
{
	Model = make_unique<CMODEL>();

	Model->Load("asset/model/bill01.fbx");

	auto scene = CManager::Get_Instance()->Get_Scene();

	auto aabb = Get_Component()->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(this));

	//aabb->Set_Position(Position);
	//aabb->Set_Radius(XMFLOAT3(10, 10, 10));

	GAME_OBJECT::Init();
}

void BILL::Draw()
{
	{
		Model->Draw();
	}

	GAME_OBJECT::Draw();
}

void BILL::Draw_DPP()
{
	{
		Model->Draw_DPP();
	}
}

void BILL::Update(float delta_time)
{
	Model->Set_Position(Position);
	Model->Set_Rotation(Rotation);
	Model->Set_Scaling(Scaling);

	Model->Update(delta_time);

	GAME_OBJECT::Update(delta_time);
}

void BILL::Uninit()
{
	Model->Uninit();
	Model.reset(nullptr);
}