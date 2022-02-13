#include	"Component_Manager.h"
#include	"Component.h"

#ifdef _DEBUG
bool COMPONENT_MANEGER::Draw_Enable_All = true;
#else
bool COMPONENT_MANEGER::Draw_Enable_All = false;
#endif // _DEBUG

void COMPONENT_MANEGER::Set_Draw_Enable_All(const bool flag)
{
	Draw_Enable_All = flag;
}

const bool COMPONENT_MANEGER::Get_Draw_Enable_All()
{
	return Draw_Enable_All;
}

aegis::vector<COMPONENT*> COMPONENT_MANEGER::Get_All_Components()
{
	aegis::vector<COMPONENT*> objects;
	objects.reserve(Conponents.size());

	for (const auto& object : Conponents)
	{
		objects.emplace_back(object.get());
	}
	return objects;
}

void COMPONENT_MANEGER::Init()
{
	for (const auto& object : Conponents)
	{
		object->Init();
	}
}

void COMPONENT_MANEGER::Update(float delta_time)
{
	for (const auto& object : Conponents)
	{
		if (object->GetEnable())
		{
			object->Update(delta_time);
		}
	}

	//Conponent_List.remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
	Conponents.erase(std::remove_if(Conponents.begin(), Conponents.end(), [](auto& object) { return object->Destroy(); }), Conponents.end());
}

void COMPONENT_MANEGER::Draw()
{
	if (Draw_Enable_All)
	{
		for (const auto& object : Conponents)
		{
			if (object->Get_Draw_Enable())
			{
				object->Draw();
			}
		}
	}
}

void COMPONENT_MANEGER::Draw_DPP()
{
	if (Draw_Enable_All)
	{
		for (const auto& object : Conponents)
		{
			if (object->Get_Draw_Enable())
			{
				object->Draw_DPP();
			}
		}
	}
}

void COMPONENT_MANEGER::Uninit()
{
	for (auto& object : Conponents)
	{
		object.reset(nullptr);
	}
	Conponents.clear();
}