#include	"Mouse.h"
#include	<algorithm>

float MOUSE::Mouse_Sensitivity;

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
	OldPos.x = Pos.x;
	OldPos.y = Pos.y;

	Pos.x += std::clamp((int)pos.x, -20, 20);
	Pos.y += std::clamp((int)pos.y, -20, 20);
}

POINT& const MOUSE::Get_Position()
{
	POINT pos;

	pos.x = Pos.x - OldPos.x;
	pos.y = Pos.y - OldPos.y;

	pos.x *= Mouse_Sensitivity;
	pos.y *= Mouse_Sensitivity;

	return pos;
}

void MOUSE::Set_Sensitivity(float& const sensitivity)
{
	Mouse_Sensitivity = sensitivity;
}

float& const MOUSE::Get_Sensitivity()
{
	return Mouse_Sensitivity;
}