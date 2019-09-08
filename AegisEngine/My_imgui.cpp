#ifdef _DEBUG

#include	"My_imgui.h"
#include	"Renderer.h"

#include	"Scene.h"

#include	"Player.h"
#include	"Enemy.h"
#include	"ModelLoader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include	"main.h"

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

	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// u8を使えば日本語の表示はできる
	ImFontConfig config;

	config.OversampleH = 2;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 1.0f;

	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	//io.Fonts->AddFontFromFileTTF("asset/font/NotoSansCJKjp-Regular.otf", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());

	// Setup Style
	ImGui::StyleColorsDark();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.f, 0.f, 0.f, 1.0f);
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
				ImGui::EndMainMenuBar();
			}
		}

		{
			string name("garage");

			//PLAYER* player = SCENE::Get_Game_Object<PLAYER>();
			//MESH_WALL* wall = SCENE::Get_Game_Object<MESH_WALL>();
			//ENEMY* wall = SCENE::Get_Game_Object<ENEMY>();
			CMODEL* player = SCENE::Get_Game_Object<CMODEL>(name);

			if (nullptr != player)
			{

				static float vec4_Position[] = { player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z };
				static float vec4_Rotation[] = { player->Get_Rotation()->x, player->Get_Rotation()->y, player->Get_Rotation()->z };
				static float vec4_Scaling[] = { player->Get_Scaling()->x, player->Get_Scaling()->y, player->Get_Scaling()->z };

				ImGui::Begin("Setting");

				static bool enable = false;
				ImGui::Checkbox("Enable", &enable);

				ImGui::DragFloat3("Position", vec4_Position, 0.02f);

				ImGui::DragFloat3("Rotation", vec4_Rotation, 0.2f, -360.0f, 360.0f);

				ImGui::DragFloat3("Scaling", vec4_Scaling, 0.02f);

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

				static char buf1[128] = u8"";

				ImGui::InputText(u8"あいうえお", buf1, 128);

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

			/*// レンダリングテクスチャ
			{
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

				ImGui::SetWindowSize(ImVec2(737, 445), ImGuiCond_Always);

				ImGui::Begin("RenderTexture", NULL, window_flags);
				//ImGui::Begin("RenderTexture");

				ImTextureID image = CRenderer::Get_SRV();

				ImGui::Image(image, ImVec2(480 * 1.5f, 270 * 1.5f));

				ImGui::End();
			}*/

			/*list<GAME_OBJECT*> object_list = SCENE::Get();

			ImGui::Begin("size");
			ImGui::Text("%d", object_list.size());
			ImGui::End();

			for (GAME_OBJECT* object : object_list)
			{
				PLAYER* player = dynamic_cast<PLAYER*>(object);

				if (nullptr != player)
				{
					XMFLOAT3 vecF;
					XMStoreFloat3(&vecF, player->Get_Front());

					XMFLOAT3 vecU;
					XMStoreFloat3(&vecU, player->Get_Up());

					XMFLOAT3 vecR;
					XMStoreFloat3(&vecR, player->Get_Right());

					ImGui::Begin("Player");

					ImGui::Text("Front : x = %.2f y = %.2f z = %.2f", vecF.x, vecF.y, vecF.z);
					ImGui::Text("Up : x = %.2f y = %.2f z = %.2f", vecU.x, vecU.y, vecU.z);
					ImGui::Text("Right : x = %.2f y = %.2f z = %.2f", vecR.x, vecR.y, vecR.z);

					ImGui::End();
				}
				
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