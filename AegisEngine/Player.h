#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include	"Game_Object.h"

//class CMODEL;
class FBXmodel;

class BULLET;
class COLLISION;
class BOUNDING_SHPERE;

class PLAYER : public GAME_OBJECT {
private:
	//CMODEL* Model = nullptr;
	FBXmodel* Model = nullptr;
	//COLLISION* Collision;
	//BOUNDING_SHPERE* Shpere;
	//BOUNDING_AABB* Shpere;

	// true = Stop false = Walk
	bool AnimType;
	float Blend;

public:
	PLAYER();
	~PLAYER();

	void Init(void);
	void Draw(void);
	void Draw_DPP();
	void Update(float delta_time);
	void Uninit(void);

	void SetPosition(const XMFLOAT3 position);					// ポジションの設定
	void SetScaling(const XMFLOAT3 scaling);					// 拡大縮小の値の設定

	//COLLISION* const Get_Collision() {
	//	return Collision;
	//}

	float* Get() {
		return &Blend;
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