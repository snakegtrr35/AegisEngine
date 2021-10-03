﻿#pragma once

#ifndef SCORE_H
#define SCORE_H

#include	"GameObject.h"

class SPRITE_ANIMATION;

class SCORE : public GAME_OBJECT {
private:
	SPRITE_ANIMATION* Number;

	int Score;								// スコア
	int NowScore;							// 現在のスコア
	int OldScore;							// のスコア
	unsigned int Additional;				// 増加量
	unsigned char Digit;					// 桁数

	static const unsigned int MaxScore;		// スコアの最大値
	unsigned short WaitFrame;				//	待ちフレーム

	bool Flag;

protected:


public:
	SCORE();
	~SCORE();

	void Init(void) override;
	void Draw(void) override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit(void) override;

	void Set_Additional(const unsigned int additional);
	void Set_Digit(const unsigned char digit);
	void Set_WaitFrame(const unsigned short wait_frame);
	void Add(const int score);
	void Reset(void);

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}
};


CEREAL_REGISTER_TYPE(SCORE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, SCORE)

#endif // !SCORE_H