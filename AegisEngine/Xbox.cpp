#include	"Xbox.h"

CONTROLER_STATE XBOX::Controllers[MAX_CONTROLLERS];
CONTROLER_STATE XBOX::Old_Controllers[MAX_CONTROLLERS];

//========================================
// Xbox�R���g���[���[�̍X�V
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

			// ���X�e�B�b�N�ݒ�
			{
				float LX = Controllers[i].state.Gamepad.sThumbLX;
				float LY = Controllers[i].state.Gamepad.sThumbLY;

				// �R���g���[�����ǂꂾ��������Ă��邩�𒲂ׂ�
				float magnitude = sqrt(LX * LX + LY * LY);

				// �R���g���[�������������������肷��
				float normalizedLX = LX / magnitude;
				float normalizedLY = LY / magnitude;

				float normalizedMagnitude = 0.0f;

				// �R���g���[�����z�s���т̊O���ɂ��邩�ǂ������m�F
				if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					// �ő�l�œ�����؂���
					if (magnitude > 32767.0f) magnitude = 32767.0f;

					// �s���т̒[����ɂ��đ傫���𒲐�����
					magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

					// �K�v�ɉ����āA�傫�������̗\�z�͈͂ɑ΂��Đ��K������
					// 0.0����1.0�̑傫���̒l��^����
					normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

					LX *= normalizedMagnitude / magnitude;
					LY *= normalizedMagnitude / magnitude;
					state.Gamepad.sThumbLX = (short)LX;
					state.Gamepad.sThumbLY = (short)LY;
				}
				else // �R���g���[�����f�b�h�]�[���ɂ���ꍇ�́A��Βl���[���ɂ���
				{
					state.Gamepad.sThumbLX = 0;
					state.Gamepad.sThumbLY = 0;

				}
				Controllers[i].state.Gamepad.sThumbLX = state.Gamepad.sThumbLX;
				Controllers[i].state.Gamepad.sThumbLY = state.Gamepad.sThumbLY;

			}

			// �E�X�e�B�b�N�ݒ�
			{
				float RX = state.Gamepad.sThumbRX;
				float RY = state.Gamepad.sThumbRY;

				// �R���g���[�����ǂꂾ��������Ă��邩�𒲂ׂ�
				float magnitude = sqrt(RX * RX + RY * RY);

				// �R���g���[�������������������肷��
				float normalizedRX = RX / magnitude;
				float normalizedRY = RY / magnitude;

				float normalizedMagnitude = 0.0f;

				// �R���g���[�����z�s���т̊O���ɂ��邩�ǂ������m�F
				if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				{
					// �ő�l�œ�����؂���
					if (magnitude > 32767.0f) magnitude = 32767.0f;

					// �s���т̒[����ɂ��đ傫���𒲐�����
					magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

					// �K�v�ɉ����āA�傫�������̗\�z�͈͂ɑ΂��Đ��K������
					// 0.0����1.0�̑傫���̒l��^����
					normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

					RX *= normalizedMagnitude / magnitude;
					RY *= normalizedMagnitude / magnitude;
					state.Gamepad.sThumbRX = (short)RX;
					state.Gamepad.sThumbRY = (short)RY;
				}
				else // �R���g���[�����f�b�h�]�[���ɂ���ꍇ�́A��Βl���[���ɂ���
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
// �g���K�[
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
// �v���X
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
// �����[�X
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
// ���X�e�B�b�N
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
// �E�X�e�B�b�N
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