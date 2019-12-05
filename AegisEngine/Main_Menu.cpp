#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Math.h"

#include	"Fade.h"

#include	"Timer.h"

string MAIN_MENU::Model_Name = "asset/model/herorifle.fbx";

static bool flag = false;
static short cnt = 0;

void MAIN_MENU::Init()
{
	// カメラ
	{
		//auto camera = Add_Game_Object<CCamera>(LAYER_NAME::BACKGROUND, "camera");
		auto camera = Add_Game_Object<DEBUG_CAMERA>(LAYER_NAME::BACKGROUND, "camera");

		camera->Set_Viewing_Angle(80.0f);

		//camera->Set_Move_Enable(false);
		//camera->Set_Rotate_Enable(false);
	}

	////プレイヤー
	//{
	//	CMODEL* player = Add_Game_Object<CMODEL>(LAYER_NAME::GAMEOBJECT, "player");

	//	//string name("asset/model/herorifle.fbx");
	//	string name("asset/model/human01_Stop.fbx");
	//	//string name("asset/model/untitled.fbx");
	//	//string name("asset/model/Dragon 2.5_fbx.fbx");

	//	XMFLOAT3 f3;

	//	f3 = XMFLOAT3(0.f, 0.f, 0.f);
	//	player->Set_Position(f3);

	//	f3 = XMFLOAT3(0.f, 0.f, 0.f);
	//	player->Set_Rotation(f3);

	//	//f3 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	//	f3 = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//	player->Set_Scaling(f3);

	//	player->Load(name);
	//}

	{
		auto player = Add_Game_Object<PLAYER>(LAYER_NAME::GAMEOBJECT, "player");
	}

	{
		auto pmd = Add_Game_Object<MESH_DOOM>(LAYER_NAME::BACKGROUND, "sky_doom");
	}

	{
		//Add_Game_Object<GRID>(LAYER_NAME::BACKGROUND, "grid");
	}

	{
		//Add_Game_Object<FIELD>(LAYER_NAME::GAMEOBJECT, "field");

		Add_Game_Object<MESH_FIELD>(LAYER_NAME::GAMEOBJECT, "field");
	}

	{
		//Add_Game_Object<POLYGON_3D>(LAYER_NAME::GAMEOBJECT, "cube");
	}

	//// テキスト画像
	//{
	//	XMFLOAT2 pos(50.0f, 50.0f);
	//	auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

	//	text->SetPosition(pos);

	//	text->SetSize(XMFLOAT4(20, 20, 20, 20));

	//	text->Set_Object_Name(string("text1"));

	//	//string time = to_string(Time);

	//	text->Edit("Hello HELL World!!!地球の未来にご奉仕するにゃん！");
	//}



	//// 画像
	//{
	//	XMFLOAT2 pos(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.2f);

	//	SPRITE* title = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

	//	title->SetPosition(pos);

	//	title->SetSize(XMFLOAT4(200, 200, 200, 200));

	//	title->SetTexture(string("UVCheckerMap01-512.png"));

	//	title->Set_Object_Name("sprite");
	//}

	//{
	//	auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

	//	XMFLOAT2 pos(100.0f, 300.0f);

	//	text->SetPosition(pos);

	//	text->SetSize(XMFLOAT4(20, 20, 20, 20));

	//	text->Set_Object_Name(string("delta_time"));

	//	//string time = to_string(Time);

	//	text->Edit("0.000");
	//}

	//{
	//	auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

	//	XMFLOAT2 pos(100.0f, 340.0f);

	//	text->SetPosition(pos);

	//	text->SetSize(XMFLOAT4(20, 20, 20, 20));

	//	text->Set_Object_Name(string("fps"));

	//	text->Edit("0.000");
	//}

	cnt = 0;

	FADE::Start_FadeIn(60);
	flag = false;
}

void MAIN_MENU::Draw()
{
	SCENE::Draw();
}

void MAIN_MENU::Update(float delta_time)
{
	SCENE::Update(delta_time);

	/*{
		static char cnt = 0;

		if (cnt == 3)
		{
			{
				auto text = Get_Game_Object<TEXTS>("delta_time");

				auto time = TIMER::Get_DeltaTime();

				text->Edit(to_string(time));
			}

			{
				auto text = Get_Game_Object<TEXTS>("fps");

				auto time = TIMER::Get_DeltaTime();

				text->Edit(to_string(1 / time));
			}

			cnt = 0;
		}

		cnt++;
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