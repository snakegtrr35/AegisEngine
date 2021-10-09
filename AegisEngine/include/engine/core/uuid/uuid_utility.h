#pragma once

#include <uuid.h>

namespace uuids
{
	template<class Archive>
	void serialize(Archive& archive, uuids::uuid& uuid)
	{
		archive(cereal::make_nvp("uuid", uuid.data));
	}

	//template<class Archive>
	//void save(Archive& archive, uuids::uuid& uuid)
	//{
	//	archive(cereal::make_nvp("uuid", uuid.data));
	//}

	//template<class Archive>
	//void load(Archive& archive, uuids::uuid& uuid)
	//{
	//	std::array<std::uint8_t, 16> data = {};

	//	archive(cereal::make_nvp("uuid", data));

	//	uuid = data;
	//}
}