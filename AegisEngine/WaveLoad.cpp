#include "WaveLoad.h"
#include "audio_clip.h"

#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

namespace Aegis
{
	namespace Wave
	{
		void Open(const std::string_view& fileName, AudioInfo* info)
		{
			// サウンドデータ読込
			WAVEFORMATEX wfx {};

			HMMIO hmmio = nullptr;
			MMIOINFO mmioinfo {};
			MMCKINFO riffchunkinfo {};
			MMCKINFO datachunkinfo {};
			MMCKINFO mmckinfo {};

#ifdef UNICODE
			hmmio = mmioOpen((LPWSTR)stringTowstring(fileName.data()).c_str(), &mmioinfo, MMIO_READ);
#else
			hmmio = mmioOpen((LPSTR)fileName.data(), &mmioinfo, MMIO_READ);
#endif // !UNICODE
			assert(hmmio);

			riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
			mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

			mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
			mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

			if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
			{
				mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
			}
			else
			{
				PCMWAVEFORMAT pcmwf {};
				mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
				memset(&wfx, 0x00, sizeof(wfx));
				memcpy(&wfx, &pcmwf, sizeof(pcmwf));
				wfx.cbSize = 0;
			}
			mmioAscend(hmmio, &mmckinfo, 0);

			datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
			mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);

			info->Channels = wfx.nChannels;
			info->SamplesPerSec = wfx.nSamplesPerSec;
			info->BitsPerSample = wfx.wBitsPerSample;
			info->BlockAlign = wfx.nBlockAlign;
			info->AvgBytesPerSec = wfx.nAvgBytesPerSec;
			info->DecodedSize = datachunkinfo.cksize;
			info->AudioDate = hmmio;
			info->Type = AudioType::Wav;
		}

		void Close(AudioInfo* info)
		{
			if (nullptr != info->AudioDate)
			{
				mmioClose(reinterpret_cast<HMMIO>(info->AudioDate), 0);
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
			uint64 buflen = info->DecodedSize;
			uint8* data = new uint8[buflen];
			uint32 readlen = mmioRead(reinterpret_cast<HMMIO>(info->AudioDate), reinterpret_cast<HPSTR>(data), buflen);

			playData->Length = readlen;
			playData->PlayLength = readlen / info->BlockAlign;
			playData->Data = data;
		}
	}
}