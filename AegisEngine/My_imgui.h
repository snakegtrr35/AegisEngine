#ifdef _DEBUG

#pragma once

#ifndef MY_IMGUI_H
#define MY_IMGUI_H

#include	"main.h"

#include	"imgui/imgui.h"
#include	"imgui/imgui_impl_dx11.h"
#include	"imgui/imgui_impl_win32.h"

class My_imgui{
private:
	bool show_demo_window;
	bool show_another_window;
	bool show_default_window;
	ImVec4 clear_color;
	float f;
	int counter;

public:
	My_imgui() : show_demo_window(false), show_another_window(false), show_default_window(false), clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f)), f(0.0f), counter(0) {}
	~My_imgui() {}

	void Init(HWND hWnd);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	void Begin();
	void End();
	void Render(void);
};

void Draw_Inspector(const string& name);

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