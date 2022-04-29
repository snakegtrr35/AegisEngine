#pragma once

#ifndef RESULT_H
#define RESULT_H

class SCENE;

/**
* @brief リザルトシーンクラス
* @details シーンクラスの派生クラス
*/
class RESULT : public SCENE {
private:

protected:

public:
	static bool Clear_Flag;

	RESULT() {}
	~RESULT() {
		Uninit();
	}

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	static void Load(SCENE* scene);

	static void Set(const bool flag) { Clear_Flag = flag; }

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

CEREAL_REGISTER_TYPE(RESULT)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SCENE, RESULT)

#endif // !RESULT_H