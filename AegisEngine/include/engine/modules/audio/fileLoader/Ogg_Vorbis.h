#pragma once

#ifndef _INCLEDE_OGG_VORBIS_LOAD_H_
#define _INCLEDE_OGG_VORBIS_LOAD_H_

#include "../AudioDefine.h"

namespace aegis::audio
{
	namespace OggVorbis
	{
		void Open(const std::string_view& fileName, AudioInfo* info);
		void Close(AudioInfo* info);
		void Uninit(AudioInfo* info, PlayData* playData);

		void Load(AudioInfo* info, PlayData* playData);
		void Stream(AudioInfo* info, char* buffer, const std::uint64_t size);

		void reset(AudioInfo* info);
	}
}

#endif // !_INCLEDE_OGG_VORBIS_LOAD_H_