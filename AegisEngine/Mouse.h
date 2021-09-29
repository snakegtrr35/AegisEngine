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

	static unique_ptr<MOUSE> pMouse;

	//! スクリーン上のマウスの変数
	POINT ScreenPoint;
	XMFLOAT2 ScreenPosition;

	// マウスの相対座標
	POINT Pos;
	POINT OldPos;

	bool MoveFlag;
	bool Move_X_Flag;
	bool Move_Y_Flag;

	int DiffW;
	int OldDiffW;

	float Mouse_Sensitivity;

	MOUSE() {}									// デフォルトコンストラクタの外部からの呼び出し禁止
	MOUSE(const MOUSE&) = delete;				// コピーコンストラクタの禁止
	MOUSE& operator=(const MOUSE&) = delete;	// ムーブコンストラクタの禁止

public:
	~MOUSE() { Uninit(); }

	static void Init(void);
	static void Update(void);
	static void Uninit(void);

	static MOUSE* const Get_Mouse() {
		return pMouse.get();
	}

	XMFLOAT2& const Get_Screen_Position();

	void Set_Position(POINT& pos);

	const XMFLOAT2 Get_Position();

	void Set_Sensitivity(const float sensitivity);
	float& const Get_Sensitivity();

	const bool Get_Move_Flag();
	const bool Get_Move_X_Flag();
	const bool Get_Move_Y_Flag();

	void Set_Wheel_Move(int w);
	const WHEEL_MOVE_ENUM Get_Wheel_Move_Flag();
	void Reset_Wheel_Moveset();
};

#endif // !MOUSE_H