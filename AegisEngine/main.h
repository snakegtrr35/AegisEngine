#pragma once

#define _CRT_SECURE_NO_WARNINGS

#ifndef MAIN_H
#define MAIN_H

#include	<d3d11.h>
#include	<DirectXMath.h>
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")


#define SCREEN_WIDTH	(1920)			// ウインドウの幅
#define SCREEN_HEIGHT	(1080)			// ウインドウの高さ

//#define SCREEN_WIDTH	(1280)			// ウインドウの幅
//#define SCREEN_HEIGHT	(720)			// ウインドウの高さ

#ifndef COMMOM
#define COMMOM

// メモリ開放
#define SAFE_RELEASE(x)			if( nullptr != x ){ x->Release(); x = nullptr; }
#define SAFE_DELETE(x)			if( nullptr != x ){ delete x;  x = nullptr; }
#define SAFE_DELETE_ARRAY(x)	if( nullptr != x ){ delete[] x;  x = nullptr; }

#define FAILDE_ASSERT			{ assert(false); }

#define TEXT_PASTE(x) L ## x
#define TEXT_UTF_8(x) u8 ## x

#define TEXT(x) TEXT_PASTE(x)
#define TEXT_U8(x) TEXT_UTF_8(x)

//// テンプレート
//template <typename T>
//T* SAFE_NEW(T* object)
//{
//	if (nullptr == object)
//	{
//		T* object = new T();
//	}
//
//	return object;
//}

//! スマートポインタ用のカスタムデリーター
/*! Release() がある */
struct Release {
	template<class T>
	void operator()(T* p) const {
		p->Release();
		p = nullptr;
	}
};

//! スマートポインタ用のカスタムデリーター
/*! Uninit() がある */
struct Delete {
	template<class T>
	void operator()(T* p) const {
		p->Uninit();
		p = nullptr;
	}
};

#endif // !COMMOM

HWND GetWindow();
HINSTANCE GethInstance();

const int Get_Random(const int min, const int max);
bool Rand_Bool(const double probability);

// 線形補間
// x1, y1 : 最小値のx,y x2, y2 : 最大値のx,y tx : 補間したい値
float Lerp(const float x1, const float y1, const float x2, const float y2, const float tx);

short Loop_Plus(short tx, short max);
short Loop_Minus(short tx, short max);

#endif // !MAIN_H