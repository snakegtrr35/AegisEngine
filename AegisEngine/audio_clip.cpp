#include "main.h"
#include "audio_clip.h"

#include "WaveLoad.h"
#include "Ogg_Vorbis.h"
#include "Ogg_Opus.h"
#include "Mp3.h"

bool AUDIO_MANAGER::Enable = true;

#ifdef UNICODE
map<wstring, CAudioClip*> AUDIO_MANAGER::Sound_Dates;
#else
map<string, CAudioClip*> AUDIO_MANAGER::Sound_Dates;
#endif // !UNICODE

IXAudio2*					AUDIO_MANAGER::Xaudio;
IXAudio2MasteringVoice*		AUDIO_MANAGER::MasteringVoice;

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE hBufferEndEvent;
	VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~VoiceCallback() { CloseHandle(hBufferEndEvent); }

	//Called when the voice has just finished playing a contiguous audio stream.
	void OnStreamEnd() { SetEvent(hBufferEndEvent); }

	//Unused methods are stubs
	void OnVoiceProcessingPassEnd() { }
	void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
	void OnBufferEnd(void* pBufferContext) {
		int a = 0;
	}
	void OnBufferStart(void* pBufferContext) {    }
	void OnLoopEnd(void* pBufferContext) {    }
	void OnVoiceError(void* pBufferContext, HRESULT Error) { }
};

VoiceCallback voiceCallback;

void callBack()
{
	int a = 0;
}

WAVEFORMATEX AudioInfoToWAVEFORMATEX(const Aegis::AudioInfo& info)
{
	WAVEFORMATEX wfx {};

	// PCMなので固定
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	//チャンネル数
	wfx.nChannels = info.Channels;
	//サンプリングレート
	wfx.nSamplesPerSec = info.SamplesPerSec;
	//サンプル当たりのビット数(8bit:8 16bit:16)
	wfx.wBitsPerSample = 16;
	//ブロックサイズ(16bit ステレオなら 2*2=4)
	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
	//データ速度(秒間当たりのデータ量)
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	return wfx;
}

#ifdef UNICODE
void CAudioClip::Load(const wchar_t* FileName)
#else
void CAudioClip::Load(const char* FileName)
#endif // !UNICODE
{
	// サウンドデータ読込
	WAVEFORMATEX wfx {};

	if (wstring::npos != wstring(FileName).find(L".wav"))
	{
		Aegis::Wave::Open(wstringTostring(FileName), &Info);
		//Aegis::Wave::Close(&Info);
	}
	else if (wstring::npos != wstring(FileName).find(L".ogg"))
	{
		Load_Ogg(FileName);
	}
	else if (wstring::npos != wstring(FileName).find(L".mp3"))
	{
		Aegis::Mp3::Open(wstringTostring(FileName), &Info);
		//Aegis::Mp3::Close(&Info);
	}

	wfx = AudioInfoToWAVEFORMATEX(Info);

	// サウンドソース生成
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		AUDIO_MANAGER::Get_Xaudio()->CreateSourceVoice(&SourceVoice[j], &wfx/*, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &voiceCallback*/);
	}
}


void CAudioClip::Unload()
{
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		if (SourceVoice[j])
		{
			SourceVoice[j]->Stop();
			SourceVoice[j]->DestroyVoice();
			SourceVoice[j] = NULL;
		}
	}

	switch (Info.Type)
	{
		case Aegis::AudioType::Wav:
			Aegis::Wave::Uninit(&Info, &AudioData);
			break;

		case Aegis::AudioType::Ogg_Vorbis:
			Aegis::OggVorbis::Uninit(&Info, &AudioData);
			break;

		case Aegis::AudioType::Ogg_Opus:
			Aegis::OggOpus::Uninit(&Info, &AudioData);
			break;

		case Aegis::AudioType::Mp3:
			Aegis::Mp3::Uninit(&Info, &AudioData);
			break;

		default:
		break;
	}
}

void CAudioClip::Play( bool Loop )
{
	for( int j = 0; j < SOUND_SOURCE_MAX; j++ )
	{
		XAUDIO2_VOICE_STATE state;

		SourceVoice[j]->GetState( &state );

		if( state.BuffersQueued == 0 )
		{
			//Sound->SourceVoice->Stop();
			//Sound->SourceVoice->FlushSourceBuffers();

			if (nullptr == AudioData.Data)
			{
				switch (Info.Type)
				{
					case Aegis::AudioType::Wav:
						Aegis::Wave::Load(&Info, &AudioData);
						break;

					case Aegis::AudioType::Ogg_Vorbis:
						Aegis::OggVorbis::Load(&Info, &AudioData);
						break;

					case Aegis::AudioType::Ogg_Opus:
						Aegis::OggOpus::Load(&Info, &AudioData);
						break;

					case Aegis::AudioType::Mp3:
						Aegis::Mp3::Load(&Info, &AudioData);
						break;

					default:
						assert(false);
						break;
				}
			}

			// バッファ設定
			XAUDIO2_BUFFER bufinfo;

			memset(&bufinfo,0x00,sizeof(bufinfo));
			//bufinfo.AudioBytes = Length;
			//bufinfo.pAudioData = SoundData;
			//bufinfo.PlayBegin = 0;
			//bufinfo.PlayLength = PlayLength;

			bufinfo.AudioBytes = AudioData.Length;
			bufinfo.pAudioData = AudioData.Data;
			bufinfo.PlayBegin = 0;
			bufinfo.PlayLength = AudioData.PlayLength;

			// ループ設定
			if( Loop )
			{
				bufinfo.LoopBegin = 0;
				//bufinfo.LoopLength = PlayLength;
				bufinfo.LoopLength = AudioData.PlayLength;
				bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
			}

			SourceVoice[j]->SubmitSourceBuffer( &bufinfo, NULL );

			// 再生
			SourceVoice[j]->Start();

			break;
		}
	}

}

void CAudioClip::PlayLoop()
{
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		XAUDIO2_VOICE_STATE state;

		SourceVoice[j]->GetState(&state);

		if (state.BuffersQueued == 0)
		{
			//Sound->SourceVoice->Stop();
			//Sound->SourceVoice->FlushSourceBuffers();

			if (nullptr == AudioData.Data)
			{
				switch (Info.Type)
				{
					case Aegis::AudioType::Wav:
						//Aegis::Wave::Load(&Info, &AudioData);
						break;

					case Aegis::AudioType::Ogg_Vorbis:
						Info.Stream[0] = make_unique<Aegis::uint8[]>(Info.AvgBytesPerSec * 1.0f);
						Info.Stream[1] = make_unique<Aegis::uint8[]>(Info.AvgBytesPerSec * 1.0f);

						Aegis::OggVorbis::reset(&Info);

						Aegis::OggVorbis::Stream(&Info, (char*)Info.Stream[0].get(), Info.AvgBytesPerSec * 1.0f);
						Aegis::OggVorbis::Stream(&Info, (char*)Info.Stream[1].get(), Info.AvgBytesPerSec * 1.0f);

						break;

					case Aegis::AudioType::Ogg_Opus:
						//Aegis::OggOpus::Load(&Info, &AudioData);
						break;

					case Aegis::AudioType::Mp3:
						//Aegis::Mp3::Load(&Info, &AudioData);
						break;

					default:
						assert(false);
						break;
				}
			}

			// バッファ設定
			XAUDIO2_BUFFER bufinfo;

			memset(&bufinfo, 0x00, sizeof(bufinfo));

			bufinfo.AudioBytes = Info.AvgBytesPerSec * 1.0f;
			bufinfo.pAudioData = Info.Stream[0].get();

			SourceVoice[j]->SubmitSourceBuffer(&bufinfo, NULL);



			bufinfo.AudioBytes = Info.AvgBytesPerSec * 1.0f;
			bufinfo.pAudioData = Info.Stream[1].get();

			SourceVoice[j]->SubmitSourceBuffer(&bufinfo, NULL);

			// 再生
			SourceVoice[j]->Start();

			std::thread th(CAudioClip::Polling, SourceVoice[j], &Info);
			th.detach();

			break;
		}
	}

}

void CAudioClip::Polling(IXAudio2SourceVoice* SourceVoice, Aegis::AudioInfo* info)
{
	while (AUDIO_MANAGER::Enable)
	{
		Sleep(1);

		if (!AUDIO_MANAGER::Enable)
		{
			return;
		}

		XAUDIO2_VOICE_STATE state;
		SourceVoice->GetState(&state);

		if (state.BuffersQueued < 2)
		{
			Aegis::OggVorbis::Stream(info, (char*)info->Stream[info->submitCount].get(), info->AvgBytesPerSec * 1.0f);

			XAUDIO2_BUFFER bufferDesc = { 0 };
			bufferDesc.AudioBytes = info->AvgBytesPerSec * 1.0f;
			bufferDesc.pAudioData = info->Stream[info->submitCount].get();

			SourceVoice->SubmitSourceBuffer(&bufferDesc);

			info->submitCount = (info->submitCount + 1) % 2;
			
		}
	}
}

void CAudioClip::Stop()
{
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		SourceVoice[j]->Stop();
	}
}

#ifdef UNICODE
void CAudioClip::Set_Name(const wstring& name)
#else
void CAudioClip::Set_Name(const string& name)
#endif // !UNICODE
{
	Name = name;
}

#ifdef UNICODE
wstring* const CAudioClip::Get_Name()
#else
string* const CAudioClip::Get_Name()
#endif // !UNICODE
{
	return &Name;
}

void CAudioClip::Set_Tag(const SOUND_TAG tag)
{
	Tag = tag;
}

const SOUND_TAG CAudioClip::Get_Tag()
{
	return Tag;
}



void AUDIO_MANAGER::Init(void)
{
	// XAudio生成
	XAudio2Create(&Xaudio, 0);

	// マスタリングボイス生成
	Xaudio->CreateMasteringVoice(&MasteringVoice);

	AUDIO_MANAGER::Load();
}

void AUDIO_MANAGER::Uninit(void)
{
	AUDIO_MANAGER::Enable = false;

	for (int i = 0; i < SOUND_FILE_COUNT; i++)
	{
		if(nullptr != Sound_Dates[g_SoundFiles[i].Name]) Sound_Dates[g_SoundFiles[i].Name]->Unload();
		SAFE_DELETE(Sound_Dates[g_SoundFiles[i].Name]);
	}
	Sound_Dates.clear();

	MasteringVoice->DestroyVoice();

	SAFE_RELEASE(Xaudio);

	CoUninitialize();
}

// マップへの追加
void AUDIO_MANAGER::Add_Sound_Object(SOUND_INDEX index) {

	CAudioClip* sound = new CAudioClip();

	sound->Set_Name(g_SoundFiles[(int)index].Name);
	sound->Set_Tag(g_SoundFiles[(int)index].Tag);
	sound->Load(g_SoundFiles[(int)index].Name.c_str());

	Sound_Dates.insert(make_pair(g_SoundFiles[(int)index].Name, sound));
};

// マップから再生
void AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX index, bool flag) {
	Sound_Dates[g_SoundFiles[(int)index].Name]->Play(flag);
};

// マップから再生
void AUDIO_MANAGER::Play_Sound(SOUND_INDEX index) {
	Sound_Dates[g_SoundFiles[(int)index].Name]->PlayLoop();
};

// マップからストップ
void AUDIO_MANAGER::Stop_Sound_Object(SOUND_INDEX index) {

	if (SOUND_INDEX::SOUND_INDEX_MAX == index)
	{
		if (!Sound_Dates.empty())
		{
			for (int i = 0; i < SOUND_FILE_COUNT; i++)
			{
				if (nullptr == Sound_Dates[g_SoundFiles[i].Name])
				{
					Sound_Dates[g_SoundFiles[i].Name]->Stop();
				}
			}
		}
	}
	else
	{
		Sound_Dates[g_SoundFiles[(int)index].Name]->Stop();
	}
};

IXAudio2* const AUDIO_MANAGER::Get_Xaudio()
{
	return Xaudio;
}

void AUDIO_MANAGER::Load()
{
	for (int i = 0; i < SOUND_FILE_COUNT; i++)
	{
		Add_Sound_Object((SOUND_INDEX)i);
	}
}

struct _ogg_header
{
	Aegis::uint32 Signature;
	Aegis::uint8 Version;
	Aegis::uint8 Flags;
	Aegis::uint64 GranulePosition;
	Aegis::uint32 SerialNumber;
	Aegis::uint32 SequenceNumber;
	Aegis::uint32 Checksum;
	Aegis::uint8 TotalSegments;
};

struct _ogg_Type
{
	std::array<char, 7> Typs;
};

#include <vorbis/vorbisfile.h>

void CAudioClip::Load_Ogg(const wchar_t* FileName)
{
	OggVorbis_File ovf;
	int error = ov_fopen(wstringTostring(FileName).c_str(), &ovf);
	ov_clear(&ovf);

	if (error != 0)
	{
		// Opus
		Aegis::OggOpus::Open(wstringTostring(FileName), &Info);
		//Aegis::OggOpus::Close(&Info);
	}
	else
	{
		// Vorbis
		Aegis::OggVorbis::Open(wstringTostring(FileName), &Info);
		//Aegis::OggVorbis::Close(&Info);
	}
}

//void CAudioClip::Load_Ogg(const wchar_t* FileName)
//{
//	WAVEFORMATEX wfx{};
//
//	{
//		ifstream file(FileName, std::ios_base::in | std::ios_base::binary);
//	
//		if (!file.is_open())
//		{
//			assert(false);
//		}
//	
//		std::string aa("vorbis");
//	
//		//_ogg_header header;
//		//file.read(reinterpret_cast<char*>(&header), sizeof(header));
//		int seek = sizeof(_ogg_header) - (sizeof(char) * 3);
//		file.seekg(seek, std::ios_base::beg);
//		_ogg_Type type;
//		file.read(reinterpret_cast<char*>(&type.Typs), sizeof(char) * type.Typs.size());
//	
//		//type.Typs[7] = (Aegis::int8)"\0";
//		std::string t((char*)&type.Typs);
//	
//		if (t == "vorbis")
//		{
//			seek = 0;
//		}
//	
//		for (int i = 0; i < 7; i++)
//		{
//			type.Typs[i] = 0;
//		}
//
//		file.close();
//	}
//
//	int ret = 0;
//	//ファイル開く
//	OggOpusFile* of = op_open_file(wstringTostring(FileName).c_str(), &ret);
//
//	if(nullptr != of)
//	{
//		//波形情報の取得
//		const OpusHead* pHead = op_head(of, -1);
//		if (!pHead) {
//			assert(false);
//		}
//		//PCMなので固定
//		wfx.wFormatTag = WAVE_FORMAT_PCM;
//		//チャンネル数(モノラル:1 ステレオ:2)
//		wfx.nChannels = pHead->channel_count;
//		//サンプリングレート(48000固定)
//		wfx.nSamplesPerSec = 48000;
//		//サンプル当たりのビット数(8bit:8 16bit:16)
//		wfx.wBitsPerSample = 16;
//		//ブロックサイズ(16bit ステレオなら 2*2=4)
//		wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
//		//データ速度(秒間当たりのデータ量 channel*sample*bit/8)
//		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
//		// 合計サイズの取得
//		ogg_int64_t decodedSize = op_pcm_total(of, -1) * wfx.nBlockAlign;
//		//必要なメモリ確保
//		char* pDecodedBuf = new char[decodedSize];
//		char* pBuf = pDecodedBuf;
//		int readSize;
//		while ((readSize = op_read(of, (opus_int16*)pBuf, 8096, NULL)) > 0)
//		{
//			pBuf += readSize * wfx.nBlockAlign;
//		}
//		op_free(of);
//
//		SoundData = new BYTE[decodedSize];
//		memcpy(SoundData, reinterpret_cast<BYTE*>(pDecodedBuf), decodedSize);
//
//		Length = decodedSize;
//		PlayLength = decodedSize / wfx.nBlockAlign;
//
//		delete[] pDecodedBuf;
//	}
//	else
//	{
//		OggVorbis_File ovf;
//		int error = ov_fopen(wstringTostring(FileName).c_str(), &ovf);
//		if (error != 0) {
//			switch (error) {
//			case OV_EREAD:       break;
//			case OV_ENOTVORBIS:  break;
//			case OV_EVERSION:    break;
//			case OV_EBADHEADER:  break;
//			case OV_EFAULT:      break;
//			default:             break;
//			}
//			assert(false);
//		}
//
//		// Oggファイルの音声フォーマット情報
//		vorbis_info* oggInfo = ov_info(&ovf, -1);
//
//		//PCMなので固定
//		wfx.wFormatTag = WAVE_FORMAT_PCM;
//		//チャンネル数
//		wfx.nChannels = oggInfo->channels;
//		//サンプリングレート
//		wfx.nSamplesPerSec = oggInfo->rate;
//		//サンプル当たりのビット数(8bit:8 16bit:16)
//		wfx.wBitsPerSample = 16;
//		//ブロックサイズ(16bit ステレオなら 2*2=4)
//		wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
//		//データ速度(秒間当たりのデータ量)
//		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
//		
//		// 合計サイズの取得
//		ogg_int64_t decodedSize = ov_pcm_total(&ovf, -1) * wfx.nBlockAlign;
//
//		char* pDecodedBuf = new char[decodedSize];
//		char* pBuf = pDecodedBuf;
//		//char* tmpBuffer = new char[decodedSize];
//		int bitstream = 0;
//		int readSize = 0;
//		int comSize = 0;
//		while ((readSize = ov_read(&ovf, pBuf, 4096, 0, 2, 1, &bitstream)) > 0)
//		{
//			pBuf += readSize;
//		}
//
//		ov_clear(&ovf);
//
//		SoundData = new BYTE[decodedSize];
//		memcpy(SoundData, reinterpret_cast<BYTE*>(pDecodedBuf), decodedSize);
//
//		Length = decodedSize;
//		PlayLength = decodedSize / wfx.nBlockAlign;
//
//		delete[] pDecodedBuf;
//	}
//
//	// サウンドソース生成
//	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
//	{
//		AUDIO_MANAGER::Get_Xaudio()->CreateSourceVoice(&SourceVoice[j], &wfx);
//	}
//}

//#define MINIMP3_IMPLEMENTATION
//#include "external\mp3\minimp3_ex.h"
//
//void CAudioClip::Load_mp3(const wchar_t* FileName)
//{
//	mp3dec_ex_t* dec = new mp3dec_ex_t;
//	if (mp3dec_ex_open(dec, wstringTostring(std::wstring(FileName)).c_str(), MP3D_SEEK_TO_SAMPLE))
//	{
//		assert(false);
//	}
//
//	const uint64_t pos = 0;
//
//	/* dec.samples, dec.info.hz, dec.info.layer, dec.info.channels should be filled */
//	//if (mp3dec_ex_seek(&dec, pos))
//	//{
//	//	assert(false);
//	//}
//
//	WAVEFORMATEX wfx{};
//
//	{
//		//PCMなので固定
//		wfx.wFormatTag = WAVE_FORMAT_PCM;
//		//チャンネル数(モノラル:1 ステレオ:2)
//		wfx.nChannels = dec->info.channels;
//		//サンプリングレート(48000固定)
//		wfx.nSamplesPerSec = dec->info.hz;
//		//サンプル当たりのビット数(8bit:8 16bit:16)
//		wfx.wBitsPerSample = 16;
//		//ブロックサイズ(16bit ステレオなら 2*2=4)
//		wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
//		//データ速度(秒間当たりのデータ量 channel*sample*bit/8)
//		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
//
//		// 合計サイズの取得
//		uint64_t decodedSize = dec->samples * sizeof(mp3d_sample_t);
//
//		mp3d_sample_t* buffer = new mp3d_sample_t[decodedSize];
//		size_t readed = mp3dec_ex_read(dec, buffer, dec->samples);
//		if (readed != dec->samples) /* normal eof or error condition */
//		{
//			if (dec->last_error)
//			{
//				assert(false);
//			}
//		}
//
//		SoundData = new BYTE[decodedSize];
//		memcpy(SoundData, reinterpret_cast<BYTE*>(buffer), decodedSize);
//
//		Length = decodedSize;
//		PlayLength = decodedSize / wfx.nBlockAlign;
//
//		delete[] buffer;
//	}
//
//	// サウンドソース生成
//	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
//	{
//		AUDIO_MANAGER::Get_Xaudio()->CreateSourceVoice(&SourceVoice[j], &wfx);
//	}
//
//	SAFE_DELETE(dec->index.frames);
//	SAFE_DELETE(dec);
//}