#pragma once

#ifndef PLAYER_H
#define PLAYER_H

class GameObject;

class CMODEL;
class FBXmodel;

class BULLET;


class PLAYER : public GameObject {
private:
	unique_ptr<CMODEL> Model;
	//FBXmodel* Model = nullptr;

	float HP;

public:
	PLAYER();
	~PLAYER();

	void Init() override;
	void Draw() override;
	void Draw_Shadow() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void SetPosition(Aegis::Vector3& position);					// ポジションの設定
	void SetScaling(Aegis::Vector3& scaling);					// 拡大縮小の値の設定


	void Add_HP(float hp) {
		 HP += hp;
	}

	/*void Set_HP(const float hp) {
		HP = hp;
	}*/

	float Get_HP() {
		return HP;
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GameObject>(this));
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<GameObject>(this));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<GameObject>(this));
	}*/
};

CEREAL_REGISTER_TYPE(PLAYER)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, PLAYER)

#endif // !PLAYER_H