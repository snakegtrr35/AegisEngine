#pragma once

#ifndef RESULT_H
#define RESULT_H

#include	"Scene.h"

/**
* @brief リザルトシーンクラス
* @details シーンクラスの派生クラス
*/
class RESULT : public SCENE {
private:

protected:

public:
	RESULT() {}
	~RESULT() {
		Uninit();
	}

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	static bool Clear_Flag;

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

CEREAL_REGISTER_TYPE(RESULT)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, RESULT)

#endif // !RESULT_H