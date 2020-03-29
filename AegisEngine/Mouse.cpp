#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

#include	"Mouse.h"
#include	<algorithm>

unique_ptr<MOUSE> MOUSE::pMouse = nullptr;

void MOUSE::Init(void)
{
	if (nullptr == pMouse.get()) pMouse.reset(new MOUSE());

	pMouse->OldPos.y = pMouse->OldPos.x = 0;
	pMouse->Pos.y = pMouse->Pos.x = 0;

	pMouse->Mouse_Sensitivity = 1.0f;

	pMouse->ScreenPoint.x = 0;
	pMouse->ScreenPoint.y = 0;
}

void MOUSE::Update(void)
{
	if (nullptr != pMouse.get())
	{
		GetCursorPos(&pMouse->ScreenPoint);

		pMouse->ScreenPosition = XMFLOAT2((float)pMouse->ScreenPoint.x, (float)pMouse->ScreenPoint.y);
	}
}

void MOUSE::Uninit(void)
{
	pMouse.reset(nullptr);
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