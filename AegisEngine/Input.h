#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "Keyboard.h"
#include "main.h"
#include "Mouse.h"
#include "Xbox.h"

#include <Commctrl.h>
#pragma comment (lib, "comctl32.lib")

static const constexpr auto MOUSE = (0x02);
static const constexpr auto	KEYBOARD = (0x06);

static const constexpr auto MiSubClassID = 1;

class CINPUT {
private:
	//static CINPUT* InputDevice;	// 多分いらない

	static RAWINPUTDEVICE device[2];

	static LRESULT CALLBACK subclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	static void setTargetWindow() {
		SetWindowSubclass(GetWindow(), subclassProc, MiSubClassID, (DWORD_PTR)nullptr);
	}

	void onRawInput(WPARAM wParam, LPARAM lParam);

public:

	static void Init() {
		//InputDevice = new CINPUT();

		setTargetWindow();

		device[0].usUsagePage = 0x01;
		device[0].usUsage = MOUSE;
		device[1].usUsagePage = 0x01;
		device[1].usUsage = KEYBOARD;
		RegisterRawInputDevices(device, 2, sizeof(RAWINPUTDEVICE));

		MOUSE::Init();
		KEYBOARD::Init();
	};

	static void Update() {
		MOUSE::Update();
		KEYBOARD::Update();
		XBOX::Update();
	};

	static void Uninit() {
		MOUSE::Uninit();
		KEYBOARD::Uninit();

		//SAFE_DELETE(InputDevice);
	};

	/*static CINPUT* const GetDevice() {
		return InputDevice;
	}*/
};

#endif // !INPUT_H