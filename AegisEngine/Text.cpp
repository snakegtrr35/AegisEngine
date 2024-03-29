﻿#include	"Text.h"
#include	"manager.h"
#include	"ShadowMap.h"
#include	"Scene.h"

using namespace aegis;

TEXTS::TEXTS()
{
	Text = "";
}

TEXTS::~TEXTS()
{
	Uninit();
}

void TEXTS::Init(void)
{
	SPRITE::Init();
}

void TEXTS::Draw(void)
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		if (Enable)
		{
			Text_Draw(Text);

			Draw_Child();
		}
	}
}

void TEXTS::Draw_DPP(void)
{
	Text_Draw(Text);

	Draw_DPP_Child();
}

void TEXTS::Update(float delta_time)
{
}

void TEXTS::Uninit(void)
{
}

void TEXTS::Edit(const std::string& text)
{
	Text = text;
}

void TEXTS::Text_Draw(const std::string& text)
{
	SPRITE sprite;
	ID3D11ShaderResourceView* shader_resource_view = nullptr;
	std::wstring font;
	short i = 0;

	sprite.Init();

	sprite.SetSize(Size);

	sprite.SetColor(Color);

	std::string ctext = text;
	std::wstring wtext = stringTowstring(ctext);

	for (auto itr : wtext)
	{
		font.push_back(itr);
		shader_resource_view = FONT::Get_Font_Resource(font);

		sprite.Set(shader_resource_view);

		sprite.SetPosition(Vector2(Position.x + (Size.w * i * 2), Position.y));

		sprite.Draw();

		i++;

		font.pop_back();
	}
}

void TEXTS::Text_Draw_DPP(const std::string& text)
{
	SPRITE sprite;
	ID3D11ShaderResourceView* shader_resource_view = nullptr;
	std::wstring font;
	short i = 0;

	sprite.Init();

	sprite.SetSize(Size);

	sprite.SetColor(Color);

	std::string ctext = text;
	std::wstring wtext = stringTowstring(ctext);

	for (auto itr : wtext)
	{
		font.push_back(itr);
		shader_resource_view = FONT::Get_Font_Resource(font);

		sprite.Set(shader_resource_view);

		sprite.SetPosition(Vector2(Position.x + (Size.w * i * 2), Position.y));

		sprite.Draw_DPP();

		i++;

		font.pop_back();
	}
}