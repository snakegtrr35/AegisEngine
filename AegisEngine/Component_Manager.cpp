#include "Component_Manager.h"
#include "Component.h"


COMPONENT_MANEGER* COMPONENT_MANEGER::getInstance()
{
	static std::unique_ptr<COMPONENT_MANEGER> instance(new COMPONENT_MANEGER);
	return instance.get();
}

void COMPONENT_MANEGER::Update(float delta_time)
{
	for (const auto& conponents : mComponentArrays)
	{
		conponents.second->Update(delta_time);
	}
	for (const auto& conponents : mComponentArrays)
	{
		conponents.second->Destory();
	}
}

void COMPONENT_MANEGER::Draw()
{
	for (const auto& conponents : mComponentArrays)
	{
		conponents.second->Draw();
	}
}

void COMPONENT_MANEGER::Draw_DPP()
{
	for (const auto& conponents : mComponentArrays)
	{
		conponents.second->Draw_DPP();
	}
}

void COMPONENT_MANEGER::Uninit()
{
	for (const auto& conponents : mComponentArrays)
	{
		conponents.second->Uninit();
	}
	mComponentArrays.clear();
}