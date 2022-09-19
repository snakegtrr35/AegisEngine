#pragma once

#ifndef _INCLEDE_MP3_LOAD_H_
#define _INCLEDE_MP3_LOAD_H_

#include "../AudioDefine.h"

namespace aegis::audio
{
	namespace Mp3
	{
		void Open(const std::string_view& fileName, AudioInfo* info);
		void Close(AudioInfo* info);
		void Uninit(AudioInfo* info, PlayData* playData);

		void Load(AudioInfo* info, PlayData* playData);
	}
}

#endif // !_INCLEDE_MP3_LOAD_H_