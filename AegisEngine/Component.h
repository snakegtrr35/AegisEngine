#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
//#include "GameObject.h"
//#include "manager.h"

#include "include\engine\core\AegisObject.h"

// コンポーネントクラス
class COMPONENT : public aegis::AegisObject {

	ABSTRACT_OBJECT_TYPE_INFO(aegis::AegisObject, COMPONENT)

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

	// メモリ上からの削除
	bool Destroy();

	void InitEnd() { IsInited = true; }
	bool IsInit() const { return IsInited; }

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Enable", Enable));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("Enable", Enable));
	}

protected:
	GameObject* Owner;

	bool Enable;
	bool DestroyFlag;

private:
	bool IsInited = false;
};

//CEREAL_REGISTER_TYPE(COMPONENT)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, COMPONENT)

#endif // COMPONENT_H!