#include "main.h"
#include "audio_clip.h"

#ifdef UNICODE
map<wstring, CAudioClip*> AUDIO_MANAGER::Sound_Dates;
#else
map<string, CAudioClip*> AUDIO_MANAGER::Sound_Dates;
#endif // !UNICODE

IXAudio2*					AUDIO_MANAGER::Xaudio;
IXAudio2MasteringVoice*		AUDIO_MANAGER::MasteringVoice;

#ifdef UNICODE
void CAudioClip::Load(const wchar_t* FileName)
#else
void CAudioClip::Load(const char* FileName)
#endif // !UNICODE
{
	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;

#ifdef UNICODE
		hmmio = mmioOpen((LPWSTR)FileName, &mmioinfo, MMIO_READ);
#else
		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
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
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)SoundData, buflen);


		Length = readlen;
		PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}

	// サウンドソース生成
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		AUDIO_MANAGER::Get_Xaudio()->CreateSourceVoice(&SourceVoice[j], &wfx);
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

	if (SoundData)
	{
		delete[] SoundData;
		SoundData = nullptr;
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

			// バッファ設定
			XAUDIO2_BUFFER bufinfo;

			memset(&bufinfo,0x00,sizeof(bufinfo));
			bufinfo.AudioBytes = Length;
			bufinfo.pAudioData = SoundData;
			bufinfo.PlayBegin = 0;
			bufinfo.PlayLength = PlayLength;

			// ループ設定
			if( Loop )
			{
				bufinfo.LoopBegin = 0;
				bufinfo.LoopLength = PlayLength;
				bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
			}

			SourceVoice[j]->SubmitSourceBuffer( &bufinfo, NULL );

			// 再生
			SourceVoice[j]->Start();

			break;
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
	// COM初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio生成
	XAudio2Create(&Xaudio, 0);

	// マスタリングボイス生成
	Xaudio->CreateMasteringVoice(&MasteringVoice);

	AUDIO_MANAGER::Load();
}

void AUDIO_MANAGER::Uninit(void)
{
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

// マップからストップ
void AUDIO_MANAGER::Stop_Sound_Object(SOUND_INDEX index) {

	if (SOUND_INDEX::SOUND_INDEX_MAX == index)
	{
		for (int i = 0; i < SOUND_FILE_COUNT; i++)
		{
			Sound_Dates[g_SoundFiles[i].Name]->Stop();
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