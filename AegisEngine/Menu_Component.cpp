#include	"Component.h"
#include	"Input.h"
#include	"Scene.h"
#include	"manager.h"
#include	"Sprite.h"

using namespace Aegis;

MOUSE_EVENT_COMPONENT::MOUSE_EVENT_COMPONENT()
{
}

MOUSE_EVENT_COMPONENT::~MOUSE_EVENT_COMPONENT()
{
	Uninit();
}

void MOUSE_EVENT_COMPONENT::Update(float delta_time)
{
}

void MOUSE_EVENT_COMPONENT::Uninit()
{
}

bool MOUSE_EVENT_COMPONENT::Mouse_Click_Left(const string& const name, SPRITE* me)
{
	Vector2 mouse_pos = MOUSE::Get_Mouse()->Get_Screen_Position();
	Vector2* pos;
	Vector4* size;
	Vector2 position;

	if (KEYBOARD::Trigger_Keyboard(VK_LBUTTON))
	{
		if (name == me->Get_Object_Name() && me->GetEnable())
		{
			pos = me->GetPosition();
			size = me->GetSize();

			position.x = pos->x;
			position.y = pos->y;

			position.y += 17.0f;	// WS_OVERLAPPEDWINDOW の時
			//position.y += 20.0f;	// WS_POPUPWINDOW の時

			if ((pos->x - size->w) < mouse_pos.x && mouse_pos.x < (pos->x + size->y) &&
				(pos->y - size->x) < mouse_pos.y && mouse_pos.y < (pos->y + size->z))
			{
				return true;
			}
		}

		auto children = me->Get_Child_Sptite();

		for (const auto& child : *children)
		{
			if (name == child->Child->Get_Object_Name() && child->Child->GetEnable())
			{
				pos = child->Child->GetPosition();
				size = child->Child->GetSize();

				position.x = pos->x;
				position.y = pos->y;

				position.y += 17.0f;	// WS_OVERLAPPEDWINDOW の時
				//position.y += 20.0f;	// WS_POPUPWINDOW の時

				if ((pos->x - size->w) < mouse_pos.x && mouse_pos.x < (pos->x + size->y) &&
					(pos->y - size->x) < mouse_pos.y && mouse_pos.y < (pos->y + size->z))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool MOUSE_EVENT_COMPONENT::Mouse_Click_Right(const string& const name, SPRITE* me)
{
	Vector2 mouse_pos = MOUSE::Get_Mouse()->Get_Screen_Position();
	Vector2* pos;
	Vector4* size;
	Vector2 position;

	if (KEYBOARD::Trigger_Keyboard(VK_RBUTTON))
	{
		if (name == me->Get_Object_Name() && me->GetEnable())
		{
			pos = me->GetPosition();
			size = me->GetSize();

			position.x = pos->x;
			position.y = pos->y;

			position.y += 17.0f;	// WS_OVERLAPPEDWINDOW の時
			//position.y += 20.0f;	// WS_POPUPWINDOW の時

			if ((pos->x - size->w) < mouse_pos.x && mouse_pos.x < (pos->x + size->y) &&
				(pos->y - size->x) < mouse_pos.y && mouse_pos.y < (pos->y + size->z))
			{
				return true;
			}
		}

		auto children = me->Get_Child_Sptite();

		for (const auto& child : *children)
		{
			if (name == child->Child->Get_Object_Name() && child->Child->GetEnable())
			{
				pos = child->Child->GetPosition();
				size = child->Child->GetSize();

				position.x = pos->x;
				position.y = pos->y;

				position.y += 17.0f;	// WS_OVERLAPPEDWINDOW の時
				//position.y += 20.0f;	// WS_POPUPWINDOW の時

				if ((pos->x - size->w) < mouse_pos.x && mouse_pos.x < (pos->x + size->y) &&
					(pos->y - size->x) < mouse_pos.y && mouse_pos.y < (pos->y + size->z))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool MOUSE_EVENT_COMPONENT::Mouse_Over(const string& const name, SPRITE* me)
{
	Vector2 mouse_pos = MOUSE::Get_Mouse()->Get_Screen_Position();
	Vector2* pos;
	Vector4* size;
	Vector2 position;

	// マウスと矩形の当たり判定

	if (name == me->Get_Object_Name() && me->GetEnable())
	{
		pos = me->GetPosition();
		size = me->GetSize();

		position.x = pos->x;
		position.y = pos->y;

		position.y += 17.0f;	// WS_OVERLAPPEDWINDOW の時
		//position.y += 20.0f;	// WS_POPUPWINDOW の時

		if ((pos->x - size->w) <= mouse_pos.x && mouse_pos.x <= (pos->x + size->y) &&
			(pos->y - size->x) <= mouse_pos.y && mouse_pos.y <= (pos->x + size->z))
		{
			return true;
		}
	}

	auto children = me->Get_Child_Sptite();

	for (const auto& child : *children)
	{
		if (name == child->Child->Get_Object_Name() && child->Child->GetEnable())
		{
			pos = child->Child->GetPosition();
			size = child->Child->GetSize();

			position.x = pos->x;
			position.y = pos->y;

			position.y += 17.0f;	// WS_OVERLAPPEDWINDOW の時
			//position.y += 20.0f;	// WS_POPUPWINDOW の時

			if ((pos->x - size->w) <= mouse_pos.x && mouse_pos.x <= (pos->x + size->y) &&
				(pos->y - size->x) <= mouse_pos.y && mouse_pos.y <= (pos->x + size->z))
			{
				return true;
			}
		}
	}
	return false;
}






KEYBOARD_EVENT_COMPONENT::KEYBOARD_EVENT_COMPONENT()
{
}

KEYBOARD_EVENT_COMPONENT::~KEYBOARD_EVENT_COMPONENT()
{
	Uninit();
}

void KEYBOARD_EVENT_COMPONENT::Update(float delta_time)
{
}

void KEYBOARD_EVENT_COMPONENT::Uninit()
{
}