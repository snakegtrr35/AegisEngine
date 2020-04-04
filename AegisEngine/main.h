#pragma once

#ifndef MAIN_H
#define MAIN_H

#include	<d3d11.h>
#include	<dxgi1_2.h>
#include	<dxgi1_4.h>
#include	<d2d1_1.h>
#include	<dwrite.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "Dxgi.lib")
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment (lib, "xaudio2.lib")
#pragma comment (lib, "winmm.lib")


#define SCREEN_WIDTH	(1920)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(1080)			// �E�C���h�E�̍���

//#define SCREEN_WIDTH	(1280)			// �E�C���h�E�̕�
//#define SCREEN_HEIGHT	(720)			// �E�C���h�E�̍���

HWND GetWindow();

#endif // !MAIN_H