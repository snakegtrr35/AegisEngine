#pragma once

#ifndef _RANDOM_XORSHIFT_H_
#define _RANDOM_XORSHIFT_H_

#include <array>

namespace Aegis
{
	inline constexpr float FloatFromBits(const std::uint32_t i, const float min = 0.f, const float max = 1.0f) noexcept
	{
		return ((i >> 8) * 0x1.0p-24f) * (max - min) + min;
	}

	inline constexpr double DoubleFromBits(const std::uint64_t i, const double min = 0.f, const double max = 1.0f) noexcept
	{
		return ((i >> 11) * 0x1.0p-53) * (max - min) + min;
	}

	namespace detail
	{
		static constexpr std::uint64_t RotL(const std::uint64_t x, const std::int32_t s) noexcept
		{
			return (x << s) | (x >> (64 - s));
		}

		static constexpr std::uint32_t RotL(const std::uint32_t x, const std::int32_t s) noexcept
		{
			return (x << s) | (x >> (32 - s));
		}
	}

	// SplitMix64
	// Output: 64 bits
	// Period: 2^64
	// Footprint: 8 bytes
	// Original implementation: http://prng.di.unimi.it/splitmix64.c
	struct SplitMix64 {
	public:

		using state_type = std::uint64_t;
		using result_type = std::uint64_t;

		constexpr SplitMix64(state_type state = ::time(NULL)) noexcept
			: m_state(state) {}

		constexpr result_type operator()() noexcept
		{
			result_type z = (m_state += 0x9e3779b97f4a7c15);
			z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
			z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
			return z ^ (z >> 31);
		}

		template <size_t N>
		[[nodiscard]]
		inline constexpr std::array<std::uint64_t, N> generateSeedSequence() noexcept
		{
			std::array<std::uint64_t, N> seeds = {};

			for (auto& seed : seeds)
			{
				seed = operator()();
			}

			return seeds;
		}

		[[nodiscard]]
		friend bool operator ==(const SplitMix64& lhs, const SplitMix64& rhs) noexcept
		{
			return (lhs.m_state == rhs.m_state);
		}

		[[nodiscard]]
		friend bool operator !=(const SplitMix64& lhs, const SplitMix64& rhs) noexcept
		{
			return (lhs.m_state != rhs.m_state);
		}

	private:

		state_type m_state;
	};

	// xoshiro256++
		// Output: 64 bits
		// Period: 2^256 - 1
		// Footprint: 32 bytes
		// Original implementation: http://prng.di.unimi.it/xoshiro256plusplus.c
		// Version: 1.0
	class Xoshiro256PlusPlus {
	public:

		using state_type = std::array<std::uint64_t, 4>;
		using result_type = std::uint64_t;

		constexpr Xoshiro256PlusPlus(std::uint64_t seed = ::time(NULL)) noexcept
			: m_state(SplitMix64{ seed }.generateSeedSequence<4>()) {}

		constexpr Xoshiro256PlusPlus(state_type state) noexcept
			: m_state(state) {}

		result_type operator()() noexcept
		{
			const result_type result = detail::RotL(m_state[0] + m_state[3], 23) + m_state[0];
			const std::uint64_t t = m_state[1] << 17;

			m_state[2] ^= m_state[0];
			m_state[3] ^= m_state[1];
			m_state[1] ^= m_state[2];
			m_state[0] ^= m_state[3];
			m_state[2] ^= t;
			m_state[3] = detail::RotL(m_state[3], 45);
			return result;
		}

		// This is the jump function for the generator. It is equivalent
		// to 2^128 calls to next(); it can be used to generate 2^128
		// non-overlapping subsequences for parallel computations.
		constexpr void jump() noexcept
		{
			constexpr std::uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

			std::uint64_t s0 = 0;
			std::uint64_t s1 = 0;
			std::uint64_t s2 = 0;
			std::uint64_t s3 = 0;

			for (std::uint64_t jump : JUMP)
			{
				for (int b = 0; b < 64; ++b)
				{
					if (jump & UINT64_C(1) << b)
					{
						s0 ^= m_state[0];
						s1 ^= m_state[1];
						s2 ^= m_state[2];
						s3 ^= m_state[3];
					}
					operator()();
				}
			}

			m_state[0] = s0;
			m_state[1] = s1;
			m_state[2] = s2;
			m_state[3] = s3;
		}

		// This is the long-jump function for the generator. It is equivalent to
		// 2^192 calls to next(); it can be used to generate 2^64 starting points,
		// from each of which jump() will generate 2^64 non-overlapping
		// subsequences for parallel distributed computations.
		constexpr void longJump() noexcept
		{
			constexpr std::uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

			std::uint64_t s0 = 0;
			std::uint64_t s1 = 0;
			std::uint64_t s2 = 0;
			std::uint64_t s3 = 0;

			for (std::uint64_t jump : LONG_JUMP)
			{
				for (int b = 0; b < 64; ++b)
				{
					if (jump & UINT64_C(1) << b)
					{
						s0 ^= m_state[0];
						s1 ^= m_state[1];
						s2 ^= m_state[2];
						s3 ^= m_state[3];
					}
					operator()();
				}
			}

			m_state[0] = s0;
			m_state[1] = s1;
			m_state[2] = s2;
			m_state[3] = s3;
		}

		[[nodiscard]]
		static constexpr result_type min() noexcept
		{
			return std::numeric_limits<result_type>::lowest();
		}

		[[nodiscard]]
		static constexpr result_type max() noexcept
		{
			return std::numeric_limits<result_type>::max();
		}

		void seed(result_type seed = ::time(NULL)) noexcept
		{
			m_state = SplitMix64{ seed }.generateSeedSequence<4>();
		}

		[[nodiscard]]
		friend bool operator ==(const Xoshiro256PlusPlus& lhs, const Xoshiro256PlusPlus& rhs) noexcept
		{
			return (lhs.m_state == rhs.m_state);
		}

		[[nodiscard]]
		friend bool operator !=(const Xoshiro256PlusPlus& lhs, const Xoshiro256PlusPlus& rhs) noexcept
		{
			return (lhs.m_state != rhs.m_state);
		}

	private:

		state_type m_state;
	};

	// xoshiro128++
	// Output: 32 bits
	// Period: 2^128 - 1
	// Footprint: 16 bytes
	// Original implementation: http://prng.di.unimi.it/xoshiro128plusplus.c
	// Version: 1.0
	class Xoshiro128PlusPlus
	{
	public:

		using state_type = std::array<std::uint32_t, 4>;
		using result_type = std::uint32_t;

		constexpr Xoshiro128PlusPlus(std::uint64_t seed = ::time(NULL)) noexcept
			: m_state()
		{
			SplitMix64 splitmix{ seed };

			for (auto& state : m_state)
			{
				state = static_cast<std::uint32_t>(splitmix());
			}
		}

		constexpr Xoshiro128PlusPlus(state_type state) noexcept
			: m_state(state) {}

		result_type operator()() noexcept
		{
			const result_type result = detail::RotL(m_state[0] + m_state[3], 7) + m_state[0];
			const std::uint32_t t = m_state[1] << 9;

			m_state[2] ^= m_state[0];
			m_state[3] ^= m_state[1];
			m_state[1] ^= m_state[2];
			m_state[0] ^= m_state[3];
			m_state[2] ^= t;
			m_state[3] = detail::RotL(m_state[3], 11);
			return result;
		}

		// This is the jump function for the generator. It is equivalent
		// to 2^64 calls to next(); it can be used to generate 2^64
		// non-overlapping subsequences for parallel computations.
		constexpr void jump() noexcept
		{
			constexpr std::uint32_t JUMP[] = { 0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b };

			std::uint32_t s0 = 0;
			std::uint32_t s1 = 0;
			std::uint32_t s2 = 0;
			std::uint32_t s3 = 0;

			for (std::uint32_t jump : JUMP)
			{
				for (int b = 0; b < 32; ++b)
				{
					if (jump & UINT32_C(1) << b)
					{
						s0 ^= m_state[0];
						s1 ^= m_state[1];
						s2 ^= m_state[2];
						s3 ^= m_state[3];
					}
					operator()();
				}
			}

			m_state[0] = s0;
			m_state[1] = s1;
			m_state[2] = s2;
			m_state[3] = s3;
		}

		// This is the long-jump function for the generator. It is equivalent to
		// 2^96 calls to next(); it can be used to generate 2^32 starting points,
		// from each of which jump() will generate 2^32 non-overlapping
		// subsequences for parallel distributed computations.
		constexpr void longJump() noexcept
		{
			constexpr std::uint32_t LONG_JUMP[] = { 0xb523952e, 0x0b6f099f, 0xccf5a0ef, 0x1c580662 };

			std::uint32_t s0 = 0;
			std::uint32_t s1 = 0;
			std::uint32_t s2 = 0;
			std::uint32_t s3 = 0;

			for (std::uint32_t jump : LONG_JUMP)
			{
				for (int b = 0; b < 32; ++b)
				{
					if (jump & UINT32_C(1) << b)
					{
						s0 ^= m_state[0];
						s1 ^= m_state[1];
						s2 ^= m_state[2];
						s3 ^= m_state[3];
					}
					operator()();
				}
			}

			m_state[0] = s0;
			m_state[1] = s1;
			m_state[2] = s2;
			m_state[3] = s3;
		}

		[[nodiscard]]
		static constexpr result_type min() noexcept
		{
			return std::numeric_limits<result_type>::lowest();
		}

		[[nodiscard]]
		static constexpr result_type max() noexcept
		{
			return std::numeric_limits<result_type>::max();
		}

		void seed(result_type seed = ::time(NULL)) noexcept
		{
			SplitMix64 splitmix{ seed };

			for (auto& state : m_state)
			{
				state = static_cast<std::uint32_t>(splitmix());
			}
		}

		[[nodiscard]]
		friend bool operator ==(const Xoshiro128PlusPlus& lhs, const Xoshiro128PlusPlus& rhs) noexcept
		{
			return (lhs.m_state == rhs.m_state);
		}

		[[nodiscard]]
		friend bool operator !=(const Xoshiro128PlusPlus& lhs, const Xoshiro128PlusPlus& rhs) noexcept
		{
			return (lhs.m_state != rhs.m_state);
		}

	private:

		state_type m_state;
	};

	using XorShift64 = Xoshiro256PlusPlus;
	using XorShift32 = Xoshiro128PlusPlus;
}

#endif // _RANDOM_XORSHIFT_H_