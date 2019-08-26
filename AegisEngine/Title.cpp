#include	"Scene.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"

#include	"Fade.h"

#include	"Timer.h"

static bool flag = false;

void TITLE::Init()
{
	// タイトル画面
	{
		XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		SPRITE* title = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

		title->SetPosition(pos);

		title->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f));

		title->SetTexture(string("title.png"));

		title->Set_Object_Name("title");

		SPRITE* arrow = title->Add_Child_Sptite(string("arrow"), title);

		arrow->SetTexture(string("arrow.png"));

		pos = XMFLOAT2(0.0f, 65.0f + 140.0f);

		title->Set_Position_Child(string("arrow"), pos);

		arrow->SetSize(XMFLOAT4(40.0f, 330.0f, 40.0f, 330.0f));

		arrow->Set_Object_Name("arrow");
	}

#ifdef _DEBUG
	// テキスト画像
	{
		XMFLOAT2 pos(10.0f, 10.0f);
		TEXTS* text = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

		text->SetPosition(pos);

		text->SetSize(XMFLOAT4(10, 10, 10, 10));

		text->Set_Object_Name(string("pos"));

		COLOR color = COLOR(0.0f, 0.0f, 0.0f, 1.0f);

		text->SetColor(color);
	}
#endif // _DEBUG

	FADE::Start_FadeIn(5);
	flag = false;
	FADE::Set_Enable(false);

	AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_BGM001, true);
}

void TITLE::Draw()
{
	for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
	{
		for (GAME_OBJECT* object : GameObjects[i])
		{
			object->Draw();
		}
	}
}

void TITLE::Update()
{
	SCENE::Update();

#ifdef _DEBUG
	{
		vector<TEXTS*> texts = Get_Game_Objects<TEXTS>();

		for (auto sprite : texts)
		{
			if (string("pos") == sprite->Get_Object_Name())
			{
				string space("   ");
				string text;
				string num;

				//XMFLOAT2 pos = XMFLOAT2(MOUSE::Get_Position().x, MOUSE::Get_Position().y);

				//num = to_string((int)pos.x);
				//num = to_string(CROCK_TIMER::Get_Time());

				//num = to_string(TIMER::Get_Time());
				//num = to_string(TIMER::Get_Time_Sec());
				num = to_string(TIMER::Get_Time_Mili(5));

				text = num;

				//text = text + space;

				//num.clear();

				//num = to_string((int)pos.y);

				//text = text + num;

				sprite->Edit(text);
			}
		}
	}
#endif // _DEBUG

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
						sprite->Set_Position_Child("arrow", XMFLOAT2(0.0f, 65.0f + 0.0f));		// 1番目
						break;

					case 1:
						sprite->Set_Position_Child("arrow", XMFLOAT2(0.0f, 65.0f + 140.0f));	// 2番目
						break;

					case 2:
						sprite->Set_Position_Child("arrow", XMFLOAT2(0.0f, 65.0f + 270.0f));	// 3番目
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
							CManager::GameEnd();//
							break;

						/*case 3:
							CManager::GameEnd();
							break;*/
					}

					AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_KETTEI, false);
				}

				break;
			}
		}
	}

	if (FADE::End_Fade())
	{
		if (flag)
			SCENE_MANAGER::Set_Scene<MAIN_MENU>();

		flag = true;
	}
}

void TITLE::Uninit()
{
	SCENE::Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}