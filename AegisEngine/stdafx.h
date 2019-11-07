// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// または、参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#ifndef STDAFX_H
#define STDAFX_H

#define _CRT_SECURE_NO_WARNINGS

// プログラムに必要な追加ヘッダーをここで参照してください

#include	<stdio.h>
#include	<stdlib.h>
#define NOMINMAX
#include	<windows.h>
#undef NOMINMAX
#include	<assert.h>

#include	"common.h"

#include	<DirectXMath.h>
using namespace DirectX;

#include	"Math.h"
using namespace Math;

#ifndef IMGUI
#define IMGUI

#include	"imgui/imgui.h"
#include	"imgui/imgui_impl_dx11.h"
#include	"imgui/imgui_impl_win32.h"
#include	"imgui/imstb_rectpack.h"
#include	"imgui/imstb_truetype.h"
#include	"imgui/ImGuizmo.h"

#endif // !IMGUI

#ifndef PICO_JSON
#define PICO_JSON

#include	"picojson.h"

#endif // !PICO_JSON

#ifndef INCLUDE
#define INCLUDE

#include	<list>
#include	<vector>
#include	<map>
#include	<unordered_map>
#include	<set>
#include	<unordered_set>
#include	<memory>
#include	<string>
#include	<functional>
#include	<random>
#include	<algorithm>
#include	<functional>
#include	<typeinfo>

#endif // !INCLUDE

using namespace std;

#endif // !STDAFX_H