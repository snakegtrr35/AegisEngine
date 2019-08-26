#pragma once

#ifndef SCORE_H
#define SCORE_H

#include	"Game_Object.h"

class SPRITE_ANIMATION;

class SCORE : public GAME_OBJECT {
private:
	SPRITE_ANIMATION* Number;

	int Score;								// �X�R�A
	int NowScore;							// ���݂̃X�R�A
	int OldScore;							// �̃X�R�A
	unsigned int Additional;				// ������
	unsigned char Digit;					// ����

	static const unsigned int MaxScore;		// �X�R�A�̍ő�l
	unsigned short WaitFrame;				//	�҂��t���[��

	bool Flag;

protected:


public:
	SCORE();
	~SCORE();

	void Init(void) override;
	void Draw(void) override;
	void Update(void) override;
	void Uninit(void) override;

	void Set_Additional(const unsigned int additional);
	void Set_Digit(const unsigned char digit);
	void Set_WaitFrame(const unsigned short wait_frame);
	void Add(const int score);
	void Reset(void);
};


#endif // !SCORE_H