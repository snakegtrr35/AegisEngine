#include "audio_clip.h"

#include "./fileLoader/WaveLoad.h"
#include "./fileLoader/Ogg_Vorbis.h"
#include "./fileLoader/Ogg_Opus.h"
#include "./fileLoader/Mp3.h"

#include <thread>

namespace aegis::audio
{
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

	WAVEFORMATEX AudioInfoToWAVEFORMATEX(const AudioInfo& info)
	{
		WAVEFORMATEX wfx{};

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
		WAVEFORMATEX wfx{};

		if (std::wstring::npos != std::wstring(FileName).find(L".wav"))
		{
			Wave::Open(wstringTostring(FileName), &Info);
			//aegis::Wave::Close(&Info);
		}
		else if (std::wstring::npos != std::wstring(FileName).find(L".ogg"))
		{
			Load_Ogg(FileName);
		}
		else if (std::wstring::npos != std::wstring(FileName).find(L".mp3"))
		{
			Mp3::Open(wstringTostring(FileName), &Info);
			//aegis::Mp3::Close(&Info);
		}

		wfx = AudioInfoToWAVEFORMATEX(Info);

		// サウンドソース生成
		for (int j = 0; j < SOUND_SOURCE_MAX; j++)
		{
			AudioManager::Instance()->Get_Xaudio()->CreateSourceVoice(&SourceVoice[j], &wfx/*, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &voiceCallback*/);
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
			case AudioType::Wav:
				Wave::Uninit(&Info, &AudioData);
				break;

			case AudioType::Ogg_Vorbis:
				OggVorbis::Uninit(&Info, &AudioData);
				break;

			case AudioType::Ogg_Opus:
				OggOpus::Uninit(&Info, &AudioData);
				break;

			case AudioType::Mp3:
				Mp3::Uninit(&Info, &AudioData);
				break;

			default:
				break;
		}
	}

	void CAudioClip::Play(bool Loop)
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
						case AudioType::Wav:
							Wave::Load(&Info, &AudioData);
							break;

						case AudioType::Ogg_Vorbis:
							OggVorbis::Load(&Info, &AudioData);
							break;

						case AudioType::Ogg_Opus:
							OggOpus::Load(&Info, &AudioData);
							break;

						case AudioType::Mp3:
							Mp3::Load(&Info, &AudioData);
							break;

						default:
							assert(false);
							break;
					}
				}

				// バッファ設定
				XAUDIO2_BUFFER bufinfo;

				memset(&bufinfo, 0x00, sizeof(bufinfo));
				//bufinfo.AudioBytes = Length;
				//bufinfo.pAudioData = SoundData;
				//bufinfo.PlayBegin = 0;
				//bufinfo.PlayLength = PlayLength;

				bufinfo.AudioBytes = AudioData.Length;
				bufinfo.pAudioData = AudioData.Data;
				bufinfo.PlayBegin = 0;
				bufinfo.PlayLength = AudioData.PlayLength;

				// ループ設定
				if (Loop)
				{
					bufinfo.LoopBegin = 0;
					//bufinfo.LoopLength = PlayLength;
					bufinfo.LoopLength = AudioData.PlayLength;
					bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
				}

				SourceVoice[j]->SubmitSourceBuffer(&bufinfo, NULL);

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
						case AudioType::Wav:
							//Wave::Load(&Info, &AudioData);
							break;

						case AudioType::Ogg_Vorbis:
							// 1秒分のデータを確保
							Info.Stream[0] = std::make_unique<std::uint8_t[]>(static_cast<std::int64_t>(Info.AvgBytesPerSec * 1.0f));
							Info.Stream[1] = std::make_unique<std::uint8_t[]>(static_cast<std::int64_t>(Info.AvgBytesPerSec * 1.0f));

							OggVorbis::reset(&Info);

							// 1秒分のデータを読み込み
							OggVorbis::Stream(&Info, (char*)Info.Stream[0].get(), static_cast<std::int64_t>(Info.AvgBytesPerSec * 1.0f));
							OggVorbis::Stream(&Info, (char*)Info.Stream[1].get(), static_cast<std::int64_t>(Info.AvgBytesPerSec * 1.0f));

							break;

						case AudioType::Ogg_Opus:
							//OggOpus::Load(&Info, &AudioData);
							break;

						case AudioType::Mp3:
							//Mp3::Load(&Info, &AudioData);
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

	void CAudioClip::Polling(IXAudio2SourceVoice* SourceVoice, AudioInfo* info)
	{
		while (AudioManager::Instance()->IsEnable())
		{
			Sleep(1);

			if (!AudioManager::Instance()->IsEnable())
			{
				return;
			}

			XAUDIO2_VOICE_STATE state;
			SourceVoice->GetState(&state);

			if (state.BuffersQueued < 2)
			{
				OggVorbis::Stream(info, (char*)info->Stream[info->submitCount].get(), info->AvgBytesPerSec * 1.0f);

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
	void CAudioClip::Set_Name(const std::wstring& name)
#else
	void CAudioClip::Set_Name(const string& name)
#endif // !UNICODE
	{
		Name = name;
	}

#ifdef UNICODE
	std::wstring* const CAudioClip::Get_Name()
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



	AudioManager* AudioManager::Instance()
	{
		return getInstance();
	}

	void AudioManager::CreateInstance()
	{
		auto& instance = getInstance();
		instance = new AudioManager();
	}

	void AudioManager::DestroyInstance()
	{
		auto& instance = getInstance();
		delete instance;
		instance = nullptr;
	}

	AudioManager*& AudioManager::getInstance()
	{
		static AudioManager* instance = nullptr;
		return instance;
	}

	void AudioManager::Init(void)
	{
		// XAudio生成
		XAudio2Create(&Xaudio, 0);

		// マスタリングボイス生成
		Xaudio->CreateMasteringVoice(&MasteringVoice);

		AudioManager::Load();
	}

	void AudioManager::Uninit(void)
	{
		AudioManager::Enable = false;

		for (int i = 0; i < SOUND_FILE_COUNT; i++)
		{
			if (nullptr != Sound_Dates[g_SoundFiles[i].Name]) Sound_Dates[g_SoundFiles[i].Name]->Unload();

			//SAFE_DELETE(Sound_Dates[g_SoundFiles[i].Name]);
			if (Sound_Dates[g_SoundFiles[i].Name])
			{
				delete Sound_Dates[g_SoundFiles[i].Name];
				Sound_Dates[g_SoundFiles[i].Name] = nullptr;
			}
		}
		Sound_Dates.clear();

		MasteringVoice->DestroyVoice();

		//SAFE_RELEASE(Xaudio);
		//Xaudio->Release();

		CoUninitialize();
	}

	// マップへの追加
	void AudioManager::Add_Sound_Object(SOUND_INDEX index) {

		CAudioClip* sound = new CAudioClip();

		sound->Set_Name(g_SoundFiles[(int)index].Name);
		sound->Set_Tag(g_SoundFiles[(int)index].Tag);
		sound->Load(g_SoundFiles[(int)index].Name.c_str());

		Sound_Dates.insert(make_pair(g_SoundFiles[(int)index].Name, sound));
	};

	// マップから再生
	void AudioManager::Play_Sound_Object(SOUND_INDEX index, bool flag) {
		Sound_Dates[g_SoundFiles[(int)index].Name]->Play(flag);
	};

	// マップから再生
	void AudioManager::Play_Sound(SOUND_INDEX index) {
		Sound_Dates[g_SoundFiles[(int)index].Name]->PlayLoop();
	};

	// マップからストップ
	void AudioManager::Stop_Sound_Object(SOUND_INDEX index) {

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

	IXAudio2* const AudioManager::Get_Xaudio()
	{
		return Xaudio.Get();
	}

	void AudioManager::Load()
	{
		for (int i = 0; i < SOUND_FILE_COUNT; i++)
		{
			Add_Sound_Object((SOUND_INDEX)i);
		}
	}

#include <ogg\vorbis\include\vorbis\vorbisfile.h>

	void CAudioClip::Load_Ogg(const wchar_t* FileName)
	{
		OggVorbis_File ovf;
		int error = ov_fopen(wstringTostring(FileName).c_str(), &ovf);
		ov_clear(&ovf);

		if (error != 0)
		{
			// Opus
			OggOpus::Open(wstringTostring(FileName), &Info);
			//OggOpus::Close(&Info);
		}
		else
		{
			// Vorbis
			OggVorbis::Open(wstringTostring(FileName), &Info);
			//OggVorbis::Close(&Info);
		}
	}
}
