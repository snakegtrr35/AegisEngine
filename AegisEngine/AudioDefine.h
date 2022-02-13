#pragma once

#ifndef _INCLEDE_AUDIO_DIFINE_H_
#define _INCLEDE_AUDIO_DIFINE_H_

namespace aegis
{
	enum class AudioType : uint8
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
		uint16 Channels;
		//! サンプル当たりのビット数(8bit:8 16bit:16)
		uint16 BitsPerSample;
		//! サンプリングレート
		uint32 SamplesPerSec;
		//! データ速度(秒間当たりのデータ量
		uint32 AvgBytesPerSec;
		//! デコードサイズ
		uint64 DecodedSize;
		//! デコードクラスのポインタ
		void* AudioDate = nullptr;
		//! ブロックサイズ(16bit ステレオなら 2*2=4)
		uint16 BlockAlign;
		//! オーディオタイプ
		AudioType Type = AudioType::None;

		array<std::unique_ptr<uint8[]>, 2> Stream;
		uint32 submitCount = 0;
	};

	struct PlayData
	{
		uint32 Length;
		uint32 PlayLength;
		uint8* Data = nullptr;
	};
}

#endif // !_INCLEDE_AUDIO_DIFINE_H_