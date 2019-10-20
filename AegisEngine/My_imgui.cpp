#ifdef _DEBUG

#include	"My_imgui.h"
#include	"Scene.h"
#include	"Game_Object.h"
#include	"manager.h"
#include	"ModelLoader.h"

#include	"imgui/ImGuizmo.h"

extern XMFLOAT2 center;
extern XMFLOAT2 wh;

extern float radius;

void EditTransform(const float* cameraView, float* cameraProjection, float* matrix);

void My_imgui::Init(HWND hWnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;  // Enable Keyboard Controls

	// u8を使えば日本語の表示はできる
	ImFontConfig config;

	config.OversampleH = 3;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 0.0f;
	config.GlyphExtraSpacing.y = 0.0f;

	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 15.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 14.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 15.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 14.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	//io.Fonts->AddFontFromFileTTF("asset/font/NotoSansCJKjp-Regular.otf", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());

	// Setup Style
	ImGui::StyleColorsDark();

	ImVec4 color = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_WindowBg] = color;
}

void My_imgui::Draw(void)
{
	//Start the Dear ImGui frame
	{
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		if(show_default_window)
		{
			// Create a window called "Hello, world!" and append into it.
			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color

			// Buttons return true when clicked (most widgets return true when edited/activated)
			if (ImGui::Button("Button"))
				counter++;

			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::Text("x = %f", ImGui::GetMousePos().x);
			ImGui::Text("y = %f", ImGui::GetMousePos().y);

			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;

			ImGui::End();

		}

		static bool show_app_style_editor = false;

		// メニューバー
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New")) {}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {}
					if (ImGui::BeginMenu("Open Recent"))
					{
						ImGui::MenuItem("fish_hat.c");
						ImGui::MenuItem("fish_hat.inl");
						ImGui::MenuItem("fish_hat.h");
						if (ImGui::BeginMenu("More.."))
						{
							ImGui::MenuItem("Hello");
							ImGui::MenuItem("Sailor");
							if (ImGui::BeginMenu("Recurse.."))
							{
								
								ImGui::EndMenu();
							}
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Close"))
					{
						CManager::GameEnd();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
					if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "CTRL+X")) {}
					if (ImGui::MenuItem("Copy", "CTRL+C")) {}
					if (ImGui::MenuItem("Paste", "CTRL+V")) {}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Setting"))
				{
					ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
					ImGui::Checkbox("Default Window", &show_default_window);      // Edit bools storing our window open/close state
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		if (show_app_style_editor) { ImGui::Begin("Style Editor", &show_app_style_editor, ImGuiWindowFlags_NoResize); ImGui::ShowStyleEditor(); ImGui::End(); }

		static string s = GAME_OBJECT::Get_Object_Name_Map().begin()->second.c_str();

		{
			{
				Draw_Inspector(s);
			}

			{
				float* r = &radius;

				static float center2[2] = { center.x, center.y };
				static float wh2[2] = { wh.x, wh.y };

				ImGui::Begin("Setting");

				{
					static char str[128] = "";

					ImGui::InputText((char*)u8"あいうえお", str, 128);

					ImGui::Text(str);
				}

				{
				ImGui::DragFloat2("Center Position", center2, 1.0f, 0.f);
				ImGui::DragFloat2("WH", wh2, 1.0f, 0.f);
				}

				ImGui::DragFloat("Radius", r, 0.1f, 0.1f, 100.0f);

				ImGui::Text("%s", s.c_str());

				ImGui::End();

				center.x = center2[0];
				center.y = center2[1];

				wh.x = wh2[0];
				wh.y = wh2[1];
				}

				// ライトの設定
			{
			ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoResize;

			static float vec4_Direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
			static float vec4_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			static float vec4_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

			ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Always);

			ImGui::Begin("Directional Light", nullptr, window_flag);

			ImGui::DragFloat3("Direction", vec4_Direction, 0.01f);
			ImGui::SameLine(); HelpMarker((char*)u8"\"平行光\" 向き\n");

			ImGui::DragFloat3("Diffuse", vec4_Diffuse, 0.01f);
			ImGui::SameLine(); HelpMarker((char*)u8"\"平行光\" 直接光\n");

			ImGui::DragFloat3("Ambient", vec4_Ambient, 0.01f);
			ImGui::SameLine(); HelpMarker((char*)u8"\"平行光\" 環境光\n");

			ImGui::End();

			// ライトの設定
			LIGHT light;
			light.Direction = XMFLOAT4(vec4_Direction[0], vec4_Direction[1], vec4_Direction[2], vec4_Direction[3]);
			light.Diffuse = COLOR(vec4_Diffuse[0], vec4_Diffuse[1], vec4_Diffuse[2], vec4_Diffuse[3]);
			light.Ambient = COLOR(vec4_Ambient[0], vec4_Ambient[1], vec4_Ambient[2], vec4_Ambient[3]);
			CRenderer::SetLight(&light);
			}

			// レンダリングテクスチャ
			{
				// タイトルバーありの場合、imageyよりWindowSizeがImVec2(17, 40)分大きければ丁度いい
				// タイトルバーなしの場合、imageyよりWindowSizeがImVec2(13, 16)分大きければ丁度いい

				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

				ImGui::SetNextWindowSize(ImVec2(1280 + 17, 720 + 40), ImGuiCond_Once);

				ImGui::Begin("Debug", nullptr, window_flags);

				ImTextureID image = CRenderer::Get_SRV();

				ImGui::Image(image, ImVec2(1280, 720));

				ImGui::End();
			}

			{
				ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoTitleBar;
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				auto map = GAME_OBJECT::Get_Object_Name_Map();

				ImGui::Begin("World", nullptr, window_flag);

				ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

				{
					int i = 0;
					for (auto object : map)
					{
						// Mouse buttons : 0 = left, 1 = right, 2 = middle + extras

						string str = object.second.c_str();

						node_flags |= /*ImGuiTreeNodeFlags_Leaf |*/ ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
						ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, str.c_str());
						if (ImGui::IsItemClicked())
						{
							s = str.c_str();
						}

						i++;
					}
				}

				ImGui::End();

			}
		}

		{
			auto camera = SCENE::Get_Game_Object<CCamera>("camera");
			auto player = SCENE::Get_Game_Object<PLAYER>("player");

			if (nullptr != camera && nullptr != player)
			{
				XMMATRIX mtr = camera->Get_Camera_View();
				XMFLOAT4X4 mat44;
				XMStoreFloat4x4(&mat44, mtr);

				float view[] = { mat44._11, mat44._12, mat44._13,mat44._14,
								 mat44._21, mat44._22, mat44._23,mat44._24,
								 mat44._31, mat44._32, mat44._33,mat44._34,
								 mat44._41, mat44._42, mat44._43,mat44._44
				};

				mtr = camera->Get_Camera_Projection();
				XMStoreFloat4x4(&mat44, mtr);

				float pro[] = { mat44._11, mat44._12, mat44._13,mat44._14,
								mat44._21, mat44._22, mat44._23,mat44._24,
								mat44._31, mat44._32, mat44._33,mat44._34,
								mat44._41, mat44._42, mat44._43,mat44._44
				};

				mtr = XMMatrixTranslation(player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z);
				XMStoreFloat4x4(&mat44, mtr);

				static float pos[] = { mat44._11, mat44._12, mat44._13,mat44._14,
								mat44._21, mat44._22, mat44._23,mat44._24,
								mat44._31, mat44._32, mat44._33,mat44._34,
								mat44._41, mat44._42, mat44._43,mat44._44
				};

				ImGui::SetNextWindowPos(ImVec2(10, 10));
				ImGui::SetNextWindowSize(ImVec2(320, 340));
				ImGui::Begin("Editor");

				EditTransform(view, pro, pos);

				ImGui::End();
			}
		}

		// Rendering
		ImGui::Render();
	}
}

void My_imgui::Update(void)
{
}

void My_imgui::Uninit(void)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void My_imgui::Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();
}

void My_imgui::End()
{
	ImGui::EndFrame();
}

void My_imgui::Render(void)
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Draw_Inspector(const string& name)
{
	auto object = SCENE::Get_Game_Object(name);

	if (nullptr != object)
	{
		float vec4_Position[] = { object->Get_Position()->x, object->Get_Position()->y, object->Get_Position()->z };
		float vec4_Rotation[] = { object->Get_Rotation()->x, object->Get_Rotation()->y, object->Get_Rotation()->z };
		float vec4_Scaling[] = { object->Get_Scaling()->x, object->Get_Scaling()->y, object->Get_Scaling()->z };

		ImGui::Begin("Inspector");

		ImGui::Text(object->Get_Object_Name().c_str());

		static bool enable = false;
		ImGui::Checkbox("Enable", &enable);

		ImGui::DragFloat3("Position", vec4_Position, 0.02f);

		ImGui::DragFloat3("Rotation", vec4_Rotation, 0.2f, -360.0f, 360.0f);

		ImGui::DragFloat3("Scaling", vec4_Scaling, 0.001f);

		static int clicked = 0;
		if (ImGui::Button("Add Component"))
			clicked++;
		if (clicked & 1)
		{
			ImGui::SameLine();
			ImGui::Text("Thanks for clicking me!");
		}

		ImGui::End();

		if (enable)
		{
			XMFLOAT3 vec1(vec4_Position[0], vec4_Position[1], vec4_Position[2]);
			XMFLOAT3 vec2(vec4_Rotation[0], vec4_Rotation[1], vec4_Rotation[2]);
			XMFLOAT3 vec3(vec4_Scaling[0], vec4_Scaling[1], vec4_Scaling[2]);

			//object->Set_Position(&vec1);
			object->Set_Rotation(&vec2);
			object->Set_Scaling(&vec3);
		}
	}
}


void EditTransform(const float* cameraView, float* cameraProjection, float* matrix)
{
	auto player = SCENE::Get_Game_Object<PLAYER>("player");

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;


	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	//static float	matrixTranslation[3] = { player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z },
	//	matrixRotation[3] = { player->Get_Rotation()->x, player->Get_Rotation()->y, player->Get_Rotation()->z },
	//	matrixScale[3] = { player->Get_Scaling()->x, player->Get_Scaling()->y, player->Get_Scaling()->z };

	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation, 3);
	ImGui::InputFloat3("Rt", matrixRotation, 3);
	ImGui::InputFloat3("Sc", matrixScale, 3);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	XMFLOAT3 vec1(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
	XMFLOAT3 vec2(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
	XMFLOAT3 vec3(matrixScale[0], matrixScale[1], matrixScale[2]);

	player->Set_Position(&vec1);
	player->Set_Rotation(&vec2);
	player->Set_Scaling(&vec3);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;
	ImGui::Checkbox("", &useSnap);
	ImGui::SameLine();

	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}
	ImGui::Checkbox("Bound Sizing", &boundSizing);
	if (boundSizing)
	{
		ImGui::PushID(3);
		ImGui::Checkbox("", &boundSizingSnap);
		ImGui::SameLine();
		ImGui::InputFloat3("Snap", boundsSnap);
		ImGui::PopID();
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

}

#endif // _DEBUG