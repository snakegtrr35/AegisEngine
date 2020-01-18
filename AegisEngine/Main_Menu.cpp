#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Math.h"

#include	"Fade.h"

#include	"Timer.h"

#include	"FBXmodel.h"

#include	"Bounding_Frustum.h"

string MAIN_MENU::Model_Name = "asset/model/herorifle.fbx";

static bool flag = false;

static FBXmodel* model;
static BOUNDING_FRUSTUM Frustun;
BOUNDING_AABB AABB;

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

	const UINT pos = replacedStr.find(from);
	const UINT len = from.length();

	if (replacedStr.size() < pos)
	{
		return replacedStr;
	}

	return replacedStr.replace(pos, len, to);
}


void MAIN_MENU::Init()
{
	bool flag;

	{
		const type_info& id = typeid(this);

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
			archive(*this);
		}
	}

	if (false == flag)
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

		//{
		//	//Add_Game_Object<GRID>(LAYER_NAME::BACKGROUND, "grid");
		//}

		{
			Add_Game_Object<MESH_FIELD>(LAYER_NAME::GAMEOBJECT, "field");
		}

		//{
		//	Add_Game_Object<POLYGON_3D>(LAYER_NAME::GAMEOBJECT, "cube");
		//}

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

	model = new FBXmodel();
	model->Load("asset/model/SambaDancing2.fbx");

	Frustun.Init();
	AABB.Init();

	SCENE::Init();

	//cnt = 0;

	//FADE::Start_FadeIn(60);
	flag = false;
}

void MAIN_MENU::Draw()
{
	SCENE::Draw();

	Frustun.Draw();
	AABB.Draw();

	auto m = XMMatrixIdentity();

	m = XMMatrixScaling(2.5, 2.5, 2.5);

	model->Draw(m);
}

void MAIN_MENU::Update(float delta_time)
{
	SCENE::Update(delta_time);

	model->Update(delta_time);
	Frustun.Update(delta_time);

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

	SAFE_DELETE(model);
	Frustun.Uninit();
	AABB.Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}