#include "Mp3.h"
//#include "audio_clip.h"

#define MINIMP3_IMPLEMENTATION
#include "external\mp3\minimp3_ex.h"

namespace aegis
{
	//// memory buffer
	//static std::vector<uint8> data;

	namespace Mp3
	{
		void Open(const std::string_view& fileName, AudioInfo* info)
		{
			mp3dec_ex_t* dec = new mp3dec_ex_t;
			if (mp3dec_ex_open(dec, fileName.data(), MP3D_SEEK_TO_SAMPLE))
			{
				assert(false);
			}

			// Open a stream from a memory buffer
			/*{
				{
					ifstream file(fileName.data(), std::ios_base::in | std::ios_base::binary);
					assert(file);

					file.seekg(0, std::ios_base::end);
					int filesize = (int)file.tellg();
					file.seekg(0, std::ios_base::beg);

					data.resize(filesize);
					file.read(reinterpret_cast<char*>(data.data()), filesize);

					file.close();

				}

				if (mp3dec_ex_open_buf(dec, data.data(), sizeof(uint8) * data.size(), MP3D_SEEK_TO_SAMPLE))
				{
					assert(false);
				}
			}*/

			info->Channels = dec->info.channels;
			info->SamplesPerSec = dec->info.hz;
			info->BitsPerSample = 16;
			info->BlockAlign = info->Channels * info->BitsPerSample / 8;
			info->AvgBytesPerSec = info->SamplesPerSec * info->BlockAlign;
			info->DecodedSize = dec->samples * sizeof(mp3d_sample_t);
			info->AudioDate = dec;
			info->Type = AudioType::Mp3;
		}

		void Close(AudioInfo* info)
		{
			if (nullptr != info->AudioDate)
			{
				mp3dec_ex_t* dec = reinterpret_cast<mp3dec_ex_t*>(info->AudioDate);

				SAFE_DELETE(dec->index.frames);
				SAFE_DELETE(dec);
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
			mp3dec_ex_t* dec = reinterpret_cast<mp3dec_ex_t*>(info->AudioDate);
			mp3d_sample_t* data = new mp3d_sample_t[info->DecodedSize];
			size_t readSize = mp3dec_ex_read(dec, reinterpret_cast<mp3d_sample_t*>(data), dec->samples);
			if (readSize != dec->samples) /* normal eof or error condition */
			{
				if (dec->last_error)
				{
					assert(false);
				}
			}

			playData->Length = info->DecodedSize;
			playData->PlayLength = info->DecodedSize / info->BlockAlign;
			playData->Data = reinterpret_cast<uint8*>(data);
		}
	}
}