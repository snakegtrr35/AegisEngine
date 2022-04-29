#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

#include "GameObject.h"
#include "include\/engine\core\memory\aegisAllocator.h"

IMPLEMENT_ABSTRACT_OBJECT_TYPE_INFO(aegis::AegisObject, GameObject)

aegis::unordered_set<aegis::string> GameObject::Object_Name_Map;

GameObject::GameObject() : Object_Name("none"), DestroyFlag(false), Uuid(aegis::Uuid::GetUuid())
{
}

GameObject::~GameObject()
{
	Object_Name_Map.erase(Object_Name);
};

void GameObject::Init()
{
}

void GameObject::Draw()
{
}

void GameObject::Draw_Shadow() {}

void GameObject::Update(float delta_time)
{
}

void GameObject::Uninit()
{
	for (const auto& component : Components)
	{
		if(!component.expired())
			component.lock()->SetDestroy();
	}
	Components.clear();
}

void GameObject::Set_Destroy()
{
	DestroyFlag = true;
};

bool GameObject::Destroy()
{
	return DestroyFlag;
};

void GameObject::Set_Object_Name(const aegis::string& name)
{
#ifdef _DEBUG
	if (Object_Name_Map.emplace(name).second)
	{
		Object_Name = name;
	}
	else
	{
		//Erroer_Message("既に使われている名前です");
		int a = 0;
	}
#else
	Object_Name_Map.emplace(name);
	Object_Name = name;
#endif // _DEBUG
};

#undef new

void* GameObject::operator new(aegis::uint64 size)
{
	return aegis::memory::AegisAllocator::allocate(size, aegis::memory::AllocatorType::Temp);
}

void* GameObject::operator new(aegis::uint64 size, std::align_val_t alignment)
{
	return aegis::memory::AegisAllocator::allocate(size, alignment, aegis::memory::AllocatorType::Temp);
}

void* GameObject::operator new[](aegis::uint64 size)
{
	return aegis::memory::AegisAllocator::allocate(size, aegis::memory::AllocatorType::Temp);
}

void* GameObject::operator new[](aegis::uint64 size, std::align_val_t alignment)
{
	return aegis::memory::AegisAllocator::allocate(size, alignment, aegis::memory::AllocatorType::Temp);
}

void GameObject::operator delete(void* ptr, aegis::uint64 size) noexcept
{
	return aegis::memory::AegisAllocator::deallocate(ptr, size, aegis::memory::AllocatorType::Temp);
}

void GameObject::operator delete(void* ptr, std::align_val_t alignment) noexcept
{
	aegis::memory::AegisAllocator::deallocate(ptr, alignment, aegis::memory::AllocatorType::Temp);
}

void GameObject::operator delete(void* ptr, aegis::uint64 size, std::align_val_t alignment) noexcept
{
	aegis::memory::AegisAllocator::deallocate(ptr, size, aegis::memory::AllocatorType::Temp);
}

void GameObject::operator delete[](void* ptr, aegis::uint64 size) noexcept
{
	aegis::memory::AegisAllocator::deallocate(ptr, size, aegis::memory::AllocatorType::Temp);
}

void GameObject::operator delete[](void* ptr, std::align_val_t alignment) noexcept
{
	aegis::memory::AegisAllocator::deallocate(ptr, alignment, aegis::memory::AllocatorType::Temp);
}

void GameObject::operator delete[](void* ptr, aegis::uint64 size, std::align_val_t alignment) noexcept
{
	aegis::memory::AegisAllocator::deallocate(ptr, size, aegis::memory::AllocatorType::Temp);
}

namespace cereal
{
	/*void prologue(cereal::JSONInputArchive&, GameObject const& data)
	{
	}

	void epilogue(cereal::JSONInputArchive&, GameObject const& data)
	{
		GameObject& gameObject = const_cast<GameObject&>(data);

		auto components = gameObject.GetComponents();

		for (auto& component : *components)
		{
			COMPONENT_MANEGER::getInstance()->AddComponent(gameObject.GetId(), std::move(component.lock()));
		}
	}

	void prologue(cereal::JSONOutputArchive&, GameObject const& data)
	{
	}

	void epilogue(cereal::JSONOutputArchive&, GameObject const& data)
	{
	}



	void prologue(cereal::BinaryInputArchive&, GameObject const& data)
	{
	}

	void epilogue(cereal::BinaryInputArchive&, GameObject const& data)
	{
		GameObject& gameObject = const_cast<GameObject&>(data);

		auto components = gameObject.GetComponents();

		for (auto& component : *components)
		{
			COMPONENT_MANEGER::getInstance()->AddComponent(gameObject.GetId(), std::move(component.lock()));
		}
	}

	void prologue(cereal::BinaryOutputArchive&, GameObject const& data)
	{
	}

	void epilogue(cereal::BinaryOutputArchive&, GameObject const& data)
	{
	}*/
}
