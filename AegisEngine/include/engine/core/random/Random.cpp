#include "Random.h"

namespace Aegis
{
	Random Random::Insrance;
	std::mutex Random::mutex;

	Random::Random(std::uint64_t seed) :
		random(seed), random64(seed)
	{
	}

	Random::~Random()
	{
	}

	Random* Random::GetInsrance() noexcept
	{
		return &Insrance;
	}

	std::uint32_t Random::GetRandomUint() noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		return Random::GetInsrance()->random();
	}

	float Random::GetRandomfloat() noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		return FloatFromBits(Random::GetInsrance()->random());
	}

	float Random::Range(const float min, const float max) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		return FloatFromBits(Random::GetInsrance()->random(), min, max);
	}

	std::uint64_t Random::GetRandomUlong() noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		return Random::GetInsrance()->random64();
	}

	void Random::ReSeed(std::uint64_t seed) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		Random::GetInsrance()->random = XorShift32(seed);
	}

	void Random::ReSeed64(std::uint64_t seed) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		Random::GetInsrance()->random64 = XorShift64(seed);
	}
}