#pragma once

#ifndef _INCLEDE_OGG_VORBIS_LOAD_H_
#define _INCLEDE_OGG_VORBIS_LOAD_H_

#include "AudioDefine.h"
#include <vorbis/vorbisfile.h>

namespace Aegis
{
	namespace OggVorbis
	{
		void Open(const std::string_view& fileName, AudioInfo* info);
		void Close(AudioInfo* info);
		void Uninit(AudioInfo* info, PlayData* playData);

		void Load(AudioInfo* info, PlayData* playData);
		void Stream(AudioInfo* info, char* buffer, const uint64 size);

		void reset(AudioInfo* info);
	}
}

#endif // !_INCLEDE_OGG_VORBIS_LOAD_H_