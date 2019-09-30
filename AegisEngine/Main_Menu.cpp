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
		CCamera* camera = Add_Game_Object<CCamera>(LAYER_NAME::BACKGROUND, "camera");

		camera->Set_Lenght(15.0f);
		camera->Set_Viewing_Angle(55.0f);

		//camera->Set_Move_Enable(false);
		//camera->Set_Rotate_Enable(false);
	}

	//プレイヤー
	{
		//CMODEL* player = Add_Game_Object<CMODEL>(LAYER_NAME::GAMEOBJECT);

		////string name("asset/model/herorifle.fbx");
		//string name("asset/model/human01_Stop.fbx");
		////string name("asset/model/untitled.fbx");
		////string name("asset/model/Dragon 2.5_fbx.fbx");

		//XMFLOAT3 f3;

		//player->Set_Object_Name("player");

		//f3 = XMFLOAT3(0.f, 0.f, 0.f);
		//player->Set_Position(f3);

		//f3 = XMFLOAT3(0.f, 0.f, 0.f);
		//player->Set_Rotation(f3);

		////f3 = XMFLOAT3(0.1f, 0.1f, 0.1f);
		//f3 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//player->Set_Scaling(f3);

		//player->Load(name);
	}

	{
		auto player = Add_Game_Object<PLAYER>(LAYER_NAME::GAMEOBJECT, "player");
	}

	{
		MESH_DOOM* pmd = Add_Game_Object<MESH_DOOM>(LAYER_NAME::GAMEOBJECT, "sky_doom");

		//pmd->Init();
	}

	{
		Add_Game_Object<GRID>(LAYER_NAME::GAMEOBJECT, "grid");
	}

	/*// テキスト画像
	{
		XMFLOAT2 pos(50.0f, 50.0f);
		TEXTS* text = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

		text->SetPosition(pos);

		text->SetSize(XMFLOAT4(20, 20, 20, 20));

		text->Set_Object_Name(string("text1"));

		string time = to_string(Time);

		text->Edit(time);
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

	{
		auto player = Get_Game_Object<CMODEL>("player");

		if (KEYBOARD::Trigger_Keyboard(VK_1))
		{
			string name("asset/model/human01_Stop.fbx");

			//player->Reload(name);
		}

		if (KEYBOARD::Trigger_Keyboard(VK_2))
		{
			string name("asset/model/human01_Walk.fbx");

			//player->Reload(name);
		}

		if (KEYBOARD::Trigger_Keyboard(VK_3))
		{
			string name("asset/model/human01_Jump.fbx");

			//player->Reload(name);
		}
	}

	if (FADE::End_Fade())
	{
		if(flag)
			SCENE_MANAGER::Set_Scene<GAME>();

		flag = true;
	}

	if (KEYBOARD::Trigger_Keyboard(VK_TAB))
	{
		CRenderer::Change_Window_Mode();
	}
}

void MAIN_MENU::Uninit()
{
	SCENE::Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}