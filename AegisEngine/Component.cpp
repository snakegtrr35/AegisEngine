#include	"Component.h"

#include	"imgui/imgui.h"

void COMPONENT::Draw_Inspector()
{
	if (ImGui::IsMouseClicked(1))
	{
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("popupID");
		}
	}

	auto id = ImGui::GetID((char*)u8"コリジョン(AABB)");

	{
		const char* names[] = { (char*)u8"削除", (char*)u8"BBB" };
		static int selected = -1;

		if (ImGui::BeginPopup("popupID"))
		{
			ImGui::Text("選択項目");
			ImGui::Separator();
			for (int i = 0; i < IM_ARRAYSIZE(names); i++)
			{
				if (ImGui::Selectable(names[i]))
				{
					selected = i;

					if (0 == selected)
					{
						SetDestroy();
					}
				}
			}
			ImGui::EndPopup();
		}
	}
}

void COMPONENT::Set_Owner(const weak_ptr<GAME_OBJECT>& owner)
{
	Owner = owner;
}

void COMPONENT::SetEnable(const bool flag)
{
	Enable = flag;
}

const bool COMPONENT::GetEnable()
{
	return Enable;
}

void COMPONENT::SetDestroy()
{
	DestroyFlag = true;
}

void COMPONENT::Set_Draw_Enable(const bool flag)
{
	Draw_Enable = flag;
}

bool COMPONENT::Get_Draw_Enable()
{
	return Draw_Enable;
}

XMFLOAT3* const COMPONENT::Get_Position()
{
	return &Position;
};

XMFLOAT3* const COMPONENT::Get_Rotation()
{
	return &Rotation;
};

XMFLOAT3* const COMPONENT::Get_Scaling()
{
	return &Scaling;
};



void COMPONENT::Set_Position(XMFLOAT3* const position)
{
	Position = *position;
}

void COMPONENT::Set_Position(const XMFLOAT3& position)
{
	Position = position;
};

void COMPONENT::Set_Rotation(XMFLOAT3* const rotation)
{
	Rotation = *rotation;
};

void COMPONENT::Set_Rotation(const XMFLOAT3& rotation)
{
	Rotation = rotation;
};

void COMPONENT::Set_Scaling(XMFLOAT3* scaling)
{
	Scaling = *scaling;
};

void COMPONENT::Set_Scaling(const XMFLOAT3& scaling)
{
	Scaling = scaling;
};

// メモリ上からの削除
bool COMPONENT::Destroy()
{
	if (DestroyFlag)
	{
		return true;
	}
	return false;
}