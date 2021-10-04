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

//#define NOMINMAX
//#include	<windows.h>

#undef min
#undef max

#include	<assert.h>

#include	<DirectXMath.h>
#include	<directxcollision.h>
using namespace DirectX;

#include	"common.h"
#include	"include/engine/core/math/Color.h"

#include	"include/engine/core/math/Math.h"

#ifndef IMGUI
#define IMGUI

#include	"imgui/imgui.h"
#include	"imgui/imgui_impl_dx11.h"
#include	"imgui/imgui_impl_win32.h"
#include	"imgui/imstb_rectpack.h"
#include	"imgui/imstb_truetype.h"
#include	"imgui/ImGuizmo.h"

#endif // !IMGUI

#ifndef CEREAL
#define CEREAL

#include	<cereal/cereal.hpp>
#include	<cereal/archives/binary.hpp>
#include	<cereal/types/base_class.hpp>
#include	<cereal/types/polymorphic.hpp>

#include	<cereal/types/memory.hpp>
#include	<cereal/types/array.hpp>
#include	<cereal/types/list.hpp>
#include	<cereal/types/vector.hpp>
#include	<cereal/types/map.hpp>
#include	<cereal/types/unordered_map.hpp>
#include	<cereal/types/set.hpp>
#include	<cereal/types/unordered_set.hpp>
#include	<cereal/types/string.hpp>
#include	<cereal/types/variant.hpp>

#include	"Cereal.h"

#endif // !CEREAL

#ifndef INCLUDE
#define INCLUDE

#include	<array>
#include	<list>
#include	<vector>
#include	<map>
#include	<unordered_map>
#include	<set>
#include	<unordered_set>
#include	<deque>
#include	<memory>
#include	<string>
#include	<variant>
#include	<functional>
#include	<fstream>
#include	<algorithm>
#include	<functional>
#include	<typeinfo>
#include	<chrono>
#include	<filesystem>
#include	<numeric>
#include	<thread>
#include	<mutex>

#include	<cstdint>
#include	<ctime>

#endif // !INCLUDE

using namespace std;

#endif // !STDAFX_H