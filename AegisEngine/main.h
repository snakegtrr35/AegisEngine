#pragma once

#ifndef MAIN_H
#define MAIN_H

#include	<d3d11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")


#define SCREEN_WIDTH	(1920)			// ウインドウの幅
#define SCREEN_HEIGHT	(1080)			// ウインドウの高さ

//#define SCREEN_WIDTH	(1280)			// ウインドウの幅
//#define SCREEN_HEIGHT	(720)			// ウインドウの高さ

HWND GetWindow();

#endif // !MAIN_H