#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

#include	"GameObject.h"

unordered_set<string> GameObject::Object_Name_Map;

GameObject::GameObject() : Object_Name("none"), DestroyFlag(false), Uuid(Aegis::Uuid::GetUuid())
{
	Component.reset(new COMPONENT_MANEGER());
}

GameObject::~GameObject()
{
	Object_Name_Map.erase(Object_Name);

	Component.reset();
};

void GameObject::Init()
{
	Component->Init();
}

void GameObject::Draw()
{
	Component->Draw();
}

void GameObject::Draw_Shadow() {}

void GameObject::Update(float delta_time)
{
	Component->Update(delta_time);
}

void GameObject::Set_Destroy()
{
	DestroyFlag = true;
};

bool GameObject::Destroy()
{
	return DestroyFlag;
};

void GameObject::Set_Object_Name(const string& name)
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