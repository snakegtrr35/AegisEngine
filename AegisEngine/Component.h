#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GAME_OBJECT;
//#include	"GameObject.h"
//#include	"manager.h"

// コンポーネントクラス
class COMPONENT /*: public GAME_OBJECT*/ {
protected:
	weak_ptr<GAME_OBJECT> Owner;

	bool Enable;
	bool Draw_Enable;
	bool DestroyFlag;

	Aegis::Vector3 Position;
	Aegis::Vector3 Rotation;
	Aegis::Vector3 Scaling;

public:

	COMPONENT() : Enable(true), DestroyFlag(false), Position(Aegis::Vector3(0.0f, 0.0f, 0.0f)), Rotation(Aegis::Vector3(0.0f, 0.0f, 0.0f)), Scaling(Aegis::Vector3(1.0f, 1.0f, 1.0f)) {
		Owner.reset();

	}

	virtual ~COMPONENT() {}

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Draw_DPP() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Uninit() = 0;

	virtual void Draw_Inspector() = 0;

	void Set_Owner(const weak_ptr<GAME_OBJECT>& owner);

	void SetEnable(const bool flag);

	const bool GetEnable();

	void SetDestroy();

	void Set_Draw_Enable(const bool flag);

	bool Get_Draw_Enable();

	Aegis::Vector3* const Get_Position();

	Aegis::Vector3* const Get_Rotation();

	Aegis::Vector3* const Get_Scaling();



	void Set_Position(Aegis::Vector3* const  position);
	void Set_Position(const Aegis::Vector3& position);

	void Set_Rotation(Aegis::Vector3* const  rotation);
	void Set_Rotation(const Aegis::Vector3& rotation);

	void Set_Scaling(Aegis::Vector3* const  scaling);
	void Set_Scaling(const Aegis::Vector3& scaling);

	// メモリ上からの削除
	bool Destroy();

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(Owner);
		ar(Position);
		ar(Rotation);
		ar(Scaling);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(Owner);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(Owner);
	}*/
};

//CEREAL_REGISTER_TYPE(COMPONENT)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, COMPONENT)

#endif // COMPONENT_H!