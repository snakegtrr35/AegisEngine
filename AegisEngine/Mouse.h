#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include	"main.h"

#include	<algorithm>

class MOUSE {
private:
	static float Mouse_Sensitivity;

	//! スクリーン上のマウスの変数
	static POINT ScreenPoint;
	static XMFLOAT2 ScreenPosition;

	// マウスの相対座標
	static POINT Pos;
	static POINT OldPos;

public:
	static void Init(void);
	static void Update(void);
	static void Uninit(void);

	static XMFLOAT2& const Get_Screen_Position();

	static void Set_Position(POINT& pos);

	static POINT& const Get_Position();

	static void Set_Sensitivity(float& const sensitivity);
	static float& const Get_Sensitivity();
};

#endif // !MOUSE_H