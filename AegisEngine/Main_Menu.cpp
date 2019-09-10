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
	// メインメニュー画面
	{
		XMFLOAT2 pos(400.0f, SCREEN_HEIGHT * 0.5f);

		SPRITE* menu1 = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

		menu1->SetPosition(pos);

		menu1->SetSize(XMFLOAT4(256, 256, 256, 256));

		menu1->SetTexture(string("asult_rifl.png"));

		menu1->Set_Object_Name("menu1");



		SPRITE* menu2 = menu1->Add_Child_Sptite(string("menu2"), menu1);

		menu2->SetTexture(string("Bazooka.png"));

		pos = XMFLOAT2(0.0f, pos.y - 180.0f);

		menu1->Set_Position_Child(string("menu2"), pos);

		menu2->SetSize(XMFLOAT4(256, 256, 256, 256));

		menu2->Set_Object_Name("menu2");


		SPRITE* go = menu1->Add_Child_Sptite(string("go"), menu1);

		go->SetTexture(string("go.png"));

		pos = XMFLOAT2(550.0f, pos.y + 100.0f);

		menu1->Set_Position_Child(string("go"), pos);

		go->SetSize(XMFLOAT4(64, 128, 64, 128));

		go->Set_Object_Name("go");

		COLOR color = COLOR(1.0f, 1.0f, 1.0f, 0.7f);
		go->SetColor(color);



		SPRITE* select = menu1->Add_Child_Sptite("select", menu1);

		select->SetTexture("select.png");

		select->SetSize(XMFLOAT4(50, 50, 50, 50));

		menu1->Set_Position_Child("select", XMFLOAT2(-310.0f, -50.0f));		// 1

		menu2->Set_Object_Name("arrow");
	}

	// カメラ
	{
		CCamera* camera = Add_Game_Object<CCamera>(LAYER_NAME::BACKGROUND);

		camera->Set_Lenght(15.0f);
		camera->Set_Viewing_Angle(55.0f);

		//camera->Set_Move_Enable(false);
		//camera->Set_Rotate_Enable(false);
	}

	// プレイヤー
	{
		CMODEL* player = Add_Game_Object<CMODEL>(LAYER_NAME::GAMEOBJECT);

		//string name("asset/model/herorifle.fbx");
		string name("asset/model/human.fbx");

		XMFLOAT3 f3;

		player->Set_Object_Name("player");

		f3 = XMFLOAT3(2.5f, 0.f, -4.5f);
		player->Set_Position(f3);

		f3 = XMFLOAT3(0.f, 0.f, 0.f);
		player->Set_Rotation(f3);

		f3 = XMFLOAT3(2.0f, 2.0f, 2.0f);
		player->Set_Scaling(f3);

		player->Load(name);
	}

	//// ガレージ
	//{
	//	CMODEL* garage = Add_Game_Object<CMODEL>(LAYER_NAME::GAMEOBJECT);

	//	garage->Set_Object_Name("garage");

	//	string name("asset/model/wall.fbx");

	//	garage->Load(name);

	//	garage->SetRotation(XMFLOAT3(90.0f, 90.0f, 0.f));

	//	garage->SetScaling(XMFLOAT3(2.0f, 2.0f, 2.0f));
	//}

	{
		MESH_DOOM* pmd = Add_Game_Object<MESH_DOOM>(LAYER_NAME::GAMEOBJECT);

		pmd->Init();
	}

	{
		Add_Game_Object<GRID>(LAYER_NAME::GAMEOBJECT);
	}

	cnt = 0;

	FADE::Start_FadeIn(60);
	flag = false;

	AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_BGM002, true);
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

	// メニューの更新
	{
		if (KEYBOARD::Trigger_Keyboard(VK_UP))
		{
			cnt--;
			cnt = Loop_Minus(cnt, 3);

			AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_SENTAKU, false);
		}

		if (KEYBOARD::Trigger_Keyboard(VK_DOWN))
		{
			cnt++;
			cnt = Loop_Plus(cnt, 3);

			AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_SENTAKU, false);
		}

		SPRITE* menu = Get_Game_Object<SPRITE>();

		list<CHILD_DATE> children = menu->Get_Child_Sptite();

		string menu1("menu2"), menu2("go"), menu3("select");

		for (auto child : children)
		{
			if (child.Name == menu2)
			{
				switch (cnt)
				{
					case 0:
						menu->Set_Position_Child("select", XMFLOAT2(-310.0f, -50.0f));		// 1番目
						break;

					case 1:
						menu->Set_Position_Child("select", XMFLOAT2(-310.0f, 320.0f));		// 2番目
						break;

					case 2:
						menu->Set_Position_Child("select", XMFLOAT2(350.0f, 455.0f));		// 3番目
						break;
				}
				break;
			}
		}

		if (KEYBOARD::Trigger_Keyboard(VK_RETURN))
		{
			vector<CMODEL*> models = Get_Game_Objects<CMODEL>();

			switch (cnt)
			{
				case 0:
					Model_Name.clear();
					Model_Name = "asset/model/herorifle.fbx";
					//Weapon_Name = "asset/model/Bullet.obj";
					break;

				case 1:
					Model_Name.clear();
					Model_Name = "asset/model/heroRoket.fbx";
					break;

				case 2:
					FADE::Start_FadeOut(60);
					break;
			}

			if (2 != cnt)
			{
				for (auto model : models)
				{
					if ("player" == model->Get_Object_Name())
					{
						model->Reload(Model_Name);

						break;
					}
				}
			}

			AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_KETTEI, false);
		}
	}

	{
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
	}

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