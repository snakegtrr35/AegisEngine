#pragma once

#ifndef  ANIMETION_STATE_MACHINE_H
#define ANIMETION_STATE_MACHINE_H

/**
* @brief �A�j���[�V�����X�e�[�g�}�V���N���X
* @details ���f���̃A�j���[�V�����Ǘ��p�N���X
*/
class ANIMETION_STATE {
private:
	unordered_map<string, string> Anime_Name_Map;		//!< �S�ẴA�j���[�V�����̖��O

	string AnimeName;									//!< ���ݍĐ����̃A�j���[�V����
	string Next_Anime_Name;								//!< �J�ڐ�̃A�j���[�V����

	float Ratio;										//!< ���݂̓�̃A�j���[�V�����̊���
	float Frame;										//!< ��̃A�j���[�V������J�ڂ����鎞��

	bool Enable;										//!< �A�j���[�V�����̑J�ڒ����̃t���O

public:

	ANIMETION_STATE() {
		Frame = Ratio = 0.f;
		Enable = false;
	}

	~ANIMETION_STATE() {};

	void Update() {

		if (Enable)
		{
			Ratio = min(1.0f, Ratio + (1.0f / Frame));

			if (1.0f == Ratio)
			{
				AnimeName = Next_Anime_Name;
				Enable = false;
			}
		}

	}

	void Add_Name(const string& name) {
		Anime_Name_Map[name] = name;
	}

	//const unordered_map<string, string>& Get_Anime_Name_Map() {
	//	return Anime_Name_Map;
	//}

	const float Get_Ratio() {
		return Ratio;
	}

	const bool Get_Enable() {
		return Enable;
	}

	void Set_Anime_Name(const string& anime_name) {
		AnimeName = anime_name;
	}

	string& Get_Anime_Name() {
		return AnimeName;
	}

	string& Get_Next_Anime_Name() {
		return Next_Anime_Name;
	}

	void Change_Anime(const string& next_name, const float frame) {
		Next_Anime_Name = next_name;
		Frame = frame;
		Enable = true;
		Ratio = 0.0f;
	}
};

#endif // ! ANIMETION_STATE_MACHINE_H