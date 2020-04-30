#ifdef _DEBUG

#pragma once

#ifndef MY_IMGUI_H
#define MY_IMGUI_H

#include	"main.h"

namespace ImGui {

	//! ��`�̕`�悷��ׂ̃I���W�i���֗̕��֐�
	void DrawRect(const ImVec2& size, const ImVec4& color, const char* text, const ImVec4& text_color, const float frame_size = 0.0f, const ImVec4& frame_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

}

class COMPONENT;
class GAME_OBJECT;

class My_imgui{
private:
	bool show_demo_window;
	bool show_another_window;
	bool show_default_window;

	bool Texture_Import_Enable;		// �e�N�X�`���C���|�[�g���j���[
	bool Texture_Delete_Enable;		// �e�N�X�`���폜���j���[

	bool Model_Import_Enable;		// ���f���C���|�[�g���j���[
	bool Model_Delete_Enable;		// ���f���폜���j���[

	bool Setting_Enable;

	bool Mouse_Over_Enable;

	bool Debug_Draw_Enable;

	//pair< vector<const char*>, vector<const char*> > Component_Items;
	pair< vector<string>, vector<string> > Component_Items;

	void Texture_Import();
	const char Texture_File_Check(const string& file_name);
	void Texture_Delete();

	void Model_Import();
	const char Model_File_Check(const string& file_name);
	void Model_Delete();

	void Setting();

	void File();

	void Light_Setting();

	void Draw_Inspector(const string& name);

	void Draw_Components(const vector<COMPONENT*>& components);

	void Add_Component(GAME_OBJECT* object, const string s);

	void Delete_Component(GAME_OBJECT* object, const string s);

public:
	My_imgui() : show_demo_window(false), show_another_window(false), show_default_window(false),
				 Texture_Import_Enable(false), Texture_Delete_Enable(false), Model_Import_Enable(false), Model_Delete_Enable(false), Setting_Enable(false), Mouse_Over_Enable(false){

#ifdef _DEBUG
		Debug_Draw_Enable = true;
#else
		Debug_Draw_Enable = false;
#endif // _DEBUG
	}

	~My_imgui() {}

	void Init(HWND hWnd);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	void Begin();
	void End();
	void Render(void);

	const bool Get_Mouse_Over_Enable();
};

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

#endif // !MY_IMGUI_H

#endif // _DEBUG