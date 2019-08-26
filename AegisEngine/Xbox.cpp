#include	"Xbox.h"

CONTROLER_STATE XBOX::Controllers[MAX_CONTROLLERS];
CONTROLER_STATE XBOX::Old_Controllers[MAX_CONTROLLERS];

//========================================
// Xboxコントローラーの更新
//========================================
void XBOX::Update(void)
{
	DWORD dwResult;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		Old_Controllers[i] = Controllers[i];

		dwResult = XInputGetState(i, &Controllers[i].state);

		if (dwResult == ERROR_SUCCESS)
		{
			Controllers[i].ConnectEnable = true;
		}
		else
		{
			Controllers[i].ConnectEnable = false;
		}
	}
}

//========================================
// トリガー
//========================================
bool XBOX::Trigger(const int any_button, const unsigned char number)
{
	if (Controllers[number].ConnectEnable)
	{
		return (Controllers[number].state.Gamepad.wButtons & any_button) ? true : false;
	}
}

//========================================
// プレス
//========================================
bool XBOX::Press(const int any_button, const unsigned char number)
{
	if ( (Old_Controllers[number].state.Gamepad.wButtons & any_button) && (Controllers[number].state.Gamepad.wButtons & any_button) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//========================================
// リリース
//========================================
bool XBOX::Release(const int any_button, const unsigned char number)
{
	if (Controllers[number].ConnectEnable)
	{
		if ( (Old_Controllers[number].state.Gamepad.wButtons & any_button) != (Controllers[number].state.Gamepad.wButtons & any_button) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//========================================
// 左スティック
//========================================
bool XBOX::LeftStick(const int any_button, const unsigned char number)
{
	if (Controllers[number].ConnectEnable)
	{
		if( (-INPUT_DEADZONE < Controllers[number].state.Gamepad.sThumbLX && Controllers[number].state.Gamepad.sThumbLX < INPUT_DEADZONE) &&
			(-INPUT_DEADZONE < Controllers[number].state.Gamepad.sThumbLY && Controllers[number].state.Gamepad.sThumbLY < INPUT_DEADZONE) )
		{
			Controllers[number].state.Gamepad.sThumbLX = 0;
			Controllers[number].state.Gamepad.sThumbLY = 0;
		}
		return true;	// 仮
	}
}

//========================================
// 右スティック
//========================================
bool XBOX::RightStick(const int any_button, const unsigned char number)
{
	if (Controllers[number].ConnectEnable)
	{
		if( (-INPUT_DEADZONE < Controllers[number].state.Gamepad.sThumbRX && Controllers[number].state.Gamepad.sThumbRX < INPUT_DEADZONE) &&
			(-INPUT_DEADZONE < Controllers[number].state.Gamepad.sThumbRY && Controllers[number].state.Gamepad.sThumbRY < INPUT_DEADZONE) )
		{
			Controllers[number].state.Gamepad.sThumbRX = 0;
			Controllers[number].state.Gamepad.sThumbRY = 0;
		}
			return true;	// 仮
	}
}