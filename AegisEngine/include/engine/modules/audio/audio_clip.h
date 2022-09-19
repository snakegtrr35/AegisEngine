#pragma once

#ifndef AUDIO_CLIP_H
#define AUDIO_CLIP_H

#include <xaudio2.h>
#include "AudioDefine.h"

#include <wrl/client.h>
using namespace Microsoft::WRL;

static const int SOUND_SOURCE_MAX = 64;
static const int SOUND_DATE_MAX = 128;

namespace aegis::audio
{
	//========================================
	// サウンド管理番号(列挙型)
	//========================================
	enum class SOUND_INDEX {
		SOUND_INDEX_BGM001 = 0,		// タイトルのBGM
		SOUND_INDEX_BGM002,			// メインメニューのBGM
		SOUND_INDEX_BGM003,			// ゲーム中のBGM
		SOUND_INDEX_CLEAR,			// クリアのBGM
		SOUND_INDEX_GAMEOVER,		// ゲームオーバーのBGM

		SOUND_INDEX_82,
		SOUND_INDEX_82_MP3,

		SOUND_INDEX_SHOT,
		SOUND_INDEX_EXPLOSION,
		SOUND_INDEX_KETTEI,
		SOUND_INDEX_SENTAKU,

		SOUND_INDEX_MAX
	};

	enum class SOUND_TAG {
		BGM = 0,
		SE,
		MAX_SOUND
	};

#ifdef UNICODE
	typedef struct {
		std::wstring Name;
		SOUND_TAG Tag;
	}SOUND_FILE;
#else
	typedef struct {
		string Name;
		SOUND_TAG Tag;
	}SOUND_FILE;
#endif // !UNICODE

	static const SOUND_FILE g_SoundFiles[] = {
	#ifdef UNICODE
		{ L"asset/sound/BGM_title.wav", SOUND_TAG::BGM },
		{ L"asset/sound/BGM_menu.wav", SOUND_TAG::BGM },
		{ L"asset/sound/BGM_battle.wav", SOUND_TAG::BGM },
		{ L"asset/sound/BGM_game_clear .wav", SOUND_TAG::BGM },
		{ L"asset/sound/BGM_game_over.wav", SOUND_TAG::BGM },

		//{ L"asset/Sound/82_vorbis.ogg", SOUND_TAG::BGM },
		{ L"asset/Sound/82_opus.ogg", SOUND_TAG::BGM },
		{ L"asset/sound/82.mp3", SOUND_TAG::BGM },

		{ L"asset/sound/SE_shot.wav", SOUND_TAG::SE },
		{ L"asset/sound/SE_explosion000.wav", SOUND_TAG::SE },
		{ L"asset/sound/SE_kettei.wav", SOUND_TAG::SE },
		{ L"asset/sound/SE_senntaku.wav", SOUND_TAG::SE },
	#else
		{ "asset/sound/BGM_title.wav", SOUND_TAG::BGM },
		{ "asset/sound/BGM_menu.wav", SOUND_TAG::BGM },
		{ "asset/sound/BGM_battle.wav", SOUND_TAG::BGM },
		{ "asset/sound/BGM_game_clear .wav", SOUND_TAG::BGM },
		{ "asset/sound/BGM_game_over.wav", SOUND_TAG::BGM },

		{ "asset/sound/SE_shot.wav", SOUND_TAG::SE },
		{ "asset/sound/SE_explosion000.wav", SOUND_TAG::SE },
		{ "asset/sound/SE_kettei.wav", SOUND_TAG::SE },
		{ "asset/sound/SE_senntaku.wav", SOUND_TAG::SE },
	#endif // !UNICODE
	};

	// 読み込みサウンド数
	static const int SOUND_FILE_COUNT = sizeof(g_SoundFiles) / sizeof(g_SoundFiles[0]);

	// 読み込みサウンド数とサウンド管理番号列挙数に差があった場合コンパイルエラーとする
	static_assert((int)SOUND_INDEX::SOUND_INDEX_MAX == SOUND_FILE_COUNT, "SOUND_INDEX_MAX != SOUND_FILE_COUNT");

	/**
	* @brief サウンドクラス
	* @details 個別のサウンドのクラス
	*/
	class CAudioClip {
	private:
		IXAudio2SourceVoice* SourceVoice[SOUND_SOURCE_MAX];
		//BYTE* SoundData;
		//
		//int	Length;
		//int	PlayLength;

		AudioInfo Info;
		PlayData AudioData;

#ifdef UNICODE
		std::wstring Name;
#else
		std::string Name;
#endif // !UNICODE

		SOUND_TAG Tag;

	public:

#ifdef UNICODE
		void Load(const wchar_t* FileName);

		void Set_Name(const std::wstring& name);

		void Load_Ogg(const wchar_t* FileName);

		//void Load_mp3(const wchar_t* FileName);

		std::wstring* const __cdecl Get_Name();
#else
		void Load(const char* FileName);

		void Set_Name(const std::string& name);

		string* const Get_Name();
#endif // !UNICODE

		void Unload();
		void Play(bool Loop = false);
		void PlayLoop();
		void Stop();

		void Set_Tag(const SOUND_TAG tag);
		const SOUND_TAG Get_Tag();

		static void Polling(IXAudio2SourceVoice* SourceVoice, AudioInfo* info);
	};

	/**
	* @brief サウンドクラス管理クラス
	* @details サウンドクラスを管理するためのクラス
	*/
	class AudioManager {
	public:

		static AudioManager* Instance();

		static void CreateInstance();
		static void DestroyInstance();

		/**
		* @brief マップへの追加をする関数
		* @param index SOUND_INDEX(サウンド管理番号)
		* @details index番目のサウンドファイルをマップに追加する
		*/
		virtual void Add_Sound_Object(SOUND_INDEX index);

		/**
		* @brief マップから特定のサウンドの再生する関数
		* @param index SOUND_INDEX(サウンド管理番号)
		* @param flag ループするかしないかのフラグ
		* @details マップのindex番目のサウンドファイルを再生する
		*/
		virtual void Play_Sound_Object(SOUND_INDEX index, bool flag = false);

		virtual void Play_Sound(SOUND_INDEX index);

		/**
		* @brief マップからサウンドをストップする関数
		* @param index SOUND_INDEX(サウンド管理番号)、引数を書かない場合はサウンド管理番号の最大値になる
		* @details マップのindex番目のサウンドファイルをストップする、引数を書かない場合は全てのサウンドをストップする
		*/
		virtual void Stop_Sound_Object(SOUND_INDEX index = SOUND_INDEX::SOUND_INDEX_MAX);

		virtual void Init();
		virtual void Uninit();

		virtual void Load();

		virtual IXAudio2* const Get_Xaudio();

		virtual bool IsEnable() const { return Enable; }

	private:

#ifdef UNICODE
		//static aegis::unordered_map<std::wstring, CAudioClip*> Sound_Dates;
		std::unordered_map<std::wstring, CAudioClip*> Sound_Dates;
#else
		//static aegis::unordered_map<std::string, CAudioClip*> Sound_Dates;
		std::unordered_map<std::string, CAudioClip*> Sound_Dates;
#endif // !UNICODE

		//static IXAudio2* Xaudio;
		ComPtr<IXAudio2> Xaudio;
		IXAudio2MasteringVoice* MasteringVoice;
		bool Enable;

		static AudioManager*& getInstance();
	};

	VC_DLL_EXPORTS AudioManager* AudioInstance();
	VC_DLL_EXPORTS void CreateInstance();
	VC_DLL_EXPORTS void DestroyInstance();
}

#endif // !AUDIO_CLIP_H