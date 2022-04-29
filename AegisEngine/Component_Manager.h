#pragma once

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include	"Component.h"
#include "include\engine\core\uuid\uuid.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

//#include	"Bounding_Aabb.h"
//#include	"Bounding_Obb.h"
//#include	"Bounding_Shpere.h"
class BOUNDING_AABB;
class BOUNDING_OBB;
class BOUNDING_SHPERE;

class GameObject;

class IComponentArray
{
public:
	IComponentArray() = default;
	virtual ~IComponentArray() {}
	virtual void EntityDestroyed() {}

	virtual void Destory() {}

	virtual void Init() {}
	virtual void Draw() {}
	virtual void Draw_DPP() {}
	virtual void Update(float delta_time) {}
	virtual void Uninit() {}
};

template<typename T>
class ComponentArray : public IComponentArray
{
	static_assert(std::is_base_of<COMPONENT, T>::value && std::is_convertible<T*, COMPONENT*>::value);

	using EntityID = aegis::uuid;

public:

	ComponentArray() = default;
	virtual ~ComponentArray() {}

	const std::shared_ptr<T>& Insert(EntityID entity, const std::shared_ptr<T>& component)
	{
		mComponentArray.emplace_back(component);

		aegis::uint64 newIndex = mComponentArray.size() - 1; // 0番目が1のため

		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;

		mSize = mComponentArray.size();

		return component;
	}

	const std::shared_ptr<T>& Insert(EntityID entity)
	{
		std::shared_ptr<T> component = std::make_shared<T>();

		mComponentArray.emplace_back(component);

		aegis::uint64 newIndex = mComponentArray.size() - 1; // 0番目が1のため

		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;

		mSize = mComponentArray.size();

		return component;
	}


	void Remove(EntityID entity)
	{
		aegis::uint64 indexOfRemovedEntity = mEntityToIndexMap[entity];
		aegis::uint64 indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity]->SetDestroy();

		EntityID entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);
	}

	T* GetData(EntityID entity)
	{
		return mComponentArray[mEntityToIndexMap[entity]].get();
	}

	void Destory() override
	{
		mComponentArray.erase(
			std::remove_if(
				mComponentArray.begin(), mComponentArray.end(),
				[](auto& Component) { return Component->Destroy(); }
			),
			mComponentArray.end()
		);
	}

	void Update(float delta_time) override
	{
		for (const auto& component : mComponentArray)
		{
			if(component->GetEnable())
				component->Update(delta_time);
		}
	}

	void Draw() override
	{
		for (const auto& component : mComponentArray)
		{
			if (component->GetEnable())
				component->Draw();
		}
	}

	void Draw_DPP() override
	{
		for (const auto& component : mComponentArray)
		{
			if (component->GetEnable())
				component->Draw_DPP();
		}
	}

	void Uninit() override
	{
		for (const auto& component : mComponentArray)
		{
			component->Uninit();
		}
		mComponentArray.clear();
	}

private:
	std::vector<std::shared_ptr<T>> mComponentArray;
	std::unordered_map<EntityID, aegis::uint64> mEntityToIndexMap;
	std::unordered_map<aegis::uint64, EntityID> mIndexToEntityMap;
	aegis::uint64 mSize;
};


class COMPONENT_MANEGER {
	using ComponentID = aegis::uint64;

public:

	static COMPONENT_MANEGER* getInstance();

	~COMPONENT_MANEGER() {
		Uninit();
	}

	template<typename T>
	auto AddComponent(aegis::uuid entity)
	{
		return GetComponentArray<T>()->Insert(entity);
	}

	template<typename T>
	auto AddComponent(aegis::uuid entity, const std::shared_ptr<T>& component)
	{

		return GetComponentArray<T>(component->GetTypeId())->Insert(entity, component);
	}

	template<typename T>
	void RemoveComponent(aegis::uuid entity)
	{
		GetComponentArray<T>()->Remove(entity);
	}

	void DestoryComponent()
	{
		for (auto& ca : mComponentArrays)
		{
			ca.second->Destory();
		}
	}

	template<typename T>
	T* GetComponent(aegis::uuid entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void Update(float delta_time);
	void Draw();
	void Draw_DPP();
	void Uninit();

private:

	COMPONENT_MANEGER() = default;

	std::unordered_map<ComponentID, std::unique_ptr<IComponentArray>> mComponentArrays;

	template<typename T>
	ComponentArray<T>* GetComponentArray()
	{
		ComponentID componentID = T::getDTI()->GetTypeID();

		auto ite = mComponentArrays.find(componentID);

		if (ite == mComponentArrays.cend())
		{
			ite = mComponentArrays.emplace(componentID, std::make_unique<ComponentArray<T>>()).first;
		}

		return reinterpret_cast<ComponentArray<T>*>(ite->second.get());
	}

	template<typename T>
	ComponentArray<T>* GetComponentArray(ComponentID componentID)
	{
		auto ite = mComponentArrays.find(componentID);

		if (ite == mComponentArrays.cend())
		{
			ite = mComponentArrays.emplace(componentID, std::make_unique<ComponentArray<T>>()).first;
		}

		return reinterpret_cast<ComponentArray<T>*>(ite->second.get());
	}
};

#undef new

#endif // !COMPONENT_MANAGER_H
