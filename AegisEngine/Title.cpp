#include	"Scene.h"
#include	"Title.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Fade.h"
#include	"Timer.h"

#include	"Sprite_Animation.h"

#include	"Scene_Manager.h"
#include	"Main_Menu.h"
#include	"Game.h"

static bool flag = false;

static unique_ptr<SPRITE_ANIMATION> sprite_anime = nullptr;

void TITLE::Init()
{
	{
		sprite_anime = make_unique<SPRITE_ANIMATION>();

		sprite_anime->SetPosition(XMFLOAT2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5));
		sprite_anime->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.5));

		sprite_anime->SetTexture("Load.png");

		sprite_anime->SetParam(10, 3, 1);

		sprite_anime->Init();
	}

	std::thread th(&Load, this);

	th.detach();

	//cnt = 0;

	//FADE::Start_FadeIn(60);
}

void TITLE::Draw()
{
	if (false == GetLockLoad())
	{
		sprite_anime.reset(nullptr);

		SCENE::Draw();

		auto m = XMMatrixIdentity();

		m = XMMatrixScaling(2.5, 2.5, 2.5);
	}
	else
	{
		sprite_anime->Draw();
	}
}

void TITLE::Draw_DPP()
{
	if (false == GetLockLoad())
	{
		sprite_anime.reset(nullptr);

		SCENE::Draw_DPP();

		auto m = XMMatrixIdentity();

		m = XMMatrixScaling(2.5, 2.5, 2.5);
	}
}

void TITLE::Update(float delta_time)
{
	if (false == GetLockLoad())
	{
		SCENE::Update(delta_time);

		// メニューの選択操作
		{
			static short cnt = 0;

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

			vector<SPRITE*> sprites = Get_Game_Objects<SPRITE>();

			for (auto sprite : sprites)
			{
				if ("title" == sprite->Get_Object_Name())
				{
					switch (cnt)
					{
					case 0:
						sprite->Set_Position_Child("arrow", *sprite->GetPosition(), XMFLOAT2(0.0f, 65.0f + 0.0f));		// 1番目
						break;

					case 1:
						sprite->Set_Position_Child("arrow", *sprite->GetPosition(), XMFLOAT2(0.0f, 65.0f + 140.0f));	// 2番目
						break;

					case 2:
						sprite->Set_Position_Child("arrow", *sprite->GetPosition(), XMFLOAT2(0.0f, 65.0f + 270.0f));	// 3番目
						break;

						//case 3:
						//	sprite->Set_Position_Child("arrow", XMFLOAT2(0.0f, 65.0f + 410.0f));	// 4番目
						//	break;
					}

					if (KEYBOARD::Trigger_Keyboard(VK_RETURN))
					{
						switch (cnt)
						{
						case 0:
							FADE::Start_FadeOut(60);
							break;

						case 1:
							//FADE::Start_FadeOut(60);
							break;

						case 2:
							CManager::Get_Instance()->GameEnd();//
							break;
						}

						AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_KETTEI, false);
					}

					break;
				}
			}
		}

		{
			if (FADE::End_Fade())
			{
				SCENE_MANAGER::Set_Scene<MAIN_MENU>();
				//SCENE_MANAGER::Set_Scene<GAME>();
			}
		}
	}
	else
	{
		sprite_anime->Update(delta_time);
	}
}

void TITLE::Uninit()
{

#ifdef _DEBUG
	static bool flag = true;

	if (flag)
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

	AUDIO_MANAGER::Stop_Sound_Object();
}

void TITLE::Load(SCENE* scene)
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
		// タイトル画面
		{
			XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

			SPRITE* title = scene->Add_Game_Object<SPRITE>(LAYER_NAME::UI, "title");

			title->SetPosition(pos);

			title->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f));

			title->SetTexture(string("title.png"));

			SPRITE* arrow = title->Add_Child_Sptite("arrow");

			arrow->SetTexture(string("arrow.png"));

			title->Set_Position_Child("arrow", *title->GetPosition(), XMFLOAT2(0.0f, 65.0f + 140.0f));

			arrow->SetSize(XMFLOAT4(40.0f, 330.0f, 40.0f, 330.0f));

			arrow->Set_Object_Name("arrow");
		}
	}

	scene->SCENE::Init();

	scene->SetLockLoad();
}