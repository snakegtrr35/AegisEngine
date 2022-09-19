#pragma once

#ifndef _INCLEDE_AUDIO_DIFINE_H_
#define _INCLEDE_AUDIO_DIFINE_H_

#include "stdafx.h"

namespace aegis::audio
{
	enum class AudioType : std::uint8_t
	{
		Wav,
		Ogg_Vorbis,
		Ogg_Opus,
		Mp3,
		Num,
		None
	};

	struct AudioInfo
	{
		//! チャンネル数(モノラル:1 ステレオ:2)
		std::uint16_t Channels;
		//! サンプル当たりのビット数(8bit:8 16bit:16)
		std::uint16_t BitsPerSample;
		//! サンプリングレート
		std::uint32_t SamplesPerSec;
		//! データ速度(秒間当たりのデータ量
		std::uint32_t AvgBytesPerSec;
		//! デコードサイズ
		std::uint64_t DecodedSize;
		//! デコードクラスのポインタ
		void* AudioDate = nullptr;
		//! ブロックサイズ(16bit ステレオなら 2*2=4)
		std::uint16_t BlockAlign;
		//! オーディオタイプ
		AudioType Type = AudioType::None;

		std::array<std::unique_ptr<std::uint8_t[]>, 2> Stream;
		std::uint32_t submitCount = 0;
	};

	struct PlayData
	{
		std::uint32_t Length;
		std::uint32_t PlayLength;
		std::uint8_t* Data = nullptr;
	};
}

#endif // !_INCLEDE_AUDIO_DIFINE_H_