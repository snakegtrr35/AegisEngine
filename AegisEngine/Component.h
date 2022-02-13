#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
//#include	"GameObject.h"
//#include	"manager.h"

// コンポーネントクラス
class COMPONENT /*: public GameObject*/ {
protected:
	std::weak_ptr<GameObject> Owner;

	bool Enable;
	bool Draw_Enable;
	bool DestroyFlag;

	aegis::Vector3 Position;
	aegis::Vector3 Rotation;
	aegis::Vector3 Scaling;

public:

	COMPONENT() : Enable(true), DestroyFlag(false), Position(aegis::Vector3(0.0f, 0.0f, 0.0f)), Rotation(aegis::Vector3(0.0f, 0.0f, 0.0f)), Scaling(aegis::Vector3(1.0f, 1.0f, 1.0f)) {
		Owner.reset();

	}

	virtual ~COMPONENT() {}

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Draw_DPP() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Uninit() = 0;

	virtual void Draw_Inspector() = 0;

	void Set_Owner(const std::weak_ptr<GameObject>& owner);

	void SetEnable(const bool flag);

	const bool GetEnable();

	void SetDestroy();

	void Set_Draw_Enable(const bool flag);

	bool Get_Draw_Enable();

	aegis::Vector3* const Get_Position();

	aegis::Vector3* const Get_Rotation();

	aegis::Vector3* const Get_Scaling();



	void Set_Position(aegis::Vector3* const  position);
	void Set_Position(const aegis::Vector3& position);

	void Set_Rotation(aegis::Vector3* const  rotation);
	void Set_Rotation(const aegis::Vector3& rotation);

	void Set_Scaling(aegis::Vector3* const  scaling);
	void Set_Scaling(const aegis::Vector3& scaling);

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
//CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, COMPONENT)

#endif // COMPONENT_H!