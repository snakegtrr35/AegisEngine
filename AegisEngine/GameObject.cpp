#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

#include	"GameObject.h"

unordered_set<string> GAME_OBJECT::Object_Name_Map;

GAME_OBJECT::GAME_OBJECT() : Object_Name("none"), DestroyFlag(false)
{
	Component.reset(new COMPONENT_MANEGER());
}

GAME_OBJECT::~GAME_OBJECT()
{
	Object_Name_Map.erase(Object_Name);

	Component.reset();
};

void GAME_OBJECT::Init()
{
	Component->Init();
}

void GAME_OBJECT::Draw()
{
	Component->Draw();
}

void GAME_OBJECT::Update(float delta_time)
{
	Component->Update(delta_time);
}

void GAME_OBJECT::Set_Destroy()
{
	DestroyFlag = true;
};

bool GAME_OBJECT::Destroy()
{
	return DestroyFlag;
};

void GAME_OBJECT::Set_Object_Name(const string& name)
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