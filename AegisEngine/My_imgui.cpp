#ifdef _DEBUG

#include	"My_imgui.h"
#include	"Renderer.h"
#include	"Scene.h"
#include	"main.h"

#include	"ModelLoader.h"

POLYGON_3D* g_pPOLYGON = nullptr;

My_imgui::My_imgui()
{
	show_demo_window = false;
	show_another_window = false;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	f = 0.0f;
	counter = 0;
}

My_imgui::~My_imgui()
{
}

void My_imgui::Init(HWND hWnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// u8を使えば日本語の表示はできる
	ImFontConfig config;

	config.OversampleH = 2;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 1.0f;

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
}

void My_imgui::Draw(void)
{
	//Start the Dear ImGui frame
	{
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
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
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		if (show_app_style_editor) { ImGui::Begin("Style Editor", &show_app_style_editor, ImGuiWindowFlags_NoResize); ImGui::ShowStyleEditor(); ImGui::End(); }

		{
			string name("player");

			auto player = SCENE::Get_Game_Object<PLAYER>(name);

			if (nullptr != player)
			{

				static float vec4_Position[] = { player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z };
				static float vec4_Rotation[] = { player->Get_Rotation()->x, player->Get_Rotation()->y, player->Get_Rotation()->z };
				static float vec4_Scaling[] = { player->Get_Scaling()->x, player->Get_Scaling()->y, player->Get_Scaling()->z };
				static float* vec1 = player->Get();

				ImGui::Begin("Setting");

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

				/*static char buf1[64] = ""; 
				
				if (ImGui::InputText("default", buf1, 64))
				{
					string a;
					a = buf1;
				}*/

				static char buf1[128] = "";

				ImGui::InputText((char*)u8"あいうえお", (char*)buf1, 128);

				ImGui::Text(buf1);

				ImGui::SliderFloat("Blend", vec1, 0.0f, 1.0f);
				//player->blend = vec1;

				ImGui::End();

				if (enable)
				{
					XMFLOAT3 vec1(vec4_Position[0], vec4_Position[1], vec4_Position[2]);
					XMFLOAT3 vec2(vec4_Rotation[0], vec4_Rotation[1], vec4_Rotation[2]);
					XMFLOAT3 vec3(vec4_Scaling[0], vec4_Scaling[1], vec4_Scaling[2]);

					player->Set_Position(&vec1);
					player->Set_Rotation(&vec2);
					player->Set_Scaling(&vec3);
				}
			}

			// ライトの設定
			{
				ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoResize;

				static float vec4_Direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
				static float vec4_Diffuse[] = { 1.2f, 1.2f, 1.2f, 1.0f };
				static float vec4_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

				ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Always);

				ImGui::Begin("Directional Light", nullptr, window_flag);

				ImGui::DragFloat3("Direction", vec4_Direction, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光源\" 向き\n");

				ImGui::DragFloat3("Diffuse", vec4_Diffuse, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光源\" 直接光\n");

				ImGui::DragFloat3("Ambient", vec4_Ambient, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光源\" 環境光\n");

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
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

				ImGui::SetNextWindowSize(ImVec2(737, 445), ImGuiCond_Always);

				ImGui::Begin("RenderTexture", nullptr, window_flags);

				ImTextureID image = CRenderer::Get_SRV();

				ImGui::Image(image, ImVec2(480 * 1.5f, 270 * 1.5f));

				ImGui::End();
			}

			/*{
				ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoTitleBar;

				ImGui::Begin("World", nullptr, window_flag);
				if (ImGui::TreeNode("World"))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
					//for (int i = 0; i < ; i++)
					for (int i = 0; i < 6; i++)
					{
						// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
						ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
						if (i < 3)
						{
							// Items 0..2 are Tree Node
							bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Object %d", i);
							if (ImGui::IsItemClicked());

							if (node_open)
							{
								ImGui::Text("Blah blah\nBlah Blah");
								ImGui::TreePop();
							}
						}
						else
						{
							// Items 3..5 are Tree Leaves
							// The only reason we use TreeNode at all is to allow selection of the leaf.
							// Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
							node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
							ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf Object %d", i);
							if (ImGui::IsItemClicked());

						}
					}
					ImGui::PopStyleVar();
					ImGui::TreePop();

				}
				ImGui::End();
			}*/
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
}

void My_imgui::End()
{
	ImGui::EndFrame();
}

void My_imgui::Render(void)
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Draw_imgui(XMFLOAT3* position, XMFLOAT3* rotation, XMFLOAT3* scaling)
{
	//float vec4_Position[3] = { position->x, position->y, position->z };
	//float vec4_Rotation[3] = { rotation->x, rotation->y, rotation->z };
	//float vec4_Scaling[3] = { scaling->x, scaling->y, scaling->z };

	////ImGui::

	//ImGui::BeginChildFrame(ImGui::GetID());
	//{
	//	ImGui::Begin("Setting");

	//	ImGui::InputFloat3("Position", vec4_Position);
	//	ImGui::InputFloat3("Rotation", vec4_Rotation);
	//	ImGui::InputFloat3("Scaling", vec4_Scaling);

	//	ImGui::End();

	//	position->x = vec4_Position[0];
	//	position->y = vec4_Position[1];
	//	position->z = vec4_Position[2];

	//	rotation->x = vec4_Position[0];
	//	rotation->y = vec4_Position[1];
	//	rotation->z = vec4_Position[2];

	//	scaling->x = vec4_Position[0];
	//	scaling->y = vec4_Position[1];
	//	scaling->z = vec4_Position[2];
	//}
}

#endif // _DEBUG