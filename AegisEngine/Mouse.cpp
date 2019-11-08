#include	"Mouse.h"
#include	<algorithm>

float MOUSE::Mouse_Sensitivity;
bool MOUSE::MoveFlag = false;
bool MOUSE::Move_X_Flag = false;
bool MOUSE::Move_Y_Flag = false;

int MOUSE::DiffW = 0;
int MOUSE::OldDiffW = 0;

POINT MOUSE::ScreenPoint;
XMFLOAT2 MOUSE::ScreenPosition;

POINT MOUSE::Pos;
POINT MOUSE::OldPos;

void MOUSE::Init(void)
{
	OldPos.y = OldPos.x = Pos.y = Pos.x = 0;

	Mouse_Sensitivity = 1.0f;

	ZeroMemory(&ScreenPoint, sizeof(ScreenPoint));
}

void MOUSE::Update(void)
{
	GetCursorPos(&ScreenPoint);

	ScreenPosition = XMFLOAT2(ScreenPoint.x, ScreenPoint.y);
}

void MOUSE::Uninit(void)
{
}

XMFLOAT2& const MOUSE::Get_Screen_Position()
{
	return ScreenPosition;
}

void  MOUSE::Set_Position(POINT& pos)
{
	if (2 <= abs(OldPos.x - Pos.x) || 2 <= abs(OldPos.y - Pos.y))
	{
		MoveFlag = true;
	}
	else
	{
		MoveFlag = false;
	}

	if (3 <= abs(OldPos.x - Pos.x))
	{
		Move_X_Flag = true;
	}

	if (3 <= abs(OldPos.y - Pos.y))
	{
		Move_Y_Flag = true;
	}

	OldPos.x = Pos.x;
	OldPos.y = Pos.y;

	//Pos.x += std::clamp((int)pos.x, -20, 20);
	//Pos.y += std::clamp((int)pos.y, -20, 20);

	Pos.x += pos.x;
	Pos.y += pos.y;
}

const XMFLOAT2 MOUSE::Get_Position()
{
	XMFLOAT2 pos;

	pos.x = Pos.x - OldPos.x;
	pos.y = Pos.y - OldPos.y;

	pos.x *= Mouse_Sensitivity;
	pos.y *= Mouse_Sensitivity;

	return pos;
}

void MOUSE::Set_Sensitivity(const float sensitivity)
{
	Mouse_Sensitivity = sensitivity;
}

float& const MOUSE::Get_Sensitivity()
{
	return Mouse_Sensitivity;
}

const bool MOUSE::Get_Move_Flag()
{
	return MoveFlag;
}

const bool MOUSE::Get_Move_X_Flag()
{
	return Move_X_Flag;
}

const bool MOUSE::Get_Move_Y_Flag()
{
	return Move_Y_Flag;
}

void MOUSE::Set_Wheel_Move(int w)
{
	OldDiffW = DiffW;

	DiffW += w;
}

const WHEEL_MOVE_ENUM MOUSE::Get_Wheel_Move_Flag()
{
	if (DiffW > OldDiffW)
	{
		return WHEEL_MOVE_ENUM::UP;
	}

	if (DiffW < OldDiffW)
	{
		return WHEEL_MOVE_ENUM::DOWN;
	}

	return WHEEL_MOVE_ENUM::NONE;
}

void MOUSE::Reset_Wheel_Moveset()
{
	OldDiffW = DiffW;
	MoveFlag = false;
	Move_X_Flag = false;
	Move_Y_Flag = false;
}