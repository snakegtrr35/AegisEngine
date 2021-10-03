#pragma once

#ifndef _INCLEDE_RANDOM_H_
#define _INCLEDE_RANDOM_H_

#include	<memory>
#include	<mutex>
//#include	<cstdint>
#include	<ctime>

#include	"Xorshift.h"

namespace Aegis
{
	class Random
	{
	public:

		static Random* GetInsrance() noexcept;

		[[nodiscard]]
		static std::uint32_t GetRandomUint() noexcept;
		[[nodiscard]]
		static float GetRandomfloat() noexcept;
		[[nodiscard]]
		static float Range(const float min, const float max) noexcept;
		[[nodiscard]]
		static std::uint64_t GetRandomUlong() noexcept;

		static void ReSeed(std::uint64_t seed) noexcept;

		static void ReSeed64(std::uint64_t seed) noexcept;

	private:

		Random(std::uint64_t seed = ::time(nullptr));
		~Random();

		static Random Insrance;
		static std::mutex mutex;

		XorShift32 random;
		XorShift64 random64;
	};
}
#endif // !_INCLEDE_RANDOM_H_