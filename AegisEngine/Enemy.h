#pragma once

#ifndef ENEMY_H
#define ENEMY_H

class GameObject;

class ENEMY : public GameObject {

	OBJECT_TYPE_INFO(GameObject, ENEMY)

private:

	bool Fire_Flag;
	float Time;

public:
	ENEMY();
	~ENEMY();

	void Init() override;
	void Draw() override;
	void Draw_Shadow() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void SetPosition(aegis::Vector3& position);					// ポジションの設定
	void SetRotation(aegis::Vector3& position);					// 回転の設定
	void SetScaling(aegis::Vector3& scaling);					// 拡大縮小の値の設定

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
	}
};

CEREAL_REGISTER_TYPE(ENEMY)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, ENEMY)

#endif // !ENEMY_H