#include	"Scene.h"
#include	"Game.h"
#include	"Title.h"
#include	"Result.h"

#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Fade.h"
#include	"Enemy.h"
#include	"Score.h"
#include	"Mesh_Field.h"
#include	"Mesh_Dome.h"
#include	"Player.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"Sprite.h"
#include	"Text.h"
#include	"Sprite_Animation.h"

#include	"Scene_Manager.h"

static bool flag = false;
static bool flag1 = true;

static float hp;

static short cnt;

static unique_ptr<SPRITE_ANIMATION> sprite_anime = nullptr;

void GAME::Init()
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

void GAME::Draw()
{
	if (false == GetLockLoad())
	{
		SCENE::Draw();
	}
	else
	{
		sprite_anime->Draw();
	}
}

void GAME::Draw_DPP()
{
	if (false == GetLockLoad())
	{
		SCENE::Draw_DPP();
	}
	else
	{
		sprite_anime->Draw_DPP();
	}
}

void GAME::Update(float delta_time)
{
	if (false == GetLockLoad())
	{
		sprite_anime.reset(nullptr);

		SCENE::Update(delta_time);

		// ポーズ画面の切り替え
		if (KEYBOARD::Trigger_Keyboard(VK_Q))
		{
			vector<SPRITE*> sprites = Get_Game_Objects<SPRITE>();

			for (auto sprite : sprites)
			{
				if ("pause" == sprite->Get_Object_Name())
				{
					if (true == sprite->GetEnable())
					{
						sprite->SetEnable(false);
						sprite->Set_Enable_Child(false);
						End_Pause();
					}
					else
					{
						sprite->SetEnable(true);
						sprite->Set_Enable_Child(true);
						Start_Pause();
					}
					break;
				}
			}
		}

		// ポーズ画面の入力処理
		if (true == Get_PauseEnable())
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

			vector<SPRITE*> sprites = Get_Game_Objects<SPRITE>();

			for (auto sprite : sprites)
			{
				if ("pause" == sprite->Get_Object_Name())
				{
					switch (cnt)
					{
					case 0:
						sprite->Set_Position_Child("select", *sprite->GetPosition(), XMFLOAT2(-165.0f, -110.0f));	// 1番目
						break;

					case 1:
						sprite->Set_Position_Child("select", *sprite->GetPosition(), XMFLOAT2(-165.0f, 0.0f));		// 2番目
						break;

					case 2:
						sprite->Set_Position_Child("select", *sprite->GetPosition(), XMFLOAT2(-165.0f, 105.0f));	// 3番目
						break;
					}
					break;
				}
			}

			if (KEYBOARD::Trigger_Keyboard(VK_RETURN))
			{
				switch (cnt)
				{
				case 0:
				{
					vector<SPRITE*> sprites = Get_Game_Objects<SPRITE>();

					for (auto sprite : sprites)
					{
						if ("pause" == sprite->Get_Object_Name())
						{
							sprite->SetEnable(false);
							sprite->Set_Enable_Child(false);
							End_Pause();
							break;
						}
					}
				}
				break;

				case 1:
					SCENE_MANAGER::Set_Scene<TITLE>();
					break;

				case 2:
					CManager::Get_Instance()->GameEnd();
					break;
				}

				AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_KETTEI, false);
			}
		}

		// HPのUI
		{
			vector<SPRITE*> sprits = Get_Game_Objects<SPRITE>();

			COLOR color1 = COLOR(1.0f, 1.0f, 0.0f, 1.0f);
			COLOR color2 = COLOR(1.0f, 0.0f, 0.0f, 1.0f);

			for (auto sprite : sprits)
			{
				if (string("hp_ui") == sprite->Get_Object_Name())
				{
					auto children = sprite->Get_Child_Sptite();

					for (const auto& child : *children)
					{
						if (string("hp") == child->Name)
						{
							child->Child->SetSize(XMFLOAT4(LerpEx(-485.0f, 170.0f, hp, 0.0f, 100.0f), 25, 485, 25));

							if (hp <= 50.0f)
							{
								child->Child->SetColor(color1);
							}

							if (hp <= 20.0f)
							{
								child->Child->SetColor(color2);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		sprite_anime->Update(delta_time);
	}
}

void GAME::Uninit()
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

void GAME::Load(SCENE* scene)
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
		Add_Game_Object<DEBUG_CAMERA>(LAYER_NAME::BACKGROUND, "camera");

		// プレイヤー
		{
			PLAYER* player = Add_Game_Object<PLAYER>(LAYER_NAME::GAMEOBJECT, "player");
		}

		// レティクル画像
		{
			XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f);
			SPRITE* sprite = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "reticle");

			sprite->SetPosition(pos);

			sprite->SetTexture("Reticule.png");

			sprite->SetSize(XMFLOAT4(400, 400, 400, 400));
		}

		// メッシュフィールド
		{
			MESH_FIELD* mf = Add_Game_Object<MESH_FIELD>(LAYER_NAME::BACKGROUND, "feild");

			mf->SetTexture("asphalt01-pattern.jpg");
		}


		// メッシュドーム
		{
			MESH_DOOM* pmd = Add_Game_Object<MESH_DOOM>(LAYER_NAME::BACKGROUND, "doom");
			pmd->Init();

			XMFLOAT3 scaling = XMFLOAT3(2.0f, 2.0f, 2.0f);

			pmd->Set_Scaling(scaling);
		}

		// 敵
		{
			string name("enemy");
			string number;

			for (int i = 0; i < 5; i++)
			{
				number = to_string(i);

				ENEMY* enemy = Add_Game_Object<ENEMY>(LAYER_NAME::GAMEOBJECT, name + number);
				enemy->SetPosition(XMFLOAT3((float)(-10.0f + i * 5.0f), 0.0f, 0.0f));
				enemy->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));

				auto component = enemy->Get_Component();

				number.clear();
			}
		}

		// スコア
		{
			XMFLOAT3 pos = XMFLOAT3(500.0f, 37.0f, 0.0f);

			SCORE* score = Add_Game_Object<SCORE>(LAYER_NAME::UI, "score");
			score->Set_Position(&pos);
			score->Set_Additional(1);
		}

		// HPのUI
		{
			XMFLOAT2 pos(50.0f, SCREEN_HEIGHT * 0.5f);
			SPRITE* sprite = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "hp");

			sprite->Set_Object_Name("hp_ui");

			sprite->SetPosition(pos);

			sprite->SetTexture("hp_rod.png");

			sprite->SetSize(XMFLOAT4(300, 32, 500, 32));

			COLOR color = COLOR(0.0f, 1.0f, 1.0f, 1.0f);

			SPRITE* hp = sprite->Add_Child_Sptite("hp");

			hp->SetTexture("hp.png");

			hp->SetPosition(pos);

			hp->SetColor(color);

			hp->SetSize(XMFLOAT4(170, 25, 485, 25));	// HP MAX 655pixel
			//hp->SetSize(XMFLOAT4(-485, 25, 485, 25));	// HP 0

			hp->SetSize(XMFLOAT4(LerpEx(-485.0f, 170.0f, 100.0f, 0.0f, 100.0f), 25, 485, 25));
		}

		// 弾のUI
		{
			//XMFLOAT2 pos(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 70.0f);
			XMFLOAT2 pos(SCREEN_WIDTH - 550.0f, SCREEN_HEIGHT - 70.0f);

			COLOR color = COLOR(0.0f, 1.0f, 1.0f, 1.0f);

			TEXTS* sprite = Add_Game_Object<TEXTS>(LAYER_NAME::UI, "bullet_ui");

			sprite->SetPosition(pos);

			sprite->SetSize(XMFLOAT4(20, 20, 20, 20));

			sprite->SetColor(color);

			//sprite->Edit(string("30 / 30"));
			sprite->Edit(string("infinite / 30"));

			// 弾のアイコン
			SPRITE* bullet_icon = sprite->Add_Child_Sptite("bullet_icon");

			bullet_icon->SetTexture("bullet_icon.png");

			pos.x += -60.0f;

			bullet_icon->SetPosition(pos);

			bullet_icon->SetSize(XMFLOAT4(50, 16, 50, 16));
		}

		// ポーズ画面のUI
		{
			XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
			COLOR color = COLOR(1.0f, 1.0f, 1.0f, 0.85f);

			SPRITE* sprite = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "pause");

			sprite->SetPosition(pos);

			sprite->SetTexture("pause.png");

			sprite->SetSize(XMFLOAT4(200, 200, 200, 200));

			sprite->SetColor(color);

			sprite->SetEnable(false);

			SPRITE* select = sprite->Add_Child_Sptite("select");

			select->SetTexture("select.png");

			select->SetSize(XMFLOAT4(30, 30, 30, 30));

			sprite->Set_Position_Child("select", *sprite->GetPosition(), XMFLOAT2(-165.0f, -110.0f));		// 1番目

			//sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, 0.0f));		// 2番目

			//sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, 105.0f));		// 3番目
		}
	}

	scene->SCENE::Init();

	scene->SetLockLoad();
}