#include	"Scene.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Fade.h"

#include	"Axis.h"

static bool flag = false;
static bool flag1 = true;

static float hp;

static short cnt;

void GAME::Init()
{
	// カメラ
	Add_Game_Object<CCamera>(LAYER_NAME::BACKGROUND);

	// プレイヤー
	{
		PLAYER* player = Add_Game_Object<PLAYER>(LAYER_NAME::GAMEOBJECT);
	}

	// レティクル画像
	{
		XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f);
		SPRITE* sprite = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

		sprite->SetPosition(pos);

		sprite->SetTexture("Reticule.png");

		sprite->SetSize(XMFLOAT4(400, 400, 400, 400));
	}

	// メッシュフィールド
	{
		MESH_FIELD* mf = Add_Game_Object<MESH_FIELD>(LAYER_NAME::BACKGROUND);

		mf->SetTexture("asphalt01-pattern.jpg");
	}


	// メッシュドーム
	{
		MESH_DOOM* pmd = Add_Game_Object<MESH_DOOM>(LAYER_NAME::BACKGROUND);
		pmd->Init();

		XMFLOAT3 scaling = XMFLOAT3(2.0f, 2.0f, 2.0f);

		pmd->Set_Scaling(scaling);
	}

	// 敵
	{
		PROBABILITY_DATE date;

		date.Attack_Probability = 0.01f;
		date.Lenght = 3.0f;
		date.Move_Lenght = 1.0f;
		date.RL_Probability = 0.01f;
		date.RL = true;

		string name("enemy");
		string number;

		for (int i = 0; i < 2; i++)
		{
			number = to_string(i);

			ENEMY* enemy = Add_Game_Object<ENEMY>(LAYER_NAME::GAMEOBJECT);
			enemy->SetPosition(XMFLOAT3((float)(-10.0f + i * 5.0f), 0.0f, 0.0f));
			enemy->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));

			enemy->Set_Date(date);

			name = name + number;

			enemy->Set_Object_Name(name);

			auto component = enemy->Get_Component();

			//component->Add_Component<AXIS_COMPONENT>();

			name.pop_back();
			number.clear();
		}

		/*for (int i = 0 + 2; i < 2 + 2; i++)
		{
			number = to_string(i);

			ENEMY* enemy = Add_Game_Object<ENEMY>(LAYER_NAME::GAMEOBJECT);
			enemy->SetPosition(XMFLOAT3((float)(-10.0f + i * 5.0f), 0.0f, 0.0f));
			enemy->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));

			enemy->Set_Date(date);

			name = name + number;

			enemy->Set_Object_Name(name);

			auto component = enemy->Get_Component();

			component->Add_Component<AXIS_COMPONENT>();

			name.pop_back();
			number.clear();
		}*/
	}

	// スコア
	{
		XMFLOAT3 pos = XMFLOAT3(500.0f, 37.0f, 0.0f);

		SCORE* score = Add_Game_Object<SCORE>(LAYER_NAME::UI);
		score->Set_Position(&pos);
		score->Set_Additional(1);
	}

	// HPのUI
	{
		XMFLOAT2 pos(50.0f, SCREEN_HEIGHT * 0.5f);
		SPRITE* sprite = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

		sprite->Set_Object_Name("hp_ui");

		sprite->SetPosition(pos);

		sprite->SetTexture("hp_rod.png");

		sprite->SetSize(XMFLOAT4(300, 32, 500, 32));

		COLOR color = COLOR(0.0f, 1.0f, 1.0f, 1.0f);

		SPRITE* hp = sprite->Add_Child_Sptite("hp", sprite);

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

		TEXTS* sprite = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

		sprite->SetPosition(pos);

		sprite->SetSize(XMFLOAT4(20, 20, 20, 20));

		sprite->SetColor(color);

		//sprite->Edit(string("30 / 30"));
		sprite->Edit(string("infinite / 30"));

		sprite->Set_Object_Name("bullet_ui");

		// 弾のアイコン
		SPRITE* bullet_icon = sprite->Add_Child_Sptite("bullet_icon", sprite);

		bullet_icon->SetTexture("bullet_icon.png");

		pos.x += -60.0f;

		bullet_icon->SetPosition(pos);

		bullet_icon->SetSize(XMFLOAT4(50, 16, 50, 16));
	}

	// ポーズ画面のUI
	{
		XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		COLOR color = COLOR(1.0f, 1.0f, 1.0f, 0.85f);

		SPRITE* sprite = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

		sprite->SetPosition(pos);

		sprite->SetTexture("pause.png");

		sprite->SetSize(XMFLOAT4(200, 200, 200, 200));

		sprite->SetColor(color);

		sprite->Set_Object_Name("pause");

		sprite->SetEnable(false);

		SPRITE* select = sprite->Add_Child_Sptite("select", sprite);

		select->SetTexture("select.png");

		select->SetSize(XMFLOAT4(30, 30, 30, 30));

		sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, -110.0f));		// 1番目

		//sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, 0.0f));		// 2番目

		//sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, 105.0f));		// 3番目
	}

	AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_BGM003, true);

	FADE::Start_FadeIn(60);

	flag = false;
	flag1 = true;

	cnt = 0;

	End_Pause();
}

void GAME::Draw()
{
	SCENE::Draw();
}

void GAME::Draw_DPP()
{
	SCENE::Draw_DPP();
}

void GAME::Update(float delta_time)
{
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
						sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, -110.0f));	// 1番目
						break;

					case 1:
						sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, 0.0f));		// 2番目
						break;

					case 2:
						sprite->Set_Position_Child("select", XMFLOAT2(-165.0f, 105.0f));	// 3番目
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
					CManager::GameEnd();
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
				list<CHILD_DATE> children = sprite->Get_Child_Sptite();

				for (auto child : children)
				{
					if (string("hp") == child.Name)
					{
						child.Child->SetSize(XMFLOAT4(LerpEx(-485.0f, 170.0f, hp, 0.0f, 100.0f), 25, 485, 25));

						if (hp <= 50.0f)
						{
							child.Child->SetColor(color1);
						}

						if (hp <= 20.0f)
						{
							child.Child->SetColor(color2);
						}
					}
				}
			}
		}
	}

	// 敵
	{
		vector<ENEMY*> enemys = Get_Game_Objects<ENEMY>();

		for (auto enemy : enemys)
		{
			auto component = enemy->Get_Component();

			auto axis = component->Get_Component<AXIS_COMPONENT>();

			axis->Rotation_Right(-1.0f);

			//XMFLOAT3 vec = axis->Get_Axis();

			//enemy->SetRotation(vec);
		}
	}


	{
		vector<ENEMY*> emenys = Get_Game_Objects<ENEMY>();

		if (0 == emenys.size())
		{
			if (flag1)
			{
				FADE::Start_FadeOut(60);
				flag1 = false;

				RESULT::Clear_Flag = true;
			}

		}
	}

	if (FADE::End_Fade())
	{
		if (flag)
			SCENE_MANAGER::Set_Scene<RESULT>();

		flag = true;
	}
}

void GAME::Uninit()
{
	SCENE::Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}