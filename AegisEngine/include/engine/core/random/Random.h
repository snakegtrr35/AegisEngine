#pragma once

#ifndef _INCLEDE_RANDOM_H_
#define _INCLEDE_RANDOM_H_

#include	<memory>
#include	<mutex>
//#include	<cstdint>
#include	<ctime>

#include	"Xorshift.h"

namespace aegis
{
	class Random
	{
	public:

		static Random* GetInsrance() noexcept;

		[[nodiscard]]
		static uint32 GetRandomUint() noexcept;
		[[nodiscard]]
		static float GetRandomfloat() noexcept;
		[[nodiscard]]
		static float Range(const float min, const float max) noexcept;
		[[nodiscard]]
		static uint64 GetRandomUlong() noexcept;

		static void ReSeed(uint64 seed) noexcept;

		static void ReSeed64(uint64 seed) noexcept;

	private:

		Random(uint64 seed = ::time(nullptr));
		~Random();

		static Random Insrance;
		static std::mutex mutex;

		XorShift32 random;
		XorShift64 random64;
	};
}
#endif // !_INCLEDE_RANDOM_H_