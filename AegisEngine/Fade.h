#pragma once

#ifndef FADE_H
#define FADE_H

#include	"Sprite.h"

class FADE : public SPRITE{
private:
	
	static bool FadeEnable;

	XMFLOAT4 Color;

	XMFLOAT2 WH;

	unsigned short Time;
	unsigned short Cnt;
	float AdditionalAlpha;

public: 
	FADE();
	~FADE();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void Set_Time(const unsigned short time);
	bool Get_FadeEnable();
	void Set_Color(const XMFLOAT4& const color);
	void Set_AdditionalAlpha(const unsigned short time, const float sign);

	static void Start_FadeOut(const float time);

	static void Start_FadeIn(const float time);

	static bool End_Fade();

	static void Set_Enable(const bool flag);
};

#endif // !FADE_H