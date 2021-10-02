#include "uuid.h"

namespace Aegis
{
	Uuid Uuid::Insrance;
	std::mutex Uuid::mutex;

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
		std::lock_guard<std::mutex> lock(mutex);
		return gen();
	}
}