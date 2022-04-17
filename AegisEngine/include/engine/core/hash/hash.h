#pragma once

#ifndef _INCLEDE_HASH_H_
#define _INCLEDE_HASH_H_

namespace aegis
{
	// !文字列からハッシュ値を作成して取得
	uint32 HashCRC32(const std::string_view& str) noexcept;
}

#endif // !_INCLEDE_HASH_H_
