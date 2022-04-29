#pragma once

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

class SCENE;

/**
* @brief メインメニューシーンクラス
* @details シーンクラスの派生クラス
* @details タイトルとゲームの間のシーン
*/
class MAIN_MENU : public SCENE {
private:

protected:

public:
	MAIN_MENU() {}
	~MAIN_MENU() {
		Uninit();
	}

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	static void Load(SCENE* scene);

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("SCENE", cereal::base_class<SCENE>(this)));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("SCENE", cereal::base_class<SCENE>(this)));
	}
};

CEREAL_REGISTER_TYPE(MAIN_MENU)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, MAIN_MENU)

#endif // !MAIN_MENU_H