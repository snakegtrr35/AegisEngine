#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
//#include	"GameObject.h"
//#include	"manager.h"

#include "include\engine\core\AegisObject.h"

// コンポーネントクラス
class COMPONENT : public aegis::AegisObject {

	ABSTRACT_OBJECT_TYPE_INFO(COMPONENT)

protected:
	GameObject* Owner;

	bool Enable;
	bool DestroyFlag;

public:

	COMPONENT() : Enable(false), DestroyFlag(false), Owner(nullptr)/*, Position(aegis::Vector3(0.0f, 0.0f, 0.0f)), Rotation(aegis::Vector3(0.0f, 0.0f, 0.0f)), Scaling(aegis::Vector3(1.0f, 1.0f, 1.0f))*/
	{
	}

	virtual ~COMPONENT() {}

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Draw_DPP() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Uninit() = 0;

	virtual void Draw_Inspector() = 0;

	//void Set_Owner(const std::weak_ptr<GameObject>& owner);
	void Set_Owner(GameObject* owner);

	void SetEnable(const bool flag);

	const bool GetEnable();

	void SetDestroy();

	/*aegis::Vector3* const Get_Position();

	aegis::Vector3* const Get_Rotation();

	aegis::Vector3* const Get_Scaling();



	void Set_Position(aegis::Vector3* const  position);
	void Set_Position(const aegis::Vector3& position);

	void Set_Rotation(aegis::Vector3* const  rotation);
	void Set_Rotation(const aegis::Vector3& rotation);

	void Set_Scaling(aegis::Vector3* const  scaling);
	void Set_Scaling(const aegis::Vector3& scaling);*/

	// メモリ上からの削除
	bool Destroy();

	template<class Archive>
	void serialize(Archive& ar)
	{
		//ar(Owner);
		/*ar(Position);
		ar(Rotation);
		ar(Scaling);*/
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