#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include	"main.h"

#include	<algorithm>

enum class WHEEL_MOVE_ENUM {
	UP = 1,
	DOWN = -1,
	NONE
};

class MOUSE {
private:
	static float Mouse_Sensitivity;

	//! スクリーン上のマウスの変数
	static POINT ScreenPoint;
	static XMFLOAT2 ScreenPosition;

	// マウスの相対座標
	static POINT Pos;
	static POINT OldPos;

	static bool MoveFlag;
	static bool Move_X_Flag;
	static bool Move_Y_Flag;

	static int DiffW;
	static int OldDiffW;

public:
	static void Init(void);
	static void Update(void);
	static void Uninit(void);

	static XMFLOAT2& const Get_Screen_Position();

	static void Set_Position(POINT& pos);

	static const XMFLOAT2 Get_Position();

	static void Set_Sensitivity(const float sensitivity);
	static float& const Get_Sensitivity();

	static const bool Get_Move_Flag();
	static const bool Get_Move_X_Flag();
	static const bool Get_Move_Y_Flag();

	static void Set_Wheel_Move(int w);
	static const WHEEL_MOVE_ENUM Get_Wheel_Move_Flag();
	static void Reset_Wheel_Moveset();
};

#endif // !MOUSE_H