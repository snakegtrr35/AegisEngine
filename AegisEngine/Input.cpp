#include	"Input.h"

//CINPUT* CINPUT::InputDevice = nullptr;
RAWINPUTDEVICE CINPUT::device[2];

LRESULT CALLBACK CINPUT::subclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (uMsg == WM_INPUT)
	{
		CINPUT* pInput = (CINPUT*)dwRefData;
		pInput->onRawInput(wParam, lParam);
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void CINPUT::onRawInput(WPARAM wParam, LPARAM lParam)
{
	HRESULT hResult = S_OK;

	UINT dwSize = 0;

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return;
	}

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{


		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		// ƒ}ƒEƒXˆÚ“®—Ê‚ÌŽæ“¾
		{
			POINT pos;

			pos.x = raw->data.mouse.lLastX;
			pos.y = raw->data.mouse.lLastY;

			MOUSE::Get_Mouse()->Set_Position(pos);
		}

		{
			USHORT bf = raw->data.mouse.usButtonFlags;

			// Wheel +-
			if (bf & 0x400)
			{
				short w = (short)raw->data.mouse.usButtonData;
				if (w > 0)
				{
					MOUSE::Get_Mouse()->Set_Wheel_Move(1);
				}
				else
				{
					MOUSE::Get_Mouse()->Set_Wheel_Move(-1);
				}
			}
		}

		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
	}

	delete[] lpb;
}