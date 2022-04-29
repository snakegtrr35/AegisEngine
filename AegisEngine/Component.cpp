#include	"Component.h"

#include	"imgui/imgui.h"

IMPLEMENT_ABSTRACT_OBJECT_TYPE_INFO(aegis::AegisObject, COMPONENT)

using namespace aegis;

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

//void COMPONENT::Set_Owner(const std::weak_ptr<GameObject>& owner)
void COMPONENT::Set_Owner(GameObject* owner)
{
	Owner = owner;

	if (Owner != nullptr)
	{
		Enable = true;
	}
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

//Vector3* const COMPONENT::Get_Position()
//{
//	return &Position;
//};
//
//Vector3* const COMPONENT::Get_Rotation()
//{
//	return &Rotation;
//};
//
//Vector3* const COMPONENT::Get_Scaling()
//{
//	return &Scaling;
//};
//
//
//
//void COMPONENT::Set_Position(Vector3* const position)
//{
//	Position = *position;
//}
//
//void COMPONENT::Set_Position(const Vector3& position)
//{
//	Position = position;
//};
//
//void COMPONENT::Set_Rotation(Vector3* const rotation)
//{
//	Rotation = *rotation;
//};
//
//void COMPONENT::Set_Rotation(const Vector3& rotation)
//{
//	Rotation = rotation;
//};
//
//void COMPONENT::Set_Scaling(Vector3* scaling)
//{
//	Scaling = *scaling;
//};
//
//void COMPONENT::Set_Scaling(const Vector3& scaling)
//{
//	Scaling = scaling;
//};

// メモリ上からの削除
bool COMPONENT::Destroy()
{
	if (DestroyFlag)
	{
		return true;
	}
	return false;
}