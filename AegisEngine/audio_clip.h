#pragma once

#ifndef AUDIO_CLIP_H
#define AUDIO_CLIP_H

#include <xaudio2.h>

static const int SOUND_SOURCE_MAX = 64;
static const int SOUND_DATE_MAX = 128;

//========================================
// �T�E���h�Ǘ��ԍ�(�񋓌^)
//========================================
enum class SOUND_INDEX {
	SOUND_INDEX_BGM001 = 0,		// �^�C�g����BGM
	SOUND_INDEX_BGM002,			// ���C�����j���[��BGM
	SOUND_INDEX_BGM003,			// �Q�[������BGM
	SOUND_INDEX_CLEAR,			// �N���A��BGM
	SOUND_INDEX_GAMEOVER,		// �Q�[���I�[�o�[��BGM

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

// �ǂݍ��݃T�E���h��
static const int SOUND_FILE_COUNT = sizeof(g_SoundFiles) / sizeof(g_SoundFiles[0]);

// �ǂݍ��݃T�E���h���ƃT�E���h�Ǘ��ԍ��񋓐��ɍ����������ꍇ�R���p�C���G���[�Ƃ���
static_assert((int)SOUND_INDEX::SOUND_INDEX_MAX == SOUND_FILE_COUNT, "SOUND_INDEX_MAX != SOUND_FILE_COUNT");

/**
* @brief �T�E���h�N���X
* @details �ʂ̃T�E���h�̃N���X
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
* @brief �T�E���h�N���X�Ǘ��N���X
* @details �T�E���h�N���X���Ǘ����邽�߂̃N���X
*/
class AUDIO_MANAGER {
private:
	static map<wstring, CAudioClip*> Sound_Dates;

	static IXAudio2* Xaudio;
	static IXAudio2MasteringVoice* MasteringVoice;

public:

	/**
	* @brief �}�b�v�ւ̒ǉ�������֐�
	* @param index SOUND_INDEX(�T�E���h�Ǘ��ԍ�)
	* @details index�Ԗڂ̃T�E���h�t�@�C�����}�b�v�ɒǉ�����
	*/
	static void Add_Sound_Object(SOUND_INDEX index);

	/**
	* @brief �}�b�v�������̃T�E���h�̍Đ�����֐�
	* @param index SOUND_INDEX(�T�E���h�Ǘ��ԍ�)
	* @param flag ���[�v���邩���Ȃ����̃t���O
	* @details �}�b�v��index�Ԗڂ̃T�E���h�t�@�C�����Đ�����
	*/
	static void Play_Sound_Object(SOUND_INDEX index, bool flag = false);

	/**
	* @brief �}�b�v����T�E���h���X�g�b�v����֐�
	* @param index SOUND_INDEX(�T�E���h�Ǘ��ԍ�)�A�����������Ȃ��ꍇ�̓T�E���h�Ǘ��ԍ��̍ő�l�ɂȂ�
	* @details �}�b�v��index�Ԗڂ̃T�E���h�t�@�C�����X�g�b�v����A�����������Ȃ��ꍇ�͑S�ẴT�E���h���X�g�b�v����
	*/
	static void Stop_Sound_Object(SOUND_INDEX index = SOUND_INDEX::SOUND_INDEX_MAX);

	static void Init();
	static void Uninit();

	static void Load();

	static IXAudio2* const Get_Xaudio();

};

#endif // !AUDIO_CLIP_H