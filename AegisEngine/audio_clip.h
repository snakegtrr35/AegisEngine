#pragma once

#ifndef AUDIO_CLIP_H
#define AUDIO_CLIP_H

#include <xaudio2.h>

static const int SOUND_SOURCE_MAX = 64;
static const int SOUND_DATE_MAX = 128;

//========================================
// サウンド管理番号(列挙型)
//========================================
enum class SOUND_INDEX {
	SOUND_INDEX_BGM001 = 0,		// タイトルのBGM
	SOUND_INDEX_BGM002,			// メインメニューのBGM
	SOUND_INDEX_BGM003,			// ゲーム中のBGM
	SOUND_INDEX_CLEAR,			// クリアのBGM
	SOUND_INDEX_GAMEOVER,		// ゲームオーバーのBGM

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

typedef struct {
	wstring Name;
	SOUND_TAG Tag;
}SOUND_FILE;

static const SOUND_FILE g_SoundFiles[] = {

	{ L"asset/sound/BGM_title.wav", SOUND_TAG::BGM },
	{ L"asset/sound/BGM_menu.wav", SOUND_TAG::BGM },
	{ L"asset/sound/BGM_battle.wav", SOUND_TAG::BGM },
	{ L"asset/sound/BGM_game_clear .wav", SOUND_TAG::BGM },
	{ L"asset/sound/BGM_game_over.wav", SOUND_TAG::BGM },

	{ L"asset/sound/SE_shot.wav", SOUND_TAG::SE },
	{ L"asset/sound/SE_explosion000.wav", SOUND_TAG::SE },
	{ L"asset/sound/SE_kettei.wav", SOUND_TAG::SE },
	{ L"asset/sound/SE_senntaku.wav", SOUND_TAG::SE },

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
	BYTE* SoundData;

	int	Length;
	int	PlayLength;

	wstring Name;
	SOUND_TAG Tag;

public:
	void Load(const wchar_t *FileName);
	void Unload();
	void Play(bool Loop=false);
	void Stop();

	void Set_Name(const wstring& name);
	wstring* const Get_Name();

	void Set_Tag(const SOUND_TAG tag);
	const SOUND_TAG Get_Tag();

};

/**
* @brief サウンドクラス管理クラス
* @details サウンドクラスを管理するためのクラス
*/
class AUDIO_MANAGER {
private:
	static map<wstring, CAudioClip*> Sound_Dates;

	static IXAudio2* Xaudio;
	static IXAudio2MasteringVoice* MasteringVoice;

public:

	/**
	* @brief マップへの追加をする関数
	* @param index SOUND_INDEX(サウンド管理番号)
	* @details index番目のサウンドファイルをマップに追加する
	*/
	static void Add_Sound_Object(SOUND_INDEX index);

	/**
	* @brief マップから特定のサウンドの再生する関数
	* @param index SOUND_INDEX(サウンド管理番号)
	* @param flag ループするかしないかのフラグ
	* @details マップのindex番目のサウンドファイルを再生する
	*/
	static void Play_Sound_Object(SOUND_INDEX index, bool flag = false);

	/**
	* @brief マップからサウンドをストップする関数
	* @param index SOUND_INDEX(サウンド管理番号)、引数を書かない場合はサウンド管理番号の最大値になる
	* @details マップのindex番目のサウンドファイルをストップする、引数を書かない場合は全てのサウンドをストップする
	*/
	static void Stop_Sound_Object(SOUND_INDEX index = SOUND_INDEX::SOUND_INDEX_MAX);

	static void Init();
	static void Uninit();

	static void Load();

	static IXAudio2* const Get_Xaudio();

};

#endif // !AUDIO_CLIP_H