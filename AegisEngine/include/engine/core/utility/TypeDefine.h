#pragma once

#ifndef _INCLEDE_TYPEDEFINE_H_
#define _INCLEDE_TYPEDEFINE_H_

#include <cstdint>

namespace aegis
{
	//using byte = std::uint8_t;
	using int8 = std::int8_t;
	using uint8 = std::uint8_t;
	using int16 = std::int16_t;
	using uint16 = std::uint16_t;
	using int32 = std::int32_t;
	using uint32 = std::uint32_t;
	using int64 = std::int64_t;
	using uint64 = std::uint64_t;
	using int_ptr = std::intptr_t;
	using uint_ptr = std::uintptr_t;
	using char8 = char;
	using char16 = wchar_t;

	//using float32 = float;
	//using float64 = double;
	typedef float float32;
	typedef double float64;
}

#endif // !_INCLEDE_TYPEDEFINE_H_