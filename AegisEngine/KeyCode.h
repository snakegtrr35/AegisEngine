#pragma once

#ifndef _INCLUDE_KEYCODE_H_
#define _INCLUDE_KEYCODE_H_

#undef DELETE

namespace Aegis
{
	enum class KeyCode : std::uint8_t
	{
		Keypad0 = 0x30,
		Keypad1 = 0x31,
		Keypad2 = 0x32,
		Keypad3 = 0x33,
		Keypad4 = 0x34,
		Keypad5 = 0x35,
		Keypad6 = 0x36,
		Keypad7 = 0x37,
		Keypad8 = 0x38,
		Keypad9 = 0x39,

		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

		F1  = 0x70,
		F2  = 0x71,
		F3  = 0x72,
		F4  = 0x73,
		F5  = 0x74,
		F6  = 0x75,
		F7  = 0x76,
		F8  = 0x77,
		F9  = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,

		LBUTTON = 0x01, // マウスの左ボタン
		RBUTTON = 0x02, // マウスの右ボタン
		MBUTTON = 0x04, // マウスの中央ボタン(3ボタンマウス)
		BUTTON1 = 0x05, // マウスの第1拡張ボタン
		BUTTON2 = 0x06, // マウスの第2拡張ボタン

		BACK = 0x08, // Backspace
		TAB = 0x09, // Tab
		Enter = 0x0D, // 

		LSHIFT = 0xA0, // 左Shift 
		RSHIFT = 0xA1, // 右Shift
		LCONTROL = 0xA2, // 左Ctrl
		RCONTROL = 0xA3, // 右Ctrl
		LMENU = 0xA4, // 左Alt
		RMENU = 0xA5, // 右Alt
		PAUSE = 0x13, // Pause
		CAPITAL = 0x14, // CapsLock
		//KANA = 0x00, // 
		ESCAPE = 0x1B, // Esc
		SPACE = 0x20, // スペース
		PRIOR = 0x21, // PageUp
		NEXT = 0x22, // PageDown
		END = 0x23, // End
		HOME = 0x24, // Home
		LEFT = 0x25, // ←キー
		UP = 0x26, // ↑キー
		RIGHT = 0x27, // →キー
		DOWN = 0x28, // ↓キー
		SNAPSHOT = 0x2C, // PrintScreen
		INSERT = 0x2D, // Insert
		DELETE = 0x2E, // Delete
		LWIN = 0x5B, // 左Windows
		NUMLOCK = 0x90, // NumLock
		SCROL = 0x91, // ScrollLock
	};
}

#endif // !_INCLUDE_KEYCODE_H_