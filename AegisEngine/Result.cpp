#include	"Scene.h"
#include	"Result.h"
#include	"Input.h"

#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"

#include	"Fade.h"
#include	"Sprite_Animation.h"

#include	"Scene_Manager.h"
#include	"Title.h"

bool RESULT::Clear_Flag;

static unique_ptr<SPRITE_ANIMATION> sprite_anime = nullptr;

void RESULT::Init()
{
	// ƒŠƒUƒ‹ƒg‰æ–Ê
	{
		XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		SPRITE* result = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "result");

		result->SetPosition(pos);

		result->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f));

		if (RESULT::Clear_Flag)
		{
			result->SetTexture(string("game_clear.png"));

			AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_CLEAR, false);
		}
		else
		{
			result->SetTexture(string("game_over.png"));

			AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_GAMEOVER, false);
		}
	}

	FADE::Start_FadeIn(60);
}

void RESULT::Draw()
{
	SCENE::Draw();
}

void RESULT::Draw_DPP()
{
	SCENE::Draw_DPP();
}

void RESULT::Update(float delta_time)
{
	SCENE::Update(delta_time);

	if (KEYBOARD::Trigger_Keyboard(VK_RETURN))
	{
		SCENE_MANAGER::Set_Scene<TITLE>();
	}

	if (KEYBOARD::Trigger_Keyboard(VK_SHIFT))
	{
		CManager::Get_Instance()->GameEnd();
	}
}

void RESULT::Uninit()
{
	static bool flag = true;

	if (flag)
	{
		const type_info& id = typeid(*this);

		string name(id.name());

		// ’uŠ·
		Replace_String(name, "class ", "      ");
		Replace_String(name, "*", " ");
		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());

		std::ofstream file(name + ".dat", std::ios::binary);

		bool f = file.is_open();

		cereal::BinaryOutputArchive archive(file);
		archive(*this);

		flag = false;
	}

	SCENE::Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}