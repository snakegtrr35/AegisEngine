#include	"Scene.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"

#include	"Fade.h"

bool RESULT::Clear_Flag;

void RESULT::Init()
{
	// リザルト画面
	{
		XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		SPRITE* result = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

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
	SCENE::Uninit();
}