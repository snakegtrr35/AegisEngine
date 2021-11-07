#include "Ogg_Opus.h"
#include "audio_clip.h"

#include <opusfile.h>
// libopusとopusfileをリンク
#pragma comment(lib, "opus.lib")
#pragma comment(lib, "opusfile.lib")

namespace Aegis
{
	//// memory buffer
	//static std::vector<uint8> data;

	namespace OggOpus
	{
		void Open(const std::string_view& fileName, AudioInfo* info)
		{
			int ret = 0;
			OggOpusFile* of = nullptr;
			of = op_open_file(fileName.data(), &ret);
			
			//波形情報の取得
			const OpusHead* pHead = nullptr;
			pHead = op_head(of, -1);
			if (!pHead) {
				assert(false);
			}

			// Open a stream from a memory buffer
			//{
			//	{
			//		ifstream file(fileName.data(), std::ios_base::in | std::ios_base::binary);
			//		assert(file);

			//		file.seekg(0, std::ios_base::end);
			//		int filesize = (int)file.tellg();
			//		file.seekg(0, std::ios_base::beg);

			//		data.resize(filesize);
			//		file.read(reinterpret_cast<char*>(data.data()), filesize);

			//		file.close();

			//	}
			//	
			//	of = op_open_memory(data.data(), sizeof(uint8) * data.size(), &ret);

			//	//波形情報の取得
			//	pHead = op_head(of, -1);
			//	if (!pHead) {
			//		assert(false);
			//	}
			//}

			info->Channels = pHead->channel_count;
			// サンプリングレート(48000固定)
			info->SamplesPerSec = 48000;
			info->BitsPerSample = 16;
			info->BlockAlign = info->Channels * info->BitsPerSample / 8;
			info->AvgBytesPerSec = info->SamplesPerSec * info->BlockAlign;
			info->DecodedSize = op_pcm_total(of, -1) * info->BlockAlign;
			info->AudioDate = of;
			info->Type = AudioType::Ogg_Opus;
		}

		void Close(AudioInfo* info)
		{
			if (nullptr != info->AudioDate)
			{
				op_free(reinterpret_cast<OggOpusFile*>(info->AudioDate));
				info->AudioDate = nullptr;
			}
		}

		void Uninit(AudioInfo* info, PlayData* playData)
		{
			Close(info);

			SAFE_DELETE_ARRAY(playData->Data);
		}

		void Load(AudioInfo* info, PlayData* playData)
		{
			uint8* data = new uint8[info->DecodedSize];
			char* pBuf = reinterpret_cast<char*>(data);
			int readSize;
			while ((readSize = op_read(reinterpret_cast<OggOpusFile*>(info->AudioDate), (opus_int16*)pBuf, 8096, nullptr)) > 0)
			{
				pBuf += readSize * info->BlockAlign;
			}
			

			playData->Length = info->DecodedSize;
			playData->PlayLength = info->DecodedSize / info->BlockAlign;
			playData->Data = data;
		}
	}
}