#include	"Scene.h"
#include	"Main_Menu.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"

#include	"Field.h"
#include	"Mesh_Dome.h"
#include	"Mesh_Field.h"
#include	"Fade.h"
#include	"Bill.h"
#include	"Player.h"
#include	"Bullet.h"
#include	"Billboard.h"

#include	"Timer.h"

#include	"FBXmodel.h"
#include	"Sprite_Animation.h"

#include	"Skybox.h"

static unique_ptr<SPRITE_ANIMATION> sprite_anime;

void MAIN_MENU::Init()
{
	{
		sprite_anime = make_unique<SPRITE_ANIMATION>();

		sprite_anime->SetPosition(XMFLOAT2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5));
		sprite_anime->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.5));

		sprite_anime->SetTexture("Load.png");

		sprite_anime->SetParam(10, 3, 1);

		sprite_anime->Init();
	}


	std::thread th(Load, this);

	th.detach();
}

void MAIN_MENU::Draw()
{
	static bool f = true;

	if (false == GetLockLoad())
	{
		if (f)
		{
			FADE::Start_FadeIn(60);
			f = false;
		}

		sprite_anime.reset(nullptr);

		SCENE::Draw();

		auto m = XMMatrixIdentity();

		m = XMMatrixScaling(2.5, 2.5, 2.5);

		//field->Draw();
	}
	else
	{
		sprite_anime->Draw();
	}
}

void MAIN_MENU::Draw_DPP()
{
	if (false == GetLockLoad())
	{
		sprite_anime.reset(nullptr);

		SCENE::Draw_DPP();

		auto m = XMMatrixIdentity();

		m = XMMatrixScaling(2.5, 2.5, 2.5);
	}
}

void MAIN_MENU::Update(float delta_time)
{
	if (false == GetLockLoad())
	{
		SCENE::Update(delta_time);
	}
	else
	{
		sprite_anime->Update(delta_time);
	}
}

void MAIN_MENU::Uninit()
{
	//field.reset(nullptr);

	{
		this->Delete_Game_Objects<BULLET>();
		this->Delete_Game_Objects<BILL_BOARD_ANIMATION>();

		this->Delete_Game_Objects<SKYBOX>();
	}

#ifdef _DEBUG
	static bool flag = true;

	if(flag)
	{
		const type_info& id = typeid(*this);

		string name(id.name());

		// 置換
		Replace_String(name, "class ", "      ");
		Replace_String(name, "*", " ");
		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());

		std::ofstream file(name + ".dat", std::ios::binary);

		bool f = file.is_open();

		cereal::BinaryOutputArchive archive(file);
		archive(*this);

		flag = false;
	}
#endif // _DEBUG

	SCENE::Uninit();

	//SAFE_DELETE(model);

	AUDIO_MANAGER::Stop_Sound_Object();
}

void MAIN_MENU::Load(SCENE* scene)
{
	bool flag;

	{
		const type_info& id = typeid(*scene);

		string name(id.name());

		// 置換
		Replace_String(name, "class ", "      ");
		Replace_String(name, "*", " ");
		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());

		std::ifstream file(name + ".dat", std::ios::binary);

		flag = file.is_open();

		if (flag)
		{
			cereal::BinaryInputArchive archive(file);
			archive(*scene);
		}
	}

	if (false == flag)
	{
		// カメラ
		{
			//auto camera = Add_Game_Object<CCamera>(LAYER_NAME::BACKGROUND, "camera");
			auto camera = scene->Add_Game_Object<DEBUG_CAMERA>(LAYER_NAME::BACKGROUND, "camera");

			camera->Set_Viewing_Angle(80.0f);

			//camera->Set_Move_Enable(false);
			//camera->Set_Rotate_Enable(false);
		}

		{
			auto player = scene->Add_Game_Object<PLAYER>(LAYER_NAME::GAMEOBJECT, "player");
		}

		{
			auto pmd = scene->Add_Game_Object<MESH_DOOM>(LAYER_NAME::BACKGROUND, "sky_doom");
		}

		{
			scene->Add_Game_Object<MESH_FIELD>(LAYER_NAME::GAMEOBJECT, "field");
		}

		/*{
			scene->Add_Game_Object<FIELD>(LAYER_NAME::GAMEOBJECT, "plane");
		}*/

		{
			auto bill = scene->Add_Game_Object<BILL>(LAYER_NAME::GAMEOBJECT, "bill");
		}

		//{
		//	//Add_Game_Object<GRID>(LAYER_NAME::BACKGROUND, "grid");
		//}

		/*{
			Add_Game_Object<POLYGON_3D>(LAYER_NAME::GAMEOBJECT, "cube");
		}*/

		//// テキスト画像
		//{
		//	XMFLOAT2 pos(50.0f, 50.0f);
		//	auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI, "text1");

		//	text->SetPosition(pos);

		//	text->SetSize(XMFLOAT4(20, 20, 20, 20));

		//	text->Edit("Hello HELL World!!!地球の未来にご奉仕するにゃん！");
		//}



		//// 画像
		//{
		//	XMFLOAT2 pos(300, 300);

		//	SPRITE* s = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "depth");

		//	s->SetPosition(pos);

		//	s->SetSize(XMFLOAT4(128, 128, 128, 128));

		//	s->Set_Object_Name("depth");
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

		/*{
			auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI, "fps");

			XMFLOAT2 pos(100.0f, 340.0f);

			text->SetPosition(pos);

			text->SetSize(XMFLOAT4(20, 20, 20, 20));

			text->Edit("0.000");
		}*/
	}

	//model = new FBXmodel();
	//model->Load("asset/model/SambaDancing2.fbx");

	scene->SCENE::Init();

	scene->SetLockLoad();
}