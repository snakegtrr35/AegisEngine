#include	"Score.h"
#include	"Sprite_Animation.h"
#include	"manager.h"
#include	"ShadowMap.h"

using namespace Aegis;

const unsigned int SCORE::MaxScore = 999999999;

SCORE::SCORE()
{
	Additional = OldScore = NowScore = Score = 0;
	Digit = 4;
	WaitFrame = 0;
	Flag = true;

	Number = new SPRITE_ANIMATION();
	Number->SetTexture(string("number02.png"));
	Number->SetParam(0, 10, 1);
	Number->SetSize(Vector4(32.0f, 32.0f, 32.0f, 32.0f));
	Number->Init();
}

SCORE::~SCORE()
{
	Uninit();
}

void SCORE::Init(void)
{
}

void SCORE::Draw(void)
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		short i;
		unsigned int number, score = NowScore;

		Vector2 wh = Number->Get_Twh();

		Vector2 pos;

		Vector3 position = *Get_Transform().Get_Position();

		for (i = Digit - 1; 0 <= i; i--)
		{
			pos = Vector2(position.x + wh.x * i, position.y);

			number = score % 10;
			Number->Set_Param_Txy(wh.x * number, 1.0f);
			Number->SetPosition(pos);
			Number->Draw();
			score -= number;
			score /= 10;
		}
	}
}

void SCORE::Draw_DPP(void)
{
	short i;
	unsigned int number, score = NowScore;

	Vector2 wh = Number->Get_Twh();

	Vector2 pos;

	Vector3 position = *Get_Transform().Get_Position();

	for (i = Digit - 1; 0 <= i; i--)
	{
		pos = Vector2(position.x + wh.x * i, position.y);

		number = score % 10;
		Number->Set_Param_Txy(wh.x * number, 1.0f);
		Number->SetPosition(pos);
		Number->Draw_DPP();
		score -= number;
		score /= 10;
	}
}

void SCORE::Update(float delta_time)
{
	//if(0 == ( cnt % WaitFrame))
	if (Flag)
	{
		if (NowScore < Score)
		{
			NowScore += Additional;
		}

		if (MaxScore <= NowScore)
		{
			NowScore = MaxScore;
		}
	}
	else
	{
		if (Score < NowScore)
		{
			NowScore -= Additional;
		}

		if (0 < NowScore)
		{
			NowScore = 0;
		}
	}

	WaitFrame++;
}

void SCORE::Uninit(void)
{
	SAFE_DELETE(Number);
}

void SCORE::Set_Additional(const unsigned int additional)
{
	Additional = additional;
}

void SCORE::Set_Digit(const unsigned char digit)
{
	Digit = digit;
}

void SCORE::Set_WaitFrame(const unsigned short wait_frame)
{
	WaitFrame = wait_frame;
}

void SCORE::Add(const int score)
{
	OldScore = Score;

	Score += score;

	if (Score < 0)
	{
		Score = 0;
	}

	if (MaxScore < Score)
	{
		Score = MaxScore;
	}

	if (Score < OldScore)
	{
		Flag = false;
	}
	else
	{
		Flag = true;
	}
}

void SCORE::Reset(void)
{
	NowScore = Score = 0;
}