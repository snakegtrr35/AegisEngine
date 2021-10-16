#pragma once

#ifndef INCLUDE_UUID_H
#define INCLUDE_UUID_H

#include	<mutex>
#include	"../random/Xorshift.h"
#include	"uuid_utility.h"

namespace Aegis
{
	using uuid = uuids::uuid;

	class Uuid
	{
	public:

		[[nodiscard]]
		static uuid GetUuid() noexcept;

		static void ReSeed(uint32 sedd) noexcept;

	private:

		static std::mutex lock_mutex;
		static uuids::uuid_random_generator<XorShift32> gen;

		Uuid();
		~Uuid();
	};
}

#endif // !INCLUDE_UUID_H