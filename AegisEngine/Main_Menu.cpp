#include	"Scene.h"
#include	"Main_Menu.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Math.h"
#include	"Field.h"
#include	"Mesh_Dome.h"
#include	"Mesh_Field.h"
#include	"Fade.h"

#include	"Timer.h"

#include	"FBXmodel.h"
#include	"Sprite_Animation.h"
#include	"Bounding_Aabb.h"
#include	"Bounding_Frustum.h"

string MAIN_MENU::Model_Name = "asset/model/herorifle.fbx";

static bool flag = false;

BOUNDING_FRUSTUM Bounding_Frustun;
BOUNDING_AABB AABB;

static bool isLoaded = false;
static std::mutex isLoadedMutex;


unique_ptr<SPRITE_ANIMATION> sprite_anime = nullptr;

void SetLockFlag()
{
	std::lock_guard<std::mutex>  lock(isLoadedMutex);
	isLoaded = true;
}

bool GetLockFlag()
{
	std::lock_guard<std::mutex>  lock(isLoadedMutex);
	return isLoaded;
}

string Replace_String(string& replacedStr, const string& from, const string& to)
{
	if (replacedStr.empty() || from.empty() || to.empty())
	{
		return replacedStr;
	}

	if (from.size() != to.size())
	{
		return replacedStr;
	}

	const UINT pos = (UINT)replacedStr.find(from);
	const UINT len = (UINT)from.length();

	if (replacedStr.size() < pos)
	{
		return replacedStr;
	}

	return replacedStr.replace(pos, len, to);
}

void Load(MAIN_MENU* scene)
{
	bool flag;

	{
		const type_info& id = typeid(scene);

		string name(id.name());

		// 置換
		Replace_String(name, "class ", "      ");
		Replace_String(name, "__ptr64", "       ");
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
			//auto player = scene->Add_Game_Object<PLAYER>(LAYER_NAME::GAMEOBJECT, "player");
		}

		{
			auto pmd = scene->Add_Game_Object<MESH_DOOM>(LAYER_NAME::BACKGROUND, "sky_doom");
		}

		{
			scene->Add_Game_Object<MESH_FIELD>(LAYER_NAME::GAMEOBJECT, "field");
		}

		{
			scene->Add_Game_Object<FIELD>(LAYER_NAME::GAMEOBJECT, "plane");
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
		//	XMFLOAT2 pos(300, 600);

		//	SPRITE* s = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "albed");

		//	s->SetPosition(pos);

		//	s->SetSize(XMFLOAT4(128, 128, 128, 128));
		//}

		//{
		//	XMFLOAT2 pos(300, 900);

		//	SPRITE* s = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "normal");

		//	s->SetPosition(pos);

		//	s->SetSize(XMFLOAT4(128, 128, 128, 128));
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

		{
			//Add_Game_Object<BOUNDING_AABB>(LAYER_NAME::GAMEOBJECT, "aabb");
		}

		{
			//Add_Game_Object<BOUNDING_OBB>(LAYER_NAME::GAMEOBJECT, "obb");
		}
	}

	//model = new FBXmodel();
	//model->Load("asset/model/SambaDancing2.fbx");

	Bounding_Frustun.Init();
	AABB.Init();

	scene->Init(true);

	Sleep(5000);

	SetLockFlag();
}

void MAIN_MENU::Init(bool)
{
	SCENE::Init();
}

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

	//cnt = 0;

	//FADE::Start_FadeIn(60);
	flag = false;
}

void MAIN_MENU::Draw()
{
	if (GetLockFlag())
	{
		sprite_anime.reset(nullptr);

		SCENE::Draw();

		Bounding_Frustun.Draw();
		AABB.Draw();

		auto m = XMMatrixIdentity();

		m = XMMatrixScaling(2.5, 2.5, 2.5);
	}
	else
	{
		sprite_anime->Draw();
	}
}

void MAIN_MENU::Draw_DPP()
{
	if (GetLockFlag())
	{
		sprite_anime.reset(nullptr);

		SCENE::Draw_DPP();

		Bounding_Frustun.Draw_DPP();
		AABB.Draw_DPP();

		auto m = XMMatrixIdentity();

		m = XMMatrixScaling(2.5, 2.5, 2.5);
	}
}

void MAIN_MENU::Update(float delta_time)
{
	if (GetLockFlag())
	{
		SCENE::Update(delta_time);
		Bounding_Frustun.Update(delta_time);
		AABB.Update(delta_time);

		{
			/*{
				vector<SPRITE*> sprites = Get_Game_Objects<SPRITE>();

				for (auto s : sprites)
				{
					if ("depth" == s->Get_Object_Name())
					{
						s->Set(CRenderer::Get());
					}
				}
			}*/

			/*{
				auto text = Get_Game_Object<TEXTS>("fps");

				auto time = TIMER::Get_FPS();

				text->Edit(to_string(time));
			}*/
		}

		/*if (FADE::End_Fade())
		{
			if(flag)
				SCENE_MANAGER::Set_Scene<GAME>();

			flag = true;
		}*/
	}
	else
	{
		sprite_anime->Update(delta_time);
	}
}

void MAIN_MENU::Uninit()
{
	static bool flag = true;

	if(flag)
	{
		const type_info& id = typeid(this);

		string name(id.name());

		// 置換
		Replace_String(name, "class ", "      ");
		Replace_String(name, "__ptr64", "       ");
		Replace_String(name, "*", " ");
		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());

		std::ofstream file(name + ".dat", std::ios::binary);

		bool f = file.is_open();

		cereal::BinaryOutputArchive archive(file);
		archive(*this);

		flag = false;
	}

	SCENE::Uninit();

	//SAFE_DELETE(model);

	Bounding_Frustun.Uninit();
	AABB.Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}