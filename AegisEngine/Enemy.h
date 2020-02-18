#pragma once

#ifndef ENEMY_H
#define ENEMY_H

class CMODEL;
class COLLISION;
class BOUNDING_AABB;
class GAME_OBJECT;

typedef struct
{
	float Attack_Probability;	// 攻撃する確率
	float Lenght;				// プレイヤーとの距離
	float Move_Lenght;			// 移動量
	float RL_Probability;		// 左右移動を切り替える確率
	bool RL;
}PROBABILITY_DATE;

class ENEMY : public GAME_OBJECT {
private:
	CMODEL* Model;

	PROBABILITY_DATE Date;

	bool Fire_Flag;

public:
	ENEMY();
	~ENEMY();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void SetPosition(const XMFLOAT3& position);					// ポジションの設定
	void SetRotation(const XMFLOAT3& position);					// 回転の設定
	void SetScaling(const XMFLOAT3& scaling);					// 拡大縮小の値の設定

	void Set_Date(const PROBABILITY_DATE& date) {
		Date = date;
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}
};

CEREAL_REGISTER_TYPE(ENEMY)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, ENEMY)

#endif // !ENEMY_H