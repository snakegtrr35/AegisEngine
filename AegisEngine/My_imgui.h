#ifdef _DEBUG

#pragma once

#ifndef MY_IMGUI_H
#define MY_IMGUI_H

#include	"main.h"
#include	"imgui/imgui.h"

class My_imgui{
private:
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;
	float f;
	int counter;

public:
	My_imgui();
	~My_imgui();

	void Init(HWND hWnd);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	void Begin();
	void End();
	void Render(void);
};

void Draw_imgui(XMFLOAT3* position, XMFLOAT3* rotation, XMFLOAT3* scaling);

#endif // !MY_IMGUI_H

#endif // _DEBUG