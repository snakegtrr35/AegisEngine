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

			XINPUT_STATE state = Controllers[i].state;

			// 左スティック設定
			{
				float LX = Controllers[i].state.Gamepad.sThumbLX;
				float LY = Controllers[i].state.Gamepad.sThumbLY;

				// コントローラがどれだけ押されているかを調べる
				float magnitude = sqrt(LX * LX + LY * LY);

				// コントローラが押される方向を決定する
				float normalizedLX = LX / magnitude;
				float normalizedLY = LY / magnitude;

				float normalizedMagnitude = 0.0f;

				// コントローラが循環不感帯の外側にあるかどうかを確認
				if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					// 最大値で等級を切り取る
					if (magnitude > 32767.0f) magnitude = 32767.0f;

					// 不感帯の端を基準にして大きさを調整する
					magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

					// 必要に応じて、大きさをその予想範囲に対して正規化する
					// 0.0から1.0の大きさの値を与える
					normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

					LX *= normalizedMagnitude / magnitude;
					LY *= normalizedMagnitude / magnitude;
					state.Gamepad.sThumbLX = (short)LX;
					state.Gamepad.sThumbLY = (short)LY;
				}
				else // コントローラがデッドゾーンにある場合は、絶対値をゼロにする
				{
					state.Gamepad.sThumbLX = 0;
					state.Gamepad.sThumbLY = 0;

				}
				Controllers[i].state.Gamepad.sThumbLX = state.Gamepad.sThumbLX;
				Controllers[i].state.Gamepad.sThumbLY = state.Gamepad.sThumbLY;

			}

			// 右スティック設定
			{
				float RX = state.Gamepad.sThumbRX;
				float RY = state.Gamepad.sThumbRY;

				// コントローラがどれだけ押されているかを調べる
				float magnitude = sqrt(RX * RX + RY * RY);

				// コントローラが押される方向を決定する
				float normalizedRX = RX / magnitude;
				float normalizedRY = RY / magnitude;

				float normalizedMagnitude = 0.0f;

				// コントローラが循環不感帯の外側にあるかどうかを確認
				if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				{
					// 最大値で等級を切り取る
					if (magnitude > 32767.0f) magnitude = 32767.0f;

					// 不感帯の端を基準にして大きさを調整する
					magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

					// 必要に応じて、大きさをその予想範囲に対して正規化する
					// 0.0から1.0の大きさの値を与える
					normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

					RX *= normalizedMagnitude / magnitude;
					RY *= normalizedMagnitude / magnitude;
					state.Gamepad.sThumbRX = (short)RX;
					state.Gamepad.sThumbRY = (short)RY;
				}
				else // コントローラがデッドゾーンにある場合は、絶対値をゼロにする
				{
					state.Gamepad.sThumbLX = 0;
					state.Gamepad.sThumbLY = 0;
				}
				Controllers[i].state.Gamepad.sThumbRX = state.Gamepad.sThumbRX;
				Controllers[i].state.Gamepad.sThumbRY = state.Gamepad.sThumbRY;
			}
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
	return false;
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
	return false;
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
	}
	return false;
}

//========================================
// 左スティック
//========================================
XMINT2 const XBOX::LeftStick(const unsigned char number)
{
	if (Controllers[number].ConnectEnable)
	{
		XMINT2 stick;

		stick.x = Controllers[number].state.Gamepad.sThumbLX;
		stick.y = Controllers[number].state.Gamepad.sThumbLY;

		return stick;
	}
	return XMINT2(0, 0);
}

//========================================
// 右スティック
//========================================
XMINT2 const XBOX::RightStick(const unsigned char number)
{
	if (Controllers[number].ConnectEnable)
	{
		XMINT2 stick;

		stick.x = Controllers[number].state.Gamepad.sThumbRX;
		stick.y = Controllers[number].state.Gamepad.sThumbRY;

		return stick;
	}
	return XMINT2(0, 0);
}