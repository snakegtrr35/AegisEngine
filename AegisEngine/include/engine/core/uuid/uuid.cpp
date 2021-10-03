#include "uuid.h"

namespace Aegis
{
	std::mutex Uuid::lock_mutex;

	uuids::uuid_random_generator<XorShift32> Uuid::gen;

	Uuid::Uuid()
	{
		XorShift32 generator;

		uuids::uuid_random_generator<XorShift32> temp_gen{ generator };

		gen = temp_gen;
	}

	Uuid::~Uuid()
	{
	}

	uuid Uuid::GetUuid() noexcept
	{
		std::lock_guard<std::mutex> lock(lock_mutex);
		return gen();
	}

	void Uuid::ReSeed(std::uint64_t seed) noexcept
	{
		std::lock_guard<std::mutex> lock(lock_mutex);

		XorShift32 generator;
		generator.seed(seed);

		uuids::uuid_random_generator<XorShift32> temp_gen{ generator };

		gen = temp_gen;
	}
}