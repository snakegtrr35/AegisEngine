#pragma once

#ifndef XBOX_H
#define XBOX_H

#include	"main.h"
#include	<xinput.h>

#pragma comment (lib, "xinput.lib")

#define MAX_CONTROLLERS 4
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

typedef struct {
	XINPUT_STATE state;
	bool ConnectEnable;
}CONTROLER_STATE;

class XBOX {
private:
	static CONTROLER_STATE Controllers[MAX_CONTROLLERS];
	static CONTROLER_STATE Old_Controllers[MAX_CONTROLLERS];

public:
	static void Update(void);		// Xbox�R���g���[���[�̍X�V

	//========================================
	// number : 0�`4 �̊�
	//========================================
	static bool Trigger(const int any_button, const unsigned char number);

	//========================================
	// number : 0�`4 �̊�
	//========================================
	static bool Press(const int any_button, const unsigned char number);

	//========================================
	// number : 0�`4 �̊�
	//========================================
	static bool Release(const int any_button, const unsigned char number);

	// Zero value if thumbsticks are within the dead zone �� ����܂�Ӗ���������Ȃ�
	static bool LeftStick(const int any_button, const unsigned char number);
	static bool RightStick(const int any_button, const unsigned char number);
};

#endif // !XBOX_H