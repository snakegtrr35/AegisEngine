// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "stdafx.h"
#include "audio_clip.h"
//#include "audio_clip.cpp"

#ifdef _DEBUG
VC_DLL_EXPORTS void __cdecl Print();

#else // _DEBUG
void __cdecl Print();

#endif

void Print() {}

namespace aegis::audio
{
	AudioManager* AudioInstance()
	{
		return AudioManager::Instance();
	}

	void CreateInstance()
	{
		AudioManager::CreateInstance();
	}

	void DestroyInstance()
	{
		AudioManager::DestroyInstance();
	}
}

std::wstring stringTowstring(const std::string_view font)
{
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t length = font.size();

	std::unique_ptr<wchar_t[]>  StrW = std::make_unique<wchar_t[]>(length * 2);

	size_t wLen = 0;

	mbstowcs_s(&wLen, StrW.get(), length * 2, font.data(), _TRUNCATE);

	return std::wstring(StrW.get());
}


std::string wstringTostring(const std::wstring_view font)
{
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t length = font.size();

	std::unique_ptr<char[]>  Str = std::make_unique<char[]>(length * 2);

	size_t Len = 0;

	wcstombs_s(&Len, Str.get(), length * 2, font.data(), length * 2);

	return std::string(Str.get());
}
