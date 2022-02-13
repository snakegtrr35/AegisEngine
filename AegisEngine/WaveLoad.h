#pragma once

#ifndef _INCLEDE_WAVELOAD_H_
#define _INCLEDE_WAVELOAD_H_

#include "AudioDefine.h"

namespace aegis
{
	namespace Wave
	{
		void Open(const std::string_view& fileName, AudioInfo* info);
		void Close(AudioInfo* info);
		void Uninit(AudioInfo* info, PlayData* playData);

		void Load(AudioInfo* info, PlayData* playData);
	}
}

#endif // !_INCLEDE_WAVELOAD_H_