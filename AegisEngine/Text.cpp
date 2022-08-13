#include "Text.h"
#include "manager.h"
#include "ShadowMap.h"
#include "Scene.h"
#include "texture.h"

IMPLEMENT_OBJECT_TYPE_INFO(SPRITE, TEXTS)

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
	SPRITE::InitEnd();
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

void TEXTS::Edit(const aegis::string& text)
{
	Text = text;
}

void TEXTS::Text_Draw(const aegis::string& text)
{
	SPRITE sprite;
	aegis::ShaderResourceView* shader_resource_view = nullptr;
	std::wstring font;
	short i = 0;

	sprite.Init();

	sprite.SetSize(Size);

	sprite.SetColor(Color);

	aegis::string ctext = text;
	std::wstring wtext = stringTowstring(ctext);

	for (auto itr : wtext)
	{
		font.push_back(itr);
		shader_resource_view = FONT::Get_Font_Resource(font.c_str());

		sprite.Set(shader_resource_view);

		sprite.SetPosition(Vector2(Position.x + (Size.w * i * 2), Position.y));

		sprite.Draw();

		i++;

		font.pop_back();
	}
}

void TEXTS::Text_Draw_DPP(const aegis::string& text)
{
	SPRITE sprite;
	aegis::ShaderResourceView* shader_resource_view = nullptr;
	aegis::wstring font;
	short i = 0;

	sprite.Init();

	sprite.SetSize(Size);

	sprite.SetColor(Color);

	aegis::string ctext = text;
	aegis::wstring wtext = stringTowstring(ctext).c_str();

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