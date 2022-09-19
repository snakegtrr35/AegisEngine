// pch.h: プリコンパイル済みヘッダー ファイルです。
// 次のファイルは、その後のビルドのビルド パフォーマンスを向上させるため 1 回だけコンパイルされます。
// コード補完や多くのコード参照機能などの IntelliSense パフォーマンスにも影響します。
// ただし、ここに一覧表示されているファイルは、ビルド間でいずれかが更新されると、すべてが再コンパイルされます。
// 頻繁に更新するファイルをここに追加しないでください。追加すると、パフォーマンス上の利点がなくなります。

#ifndef PCH_H
#define PCH_H

// プリコンパイルするヘッダーをここに追加します

#if _DEBUG
#define VC_DLL_EXPORTS extern "C" __declspec(dllexport)
#else
#define VC_DLL_EXPORTS
#endif // _DEBUG

#include <cstdint>
#include <array>
#include <string>
#include <string_view>
#include <memory>
#include <cassert>

#include <unordered_map>
#include <clocale>

//#include "../../core/aegisContainer.h"

std::wstring stringTowstring(const std::string_view font);
std::string wstringTostring(const std::wstring_view font);

#endif //PCH_H
