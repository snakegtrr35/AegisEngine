#pragma once

#ifndef  ANIMETION_STATE_MACHINE_H
#define ANIMETION_STATE_MACHINE_H

/**
* @brief アニメーションステートマシンクラス
* @details モデルのアニメーション管理用クラス
*/
class ANIMETION_STATE {
private:
	aegis::unordered_map<std::string, std::string> Anime_Name_Map;		//!< 全てのアニメーションの名前

	std::string AnimeName;									//!< 現在再生中のアニメーション
	std::string Next_Anime_Name;								//!< 遷移先のアニメーション

	float Ratio;										//!< 現在の二つのアニメーションの割合
	float Frame;										//!< 二つのアニメーションを遷移させる時間

	bool Enable;										//!< アニメーションの遷移中かのフラグ

public:

	ANIMETION_STATE() {
		Frame = Ratio = 0.f;
		Enable = false;
	}

	~ANIMETION_STATE() {};

	void Update() {

		if (Enable)
		{
			Ratio = std::min(1.0f, Ratio + (1.0f / Frame));

			if (1.0f == Ratio)
			{
				AnimeName = Next_Anime_Name;
				Enable = false;
			}
		}

	}

	void Add_Name(const std::string& name) {
		Anime_Name_Map[name] = name;
	}

	//const aegis::unordered_map<std::string, std::string>& Get_Anime_Name_Map() {
	//	return Anime_Name_Map;
	//}

	const float Get_Ratio() {
		return Ratio;
	}

	const bool Get_Enable() {
		return Enable;
	}

	void Set_Anime_Name(const std::string& anime_name) {
		AnimeName = anime_name;
	}

	std::string& Get_Anime_Name() {
		return AnimeName;
	}

	std::string& Get_Next_Anime_Name() {
		return Next_Anime_Name;
	}

	void Change_Anime(const std::string& next_name, const float frame) {
		Next_Anime_Name = next_name;
		Frame = frame;
		Enable = true;
		Ratio = 0.0f;
	}
};

#endif // ! ANIMETION_STATE_MACHINE_H