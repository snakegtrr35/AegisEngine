﻿#pragma once

#ifndef TITLE_H
#define TITLE_H

class SCENE;

/**
* @brief タイトルシーンクラス
* @details シーンクラスの派生クラス
*/
class TITLE : public SCENE {
private:

protected:

public:
	TITLE() {}
	~TITLE() { Uninit(); }

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

CEREAL_REGISTER_TYPE(TITLE)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, TITLE)

#endif TITLE_H //!TITLE_H