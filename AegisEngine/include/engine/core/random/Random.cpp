#include "Random.h"

namespace aegis
{
	Random Random::Insrance;
	std::mutex Random::mutex;

	Random::Random(uint64 seed) :
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

	uint32 Random::GetRandomUint() noexcept
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

	uint64 Random::GetRandomUlong() noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		return Random::GetInsrance()->random64();
	}

	void Random::ReSeed(uint64 seed) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		Random::GetInsrance()->random = XorShift32(seed);
	}

	void Random::ReSeed64(uint64 seed) noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		Random::GetInsrance()->random64 = XorShift64(seed);
	}
}