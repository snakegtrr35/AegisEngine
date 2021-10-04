#include "main.h"
#include "audio_clip.h"

#define NOMINMAX
#include	<windows.h>

#include <opusfile.h>
#include <vorbis/vorbisfile.h>
// libopusとopusfileをリンク
#pragma comment(lib, "opus.lib")
#pragma comment(lib, "opusfile.lib")
// vorbisをリンク
#pragma comment ( lib, "vorbis_static.lib" )
#pragma comment ( lib, "vorbisfile_static.lib" )
// liboggをリンク
#pragma comment(lib, "libogg.lib")

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

	if(wstring::npos == wstring(FileName).find(L".ogg"))
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
	else
	{
		Load_Ogg(FileName);
		return;
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

	if (nullptr != SoundData)
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

void CAudioClip::Load_Ogg(const wchar_t* FileName)
{
	WAVEFORMATEX wfx{};

	int ret = 0;
	//ファイル開く
	OggOpusFile* of = op_open_file(wstringTostring(FileName).c_str(), &ret);

	if(nullptr != of)
	{
		//波形情報の取得
		const OpusHead* pHead = op_head(of, -1);
		if (!pHead) {
			assert(false);
		}
		//PCMなので固定
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		//チャンネル数(モノラル:1 ステレオ:2)
		wfx.nChannels = pHead->channel_count;
		//サンプリングレート(48000固定)
		wfx.nSamplesPerSec = 48000;
		//サンプル当たりのビット数(8bit:8 16bit:16)
		wfx.wBitsPerSample = 16;
		//ブロックサイズ(16bit ステレオなら 2*2=4)
		wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
		//データ速度(秒間当たりのデータ量 channel*sample*bit/8)
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
		// 合計サイズの取得
		ogg_int64_t decodedSize = op_pcm_total(of, -1) * wfx.nBlockAlign;
		//必要なメモリ確保
		char* pDecodedBuf = new char[decodedSize];
		char* pBuf = pDecodedBuf;
		int readSize;
		while ((readSize = op_read(of, (opus_int16*)pBuf, 8096, NULL)) > 0)
		{
			pBuf += readSize * wfx.nBlockAlign;
		}
		op_free(of);

		SoundData = new BYTE[decodedSize];
		memcpy(SoundData, reinterpret_cast<BYTE*>(pDecodedBuf), decodedSize);

		Length = decodedSize;
		PlayLength = decodedSize / wfx.nBlockAlign;

		delete[] pDecodedBuf;
	}
	else
	{
		OggVorbis_File ovf;
		int error = ov_fopen(wstringTostring(FileName).c_str(), &ovf);
		if (error != 0) {
			switch (error) {
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
		vorbis_info* oggInfo = ov_info(&ovf, -1);

		//PCMなので固定
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		//チャンネル数
		wfx.nChannels = oggInfo->channels;
		//サンプリングレート
		wfx.nSamplesPerSec = oggInfo->rate;
		//サンプル当たりのビット数(8bit:8 16bit:16)
		wfx.wBitsPerSample = 16;
		//ブロックサイズ(16bit ステレオなら 2*2=4)
		wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
		//データ速度(秒間当たりのデータ量)
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
		
		// 合計サイズの取得
		ogg_int64_t decodedSize = ov_pcm_total(&ovf, -1) * wfx.nBlockAlign;

		char* pDecodedBuf = new char[decodedSize];
		char* pBuf = pDecodedBuf;
		//char* tmpBuffer = new char[decodedSize];
		int bitstream = 0;
		int readSize = 0;
		int comSize = 0;
		while ((readSize = ov_read(&ovf, pBuf, 4096, 0, 2, 1, &bitstream)) > 0)
		{
			pBuf += readSize;
		}

		ov_clear(&ovf);

		SoundData = new BYTE[decodedSize];
		memcpy(SoundData, reinterpret_cast<BYTE*>(pDecodedBuf), decodedSize);

		Length = decodedSize;
		PlayLength = decodedSize / wfx.nBlockAlign;

		delete[] pDecodedBuf;
	}

	// サウンドソース生成
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		AUDIO_MANAGER::Get_Xaudio()->CreateSourceVoice(&SourceVoice[j], &wfx);
	}
}