#pragma once

#ifndef GAME_H
#define GAME_H

#include	"Scene.h"

/**
* @brief ゲームシーンクラス
* @details シーンクラスの派生クラス
*/
class GAME : public SCENE {
private:

protected:

public:
	GAME() {}
	~GAME() {
		Uninit();
	}

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<SCENE>(this));
	}


	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<SCENE>(this));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<SCENE>(this));
	}*/
};

CEREAL_REGISTER_TYPE(GAME)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, GAME)

#endif // !GAME_H