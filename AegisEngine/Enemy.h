﻿#pragma once

#ifndef ENEMY_H
#define ENEMY_H

class GAME_OBJECT;
class CMODEL;

class ENEMY : public GAME_OBJECT {
private:
	unique_ptr<CMODEL> Model;

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

	void SetPosition(Aegis::Vector3& position);					// ポジションの設定
	void SetRotation(Aegis::Vector3& position);					// 回転の設定
	void SetScaling(Aegis::Vector3& scaling);					// 拡大縮小の値の設定

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}
};

CEREAL_REGISTER_TYPE(ENEMY)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, ENEMY)

#endif // !ENEMY_H