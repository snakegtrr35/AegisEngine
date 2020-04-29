#pragma once

#ifndef PLAYER_H
#define PLAYER_H

class GAME_OBJECT;

class CMODEL;
class FBXmodel;

class BULLET;


class PLAYER : public GAME_OBJECT {
private:
	CMODEL* Model = nullptr;
	//FBXmodel* Model = nullptr;

	float HP;

public:
	PLAYER();
	~PLAYER();

	void Init(void) override;
	void Draw(void) override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit(void) override;

	void SetPosition(const XMFLOAT3 position);					// ポジションの設定
	void SetScaling(const XMFLOAT3 scaling);					// 拡大縮小の値の設定


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
		ar(cereal::base_class<GAME_OBJECT>(this));
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}*/
};

CEREAL_REGISTER_TYPE(PLAYER)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, PLAYER)

#endif // !PLAYER_H