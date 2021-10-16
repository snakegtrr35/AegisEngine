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

bool RESULT::Clear_Flag = true;
static bool flag = true;

static unique_ptr<SPRITE_ANIMATION> sprite_anime = nullptr;

void RESULT::Init()
{
	// リザルト画面
	{
		sprite_anime = make_unique<SPRITE_ANIMATION>();

		sprite_anime->SetPosition(XMFLOAT2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5));
		sprite_anime->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.5));

		sprite_anime->SetTexture("Load.png");

		sprite_anime->SetParam(10, 3, 1);

		sprite_anime->Init();
	}

	flag = true;

	std::thread th(Load, this);

	th.detach();

	FADE::Start_FadeIn(60);
}

void RESULT::Draw()
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

void RESULT::Draw_DPP()
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

void RESULT::Update(float delta_time)
{
	if (false == GetLockLoad())
	{
		sprite_anime.reset(nullptr);

		SCENE::Update(delta_time);

		if (KEYBOARD::Trigger_Keyboard(VK_RETURN))
		{
			if (flag)
			{
				FADE::Start_FadeOut(60);

				flag = false;
			}
		}

		if (FADE::End_Fade())
		{
			SCENE_MANAGER::Set_Scene<TITLE>();
		}

	}
	else
	{
		sprite_anime->Update(delta_time);
	}
}

void RESULT::Uninit()
{

//#ifdef _DEBUG
//	static bool flag = true;
//
//	if (flag)
//	{
//		const type_info& id = typeid(*this);
//
//		string name(id.name());
//
//		// 置換
//		Replace_String(name, "class ", "      ");
//		Replace_String(name, "*", " ");
//		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());
//
//		std::ofstream file(name + ".dat", std::ios::binary);
//
//		bool f = file.is_open();
//
//		cereal::BinaryOutputArchive archive(file);
//		archive(*this);
//
//		flag = false;
//	}
//#endif // _DEBUG

	SCENE::Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}

void RESULT::Load(SCENE* scene)
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
		// リザルト画面
		XMFLOAT2 pos(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		SPRITE* result = Add_Game_Object<SPRITE>(LAYER_NAME::UI, "result");

		result->SetPosition(pos);

		result->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f));
	}
	
	//{
	//	auto result = scene->Get_Game_Object<SPRITE>("result");
	//
	//	if (!result.expired())
	//	{
	//		if (RESULT::Clear_Flag)
	//		{
	//			result.lock()->SetTexture(string("game_clear.png"));
	//
	//			//AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_CLEAR, false);
	//		}
	//		else
	//		{
	//			result.lock()->SetTexture(string("game_over.png"));
	//
	//			//AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_GAMEOVER, false);
	//		}
	//	}
	//}

	scene->SCENE::Init();

	{
		auto result = scene->Get_Game_Object<SPRITE>("result");
	
		if (!result.expired())
		{
			if (RESULT::Clear_Flag)
			{
				result.lock()->SetTexture(string("game_clear.png"));
			}
			else
			{
				result.lock()->SetTexture(string("game_over.png"));
			}
		}
	}

#ifdef _DEBUG
	if (false == flag)
	{
		const type_info& id = typeid(*scene);

		string name(id.name());

		// 置換
		Replace_String(name, "class ", "      ");
		Replace_String(name, "*", " ");
		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());

		std::ofstream file(name + ".dat", std::ios::binary);

		bool f = file.is_open();

		cereal::BinaryOutputArchive archive(file);
		archive(*scene);

		flag = false;
	}
#endif // _DEBUG

	scene->SetLockLoad();
}