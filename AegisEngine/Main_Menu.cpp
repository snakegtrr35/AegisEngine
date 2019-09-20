#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Math.h"

#include	"Fade.h"

string MAIN_MENU::Model_Name = "asset/model/herorifle.fbx";

static bool flag = false;
static short cnt = 0;

void MAIN_MENU::Init()
{
	// カメラ
	{
		CCamera* camera = Add_Game_Object<CCamera>(LAYER_NAME::BACKGROUND);

		camera->Set_Lenght(15.0f);
		camera->Set_Viewing_Angle(55.0f);

		//camera->Set_Move_Enable(false);
		//camera->Set_Rotate_Enable(false);
	}

	//プレイヤー
	{
		CMODEL* player = Add_Game_Object<CMODEL>(LAYER_NAME::GAMEOBJECT);

		//string name("asset/model/herorifle.fbx");
		//string name("asset/model/human02.fbx");
		string name("asset/model/untitled.fbx");
		//string name("asset/model/Dragon 2.5_fbx.fbx");

		XMFLOAT3 f3;

		player->Set_Object_Name("player");

		f3 = XMFLOAT3(0.f, 0.f, 0.f);
		player->Set_Position(f3);

		f3 = XMFLOAT3(0.f, 0.f, 0.f);
		player->Set_Rotation(f3);

		//f3 = XMFLOAT3(0.1f, 0.1f, 0.1f);
		f3 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		player->Set_Scaling(f3);

		player->Load(name);
	}

	{
		MESH_DOOM* pmd = Add_Game_Object<MESH_DOOM>(LAYER_NAME::GAMEOBJECT);

		pmd->Init();
	}

	{
		Add_Game_Object<GRID>(LAYER_NAME::GAMEOBJECT);
	}

	/*// テキスト画像
	{
		XMFLOAT2 pos(50.0f, 50.0f);
		TEXTS* text = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

		text->SetPosition(pos);

		text->SetSize(XMFLOAT4(20, 20, 20, 20));

		text->Set_Object_Name(string("text1"));

		text->Edit("");
	}*/

	cnt = 0;

	FADE::Start_FadeIn(60);
	flag = false;
}

void MAIN_MENU::Draw()
{
	for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
	{
		for (GAME_OBJECT* object : GameObjects[i])
		{
			object->Draw();
		}
	}
}

void MAIN_MENU::Update()
{
	SCENE::Update();

	/*{
		vector<CMODEL*> models = Get_Game_Objects<CMODEL>();

		for (auto player : models)
		{
			if ("player" == player->Get_Object_Name())
			{
				XMFLOAT3* rotation = player->Get_Rotation();

				rotation->y += 1.0f;

				player->Set_Rotation(rotation);
			}
		}
	}*/

	if (FADE::End_Fade())
	{
		if(flag)
			SCENE_MANAGER::Set_Scene<GAME>();

		flag = true;
	}
}

void MAIN_MENU::Uninit()
{
	SCENE::Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}