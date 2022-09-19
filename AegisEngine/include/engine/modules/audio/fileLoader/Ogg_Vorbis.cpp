#include "Ogg_Vorbis.h"
#include "../audio_clip.h"

#include <ogg\ogg\include\ogg\os_types.h>
#include <ogg\vorbis\include\vorbis\vorbisfile.h>

//// vorbisをリンク
//#pragma comment ( lib, "vorbis_static.lib" )
//#pragma comment ( lib, "vorbisfile_static.lib" )
//// liboggをリンク
//#pragma comment(lib, "libogg.lib")

#if _DEBUG
//#pragma comment(lib, "ogg/vorbis/lib/Debug/vorbis_static")
//#pragma comment(lib, "ogg/vorbis/lib/Debug/vorbisfile_static")
#pragma comment(lib, "./external/ogg/vorbis/lib/Debug/vorbis_static")
#pragma comment(lib, "./external/ogg/vorbis/lib/Debug/vorbisfile_static")
// liboggをリンク
//#pragma comment(lib, "ogg/ogg/lib/Debug/libogg.lib")
#pragma comment(lib, "./external/ogg/ogg/lib/Debug/libogg")
#else
//#pragma comment(lib, "ogg/vorbis/lib/Release/vorbis_static")
//#pragma comment(lib, "ogg/vorbis/lib/Release/vorbisfile_static")
#pragma comment(lib, "./external/ogg/vorbis/lib/Release/vorbis_static")
#pragma comment(lib, "./external/ogg/vorbis/lib/Release/vorbisfile_static")
// liboggをリンク
//#pragma comment(lib, "ogg/ogg/lib/Release/libogg.lib")
#pragma comment(lib, "./external/ogg/ogg/lib/Release/libogg")
#endif // !DEBUG

namespace aegis::audio
{
	namespace OggVorbis
	{
		void Open(const std::string_view& fileName, AudioInfo* info)
		{
			OggVorbis_File* ovf = new OggVorbis_File();
			int error = ov_fopen(fileName.data(), ovf);
			if (error != 0) {
				switch (error)
				{
					case OV_EREAD:       break;
					case OV_ENOTVORBIS:  break;
					case OV_EVERSION:    break;
					case OV_EBADHEADER:  break;
					case OV_EFAULT:      break;
					default:             break;
				}
				assert(false);
			}

			// Oggファイルの音声フォーマット情報
			vorbis_info* oggInfo = ov_info(ovf, -1);

			info->Channels = oggInfo->channels;
			info->SamplesPerSec = oggInfo->rate;
			info->BitsPerSample = 16;
			info->BlockAlign = info->Channels * info->BitsPerSample / 8;
			info->AvgBytesPerSec = info->SamplesPerSec * info->BlockAlign;

			// 合計サイズの取得
			ogg_int64_t decodedSize = ov_pcm_total(ovf, -1) * info->BlockAlign;

			info->DecodedSize = ov_pcm_total(ovf, -1) * info->BlockAlign;
			info->AudioDate = ovf;
			info->Type = AudioType::Ogg_Vorbis;

			// 読み込み位置を最初に戻す
			ov_time_seek(ovf, 0.0);
		}

		void Close(AudioInfo* info)
		{
			if (nullptr != info->AudioDate)
			{
				ov_clear(reinterpret_cast<OggVorbis_File*>(info->AudioDate));
				//SAFE_DELETE(info->AudioDate)
				if (info->AudioDate)
				{
					delete info->AudioDate;
					info->AudioDate = nullptr;
				}
			}
		}

		void Uninit(AudioInfo* info, PlayData* playData)
		{
			Close(info);

			//SAFE_DELETE_ARRAY(playData->Data);
			if (playData->Data)
			{
				delete[] playData->Data;
				playData->Data = nullptr;
			}
		}

		void Load(AudioInfo* info, PlayData* playData)
		{
			std::uint8_t* data = new std::uint8_t[info->DecodedSize];
			char* pBuf = reinterpret_cast<char*>(data);
			int bitstream = 0;
			int readSize = 0;
			while ((readSize = ov_read(reinterpret_cast<OggVorbis_File*>(info->AudioDate), pBuf, 4096, 0, 2, 1, &bitstream)) > 0)
			{
				pBuf += readSize;
			}

			playData->Length = info->DecodedSize;
			playData->PlayLength = info->DecodedSize / info->BlockAlign;
			playData->Data = data;
		}

		void Stream(AudioInfo* info, char* buffer, const std::uint64_t size)
		{
			int requestSize = 4096;
			int comSize = 0;
			int bitstream = 0;
			int readSize = 0;

			memset(buffer, 0x00, size);

			while (comSize < size)
			{
				readSize = ov_read(reinterpret_cast<OggVorbis_File*>(info->AudioDate), (buffer + comSize), requestSize, 0, 2, 1, &bitstream);

				if (0 == readSize) {
					// ループする場合読み込み位置を最初に戻す
					ov_time_seek(reinterpret_cast<OggVorbis_File*>(info->AudioDate), 0.0);
				}

				comSize += readSize;

				if (size - comSize < 4096) {
					requestSize = size - comSize;
				}
			}
		}

		void reset(AudioInfo* info)
		{
			ov_pcm_seek(reinterpret_cast<OggVorbis_File*>(info->AudioDate), 0);
		}
	}
}