﻿#pragma once

#ifndef COMMON_H
#define COMMON_H

#include "main.h"
#include <string>

#include "include/engine/core/aegisContainer.h"
#include "include\engine\core\AllocatorWrapper.h"

// メモリ開放
#define SAFE_RELEASE(x)			if( nullptr != x ){ x->Release(); x = nullptr; }
#define SAFE_DELETE(x)			if( nullptr != x ){ delete x;  x = nullptr; }
#define SAFE_DELETE_ARRAY(x)	if( nullptr != x ){ delete[] x;  x = nullptr; }

#define FAILDE_ASSERT			{ assert(false); }

#define TEXT_PASTE(x) L ## x
#define TEXT_UTF_8(x) u8 ## x

#define TEXT(x) TEXT_PASTE(x)
#define TEXT_U8(x) TEXT_UTF_8(x)

const constexpr float ANIMETION_FRAME_60 = 1.f / 60.0f;		// 60FPS
const constexpr float ANIMETION_FRAME_30 = 1.f / 30.0f;		// 30FPS

//! スマートポインタ用のカスタムデリータ
/*! Release() がある */
struct Release {
	template<class T>
	void operator()(T*& p) const noexcept
	{
		p->Release();
		p = nullptr;
	}
};

//! スマートポインタ用のカスタムデリーター
struct Delete {
	template<class T>
	void operator()(T*& p) noexcept
	{
		delete p;
		p = nullptr;
	}
};

//! スマートポインタ用のカスタムデリーター(配列版)
struct Delete_Array {
	template<class T>
	void operator()(T*& p) noexcept
	{
		delete[] p;
		p = nullptr;
	}
};

//! スマートポインタ用のカスタムデリーター
/*! Uninit() がある */
struct Uninit {
	template<class T>
	void operator()(T*& p) noexcept
	{
		p->Uninit();
		delete p;
		p = nullptr;
	}
};

//! スマートポインタ用のカスタムデリーター(配列版)
/*! Uninit() がある */
struct Uninit_Array {
	template<class T>
	void operator()(T*& p) noexcept
	{
		p->Uninit();
		delete[] p;
		p = nullptr;
	}
};

//! スマートポインタ用のカスタムデリーター
template<class T>
struct AllocatorDelete
{
	AllocatorDelete() noexcept = default;

	void operator()(T*& p) noexcept
	{
		if (p)
		{
			//p->Release();
			aegis::allocatorWrapper::deallocate(p);
			//p = nullptr;
		}
	}
};

//! スマートポインタ用のカスタムデリーター
template<class T>
struct AllocatorDelete<T[]>
{
	AllocatorDelete() noexcept = default;

	void operator()(T*& p) noexcept
	{
		if (p)
		{
			const aegis::uint32 size = sizeof(p) / sizeof(p[0]);
			aegis::allocatorWrapper::deallocate(p, size * sizeof(T));
			p = nullptr;
		}
	}
};

std::wstring stringTowstring(const std::string_view font);
std::string wstringTostring(const std::wstring_view font);

aegis::string ExtratNum(aegis::string& str);

std::string Replace_String(std::string_view replacedStr, const std::string_view from, const std::string_view to);

#ifdef UNICODE
void Erroer_Message(const std::wstring_view str1, const std::wstring_view str2 = std::wstring_view(L"エラーメッセージ"));
void Erroer_Message(const std::string_view str1, const std::string_view str2 = std::string_view("エラーメッセージ"));
#else
void Erroer_Message(const std::string_view str1, const std::string_view str2 = std::string("エラーメッセージ"));
#endif // !UNICODE

#endif // !COMMON_H