#pragma once

#include <uuid.h>

namespace uuids
{
	template<class Archive>
	void save(Archive& archive, uuids::uuid const& uuid)
	{
		archive(cereal::make_nvp("uuid", uuid.data));
	}

	template<class Archive>
	void load(Archive& archive, uuids::uuid uuid)
	{
		archive(cereal::make_nvp("uuid", uuid.data));
	}
}