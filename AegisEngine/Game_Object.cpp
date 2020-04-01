#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

#include	"Game_Object.h"
#include	"Component.h"

unordered_set<string> GAME_OBJECT::Object_Name_Map;

GAME_OBJECT::GAME_OBJECT() : Object_Name("none"), DestroyFlag(false), Rotation(XMFLOAT3(0.0f, 0.0f, 0.0f)), Position(XMFLOAT3(0.0f, 0.0f, 0.0f)), Scaling(XMFLOAT3(1.0f, 1.0f, 1.0f))
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

// ƒƒ‚ƒŠã‚©‚ç‚Ìíœ
bool GAME_OBJECT::Destroy()
{
	if (DestroyFlag)
	{
		return true;
	}
	return false;
};

void GAME_OBJECT::Set_Object_Name(const string& name)
{

	if (Object_Name_Map.find(name) == Object_Name_Map.end())
	{
		Object_Name_Map.emplace(name);
		Object_Name = name;
	}
#ifdef _DEBUG
	else
	{
		//Erroer_Message("Šù‚Ég‚í‚ê‚Ä‚¢‚é–¼‘O‚Å‚·");

   		int a = 0;
	}
#endif // _DEBUG
};