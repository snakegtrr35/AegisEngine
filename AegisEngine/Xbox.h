#pragma once

#ifndef XBOX_H
#define XBOX_H

#include	<xinput.h>

#pragma comment (lib, "xinput.lib")

#define MAX_CONTROLLERS 4

// ゲームパッドボタン情報
/*
XINPUT_GAMEPAD_DPAD_UP
XINPUT_GAMEPAD_DPAD_DOWN
XINPUT_GAMEPAD_DPAD_LEFT
XINPUT_GAMEPAD_DPAD_RIGHT
XINPUT_GAMEPAD_START
XINPUT_GAMEPAD_BACK
XINPUT_GAMEPAD_LEFT_THUMB
XINPUT_GAMEPAD_RIGHT_THUMB
XINPUT_GAMEPAD_LEFT_SHOULDER
XINPUT_GAMEPAD_RIGHT_SHOULDER
XINPUT_GAMEPAD_A
XINPUT_GAMEPAD_B
XINPUT_GAMEPAD_X
XINPUT_GAMEPAD_Y
*/

typedef struct {
	XINPUT_STATE state;
	bool ConnectEnable;
}CONTROLER_STATE;

class XBOX {
private:
	static CONTROLER_STATE Controllers[MAX_CONTROLLERS];
	static CONTROLER_STATE Old_Controllers[MAX_CONTROLLERS];

public:
	static void Update(void);		// Xboxコントローラーの更新

	//========================================
	// number : 0～4 の間
	//========================================
	static bool Trigger(const int any_button, const unsigned char number = 0);

	//========================================
	// number : 0～4 の間
	//========================================
	static bool Press(const int any_button, const unsigned char number = 0);

	//========================================
	// number : 0～4 の間
	//========================================
	static bool Release(const int any_button, const unsigned char number = 0);

	static XMINT2 const LeftStick(const int any_button, const unsigned char number = 0);
	static XMINT2 const RightStick(const int any_button, const unsigned char number = 0);
};

#endif // !XBOX_H