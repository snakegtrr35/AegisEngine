#ifdef _DEBUG

#include "Scene.h"
#include "manager.h"
#include "ShadowMap.h"
#include "texture.h"
#include "TextureManager.h"
#include "Model_Manager.h"
#include "Component.h"
#include "Component_Manager.h"
#include "Light.h"
#include "camera.h"
#include "Debug_Camera.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"

#include "Bounding.h"
#include "Bounding_Aabb.h"
#include "Bounding_Obb.h"
#include "Bounding_Sphere.h"
#include "Bounding_Capsule.h"



#include "include\engine\core\memory\aegisAllocator.h"
#include "../AegisEngine/Renderer.h"

#ifdef DX11
#include "../AegisEngine/main.h"
#include "../AegisEngine/RenderDX11.h"
#elif DX12
// 未実装
#elif VULKAN
// 未実装
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"

#ifdef DX11
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#elif DX12
// 未実装
#elif VULKAN
// 未実装
#endif

#include "imgui//IconsFontAwesome5_c.h"

#include "imgui/ImGuizmo.h"

ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

#include "My_imgui.h"

#include "../AegisEngine/GlyphRangesJapanese.h"

#include <psapi.h>

#include "Timer.h"

namespace aegis
{
	namespace Details
	{
		void InitDevice()
		{
#ifdef DX11
			ImGui_ImplWin32_Init(GetWindow());
			auto render = static_cast<RenderDX11*>(CRenderer::getInstance());
			ImGui_ImplDX11_Init(render->GetDevice(), render->GetDeviceContext());
#elif DX12
			// 未実装
#elif VULKAN
			// 未実装
#endif
		}

		void Uninit()
		{
#ifdef DX11
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
#elif DX12
			// 未実装
#elif VULKAN
			// 未実装
#endif
		}

		void Begin()
		{
#ifdef DX11
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
#elif DX12
			// 未実装
#elif VULKAN
			// 未実装
#endif
		}

		void Render()
		{
#ifdef DX11
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif DX12
			// 未実装
#elif VULKAN
			// 未実装
#endif
		}
	}

	static aegis::string old_name;
	
	void EditTransform(const float32* cameraView, float32* cameraProjection, float32* matrix, bool enable, GameObject* object);
	
	template <typename ... Args>
	aegis::string format(const aegis::string& fmt, Args ... args)
	{
		uint64 len = std::snprintf(nullptr, 0, fmt.c_str(), args ...);
		aegis::vector<char> buf(len + 1);
		std::snprintf(&buf[0], len + 1, fmt.c_str(), args ...);
		return aegis::string(&buf[0]/*, &buf[0] + len*/);
	}
	
	void printFile(const aegis::string& path)
	{
		std::wstring wstr;
		aegis::string str;

		for (const std::filesystem::directory_entry& i : std::filesystem::directory_iterator(path)) {
			if (i.is_directory()) {

				//wstr = stringTowstring(i.path().filename().aegis::string());

				str = format((char*)u8"%s %s", ICON_FA_FOLDER, i.path().filename().string().c_str());

				if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Bullet))
				{
					printFile(i.path().string().c_str());
					ImGui::TreePop();
				}
			}
			else
			{
				str = format((char*)u8"%s %s", ICON_FA_FILE, i.path().filename().string().c_str());

				if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Leaf))
				{
					ImGui::PushID(str.c_str());
					{
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
						{
							ImGui::SetDragDropPayload("DND_DEMO_CELL", i.path().filename().string().c_str(), sizeof(char*) * i.path().filename().string().size());    // Set payload to carry the index of our item (could be anything)
							{
								ImGui::Text((char*)u8"%s", str.c_str());
							}
							ImGui::EndDragDropSource();
						}

						ImGui::TreePop();
					}
					ImGui::PopID();
				}
			}
		}

	}
	
	
	void My_imgui::Init(HWND hWnd)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;  // Enable Keyboard Controls

		// u8を使えば日本語の表示はできる
		ImFontConfig config;

		// Load a first font
		//ImFont* font = io.Fonts->AddFontDefault();

		// Add character ranges and merge into the previous font
		// The ranges array is not copied by the AddFont* functions and is used lazily
		// so ensure it is available at the time of building or calling GetTexDataAsRGBA32().
		config.MergeMode = false;
		io.Fonts->AddFontFromFileTTF("./asset/font/Gidole-Regular.ttf", 15.0f, &config, io.Fonts->GetGlyphRangesDefault());
		config.MergeMode = true;
		io.Fonts->AddFontFromFileTTF("./asset/font/meiryo.ttc", 16.0f, &config, glyphRangesJapanese);

		///
		config.PixelSnapH = true;
		ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		//io.Fonts->AddFontFromFileTTF("./asset/font/fa-regular-400.ttf", 13.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("./asset/font/Font Awesome 5 Free-Solid-900.otf", 13.0f, &config, icon_ranges);
		///



		io.Fonts->Build();

		{
			CRenderer* render = CRenderer::getInstance();

			// Setup Platform/Renderer bindings
			ImGui_ImplWin32_Init(hWnd);
			///ImGui_ImplDX11_Init(render->GetDevice(), render->GetDeviceContext());

			Details::InitDevice();
		}

		// Setup Style
		ImGui::StyleColorsDark();

		ImVec4 color = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);

		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_WindowBg] = color;

		{
			Component_Items.first.emplace_back((char*)u8"コリジョン AABB");
			Component_Items.second.emplace_back("コリジョン AABB");

			Component_Items.first.emplace_back((char*)u8"コリジョン OBB");
			Component_Items.second.emplace_back("コリジョン OBB");

			Component_Items.first.emplace_back((char*)u8"コリジョン 球");
			Component_Items.second.emplace_back("コリジョン 球");

			Component_Items.first.emplace_back((char*)u8"コリジョン カプセル");
			Component_Items.second.emplace_back("コリジョン カプセル");

			Component_Items.first.emplace_back((char*)u8"EEEE");
			Component_Items.second.emplace_back("EEEE");

			Component_Items.first.emplace_back((char*)u8"FFFF");
			Component_Items.second.emplace_back("FFFF");
		}
	}
	
	void My_imgui::Draw(void)
	{
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_default_window)
		{
			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::Text("Application average %.6f ms/frame (%.1f FPS)", (1000.0f / ImGui::GetIO().Framerate) /** 0.001f*/, ImGui::GetIO().Framerate);

			ImGui::Text("x = %f", ImGui::GetMousePos().x);
			ImGui::Text("y = %f", ImGui::GetMousePos().y);

			ImGui::End();
		}

		if (show_another_window)
		{
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

					if (ImGui::BeginMenu("Import"))
					{
						{
							if (ImGui::MenuItem("Texture"))
							{
								Texture_Import_Enable = true;
							}
							if (ImGui::MenuItem("Model"))
							{
								Model_Import_Enable = true;
							}
						}

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Delete"))
					{
						{
							if (ImGui::MenuItem("Texture"))
							{
								Texture_Delete_Enable = true;
							}
							if (ImGui::MenuItem("model"))
							{
								Model_Delete_Enable = true;
							}
						}

						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Close"))
					{
						CManager::Get_Instance()->GameEnd();
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

					ImGui::Checkbox("Default Window", &show_default_window);

					ImGui::Checkbox("Debug Draw Enable", &Debug_Draw_Enable);
					ImGui::SameLine(); HelpMarker((char*)u8"デバッグ表示　有効無効\n");

					ImGui::MenuItem("Setting", NULL, &Setting_Enable);

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		if (show_app_style_editor)
		{
			ImGui::Begin("Style Editor", &show_app_style_editor, ImGuiWindowFlags_NoResize);

			ImGuiStyle style = ImGui::GetStyle();

			ImGui::ShowStyleEditor(&style);
			ImGui::End();
		}

		// ドッキングスペース
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		static aegis::string s = GameObject::Get_Object_Name_Map().begin()->c_str();

		old_name = s;

		{
			// ライトの設定
			{
				CRenderer* render = CRenderer::getInstance();

				ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoResize;

				LIGHT* light = render->GetLight();

				float32 vec4_Direction[] = { light->Direction.x, light->Direction.y, light->Direction.z, light->Direction.w };
				float32 vec4_Position[] = { light->Position.x, light->Position.y, light->Position.z, light->Position.w };
				float32 vec4_Diffuse[] = { light->Diffuse.r, light->Diffuse.g, light->Diffuse.b, light->Diffuse.a };
				float32 vec4_Ambient[] = { light->Ambient.r, light->Ambient.g, light->Ambient.b, light->Ambient.a };
				float32 vec4_Specular[] = { light->Specular.r, light->Specular.g, light->Specular.b, light->Specular.a };

				ImGui::Begin("Directional Light", nullptr/*, window_flag*/);

				ImGui::DragFloat4("Direction", vec4_Direction, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光\" 向き\n");

				ImGui::DragFloat4("Position", vec4_Position, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光源の位置\"");

				ImGui::DragFloat4("Diffuse", vec4_Diffuse, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光\" 拡散(直接)光\n");

				ImGui::DragFloat4("Ambient", vec4_Ambient, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光\" 環境光\n");

				ImGui::DragFloat4("Specular", vec4_Specular, 0.01f);
				ImGui::SameLine(); HelpMarker((char*)u8"\"平行光\" 鏡面光\n");

				ImGui::End();

				// ライトの設定
				light->Direction = Vector4(vec4_Direction[0], vec4_Direction[1], vec4_Direction[2], vec4_Direction[3]);
				light->Position = Vector4(vec4_Position[0], vec4_Position[1], vec4_Position[2], vec4_Position[3]);
				light->Diffuse = COLOR(vec4_Diffuse[0], vec4_Diffuse[1], vec4_Diffuse[2], vec4_Diffuse[3]);
				light->Ambient = COLOR(vec4_Ambient[0], vec4_Ambient[1], vec4_Ambient[2], vec4_Ambient[3]);
				light->Specular = COLOR(vec4_Specular[0], vec4_Specular[1], vec4_Specular[2], vec4_Specular[3]);
				render->SetLight(light);
			}

			// レンダリングテクスチャ
			{
				// タイトルバーありの場合、imageyよりWindowSizeがImVec2(17, 40)分大きければ丁度いい
				// タイトルバーなしの場合、imageyよりWindowSizeがImVec2(13, 16)分大きければ丁度いい

				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

				{
					ImGui::SetNextWindowSize(ImVec2(512 + 17, 512 + 40), ImGuiCond_Once);

					ImGui::Begin("Shadow", nullptr, window_flags);

					ImTextureID image = aegis::Cast(CManager::Get_Instance()->Get_ShadowMap()->Get());

					ImGui::Image(image, ImVec2(512, 512));

					ImGui::End();
				}
			}

			// オブジェクト一覧
			{
				ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoTitleBar;
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				auto map = GameObject::Get_Object_Name_Map();

				aegis::vector<aegis::string> Object_Name_List;
				Object_Name_List.reserve(map.size());

				for (auto object : map)
				{
					Object_Name_List.emplace_back(object);
				}

				sort(Object_Name_List.begin(), Object_Name_List.end());

				ImGui::Begin("World", nullptr, window_flag);

				ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

				{
					int32 i = 0;
					for (auto object : Object_Name_List)
					{
						// マウスボタン : 0 = left, 1 = right, 2 = middle + extras
						aegis::string str = object.c_str();

						node_flags |= /*ImGuiTreeNodeFlags_Leaf |*/ ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
						ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, str.c_str());

						if (ImGui::IsMouseClicked(0))
						{
							if (ImGui::IsItemClicked(0))
							{
								s = str.c_str();
							}
						}

						if (ImGui::IsMouseClicked(1))
						{
							if (ImGui::IsItemClicked(1))
							{
								s = str.c_str();
							}
						}

						i++;
					}
				}

				ImGui::End();

			}
		}

		{
			const std::weak_ptr<DEBUG_CAMERA> camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

			ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);

			ImGui::Begin("Test", nullptr, window_flags);

			{
				ImGuiIO& io = ImGui::GetIO();

				if (io.WantCaptureMouse)
				{
					ImGui::Text((char*)u8"true");
				}
				else
				{
					ImGui::Text((char*)u8"false");

				}

				auto p = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<PLAYER>("player");

				if (!p.expired())
				{
					("HP  %f", p.lock()->Get_HP());
				}
			}

			{
				///ImGui::Text((char*)u8"GPUメモリ使用量 %d byte", info.CurrentUsage);
				///
				///ImGui::Text((char*)u8"1 %d キロバイト", info.CurrentUsage / 1000);
				///
				///ImGui::Text((char*)u8"1 %d メガバイト", info.CurrentUsage / 1000 / 1000);
			}

			{
				HANDLE hProc = GetCurrentProcess();

				PROCESS_MEMORY_COUNTERS_EX pmc;

				BOOL isSuccess = GetProcessMemoryInfo(hProc, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

				if (isSuccess == FALSE)
				{
					CloseHandle(hProc);
				}
				else
				{
					ImGui::Text((char*)u8"メモリ使用量 %d byte", pmc.PrivateUsage);

					ImGui::Text((char*)u8"1 メモリ使用量 %.3f キロバイト", float32(pmc.PrivateUsage / 1000.0f));

					ImGui::Text((char*)u8"2 メモリ使用量 %.3f メガバイト", float32(pmc.PrivateUsage / 1000.0f / 1000.0f));

					for (int i = 0; i < (int)aegis::memory::AllocatorType::Max; i++)
					{
						aegis::string type;
						switch (aegis::memory::AllocatorType(i))
						{
							case aegis::memory::AllocatorType::Boot:
								type = "Boot";
								break;

							case aegis::memory::AllocatorType::Default:
								type = "Default";
								break;

							case aegis::memory::AllocatorType::Resource:
								type = "Resource";
								break;

							case aegis::memory::AllocatorType::Temp:
								type = "Temp";
								break;

							case aegis::memory::AllocatorType::Develop:
								type = "Develop";
								break;
						}
						ImGui::Text((char*)u8"2 メモリ使用量[ アロケーター: %s ] %lu バイト", type.c_str(), aegis::memory::AegisAllocator::getTotalSize(aegis::memory::AllocatorType(i)));
					}

					CloseHandle(hProc);
				}
			}

			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImVec2 pos = ImGui::GetCurrentWindow()->DC.CursorPos;
				{
					ImGui::DrawRect(ImVec2(100, 100), ImVec4(1.0f, 0.f, 0.f, 1.0f), "test", ImVec4(0.f, 0.f, 0.f, 1.0f), 3.0f);
				}

				ImGui::SameLine();

				{
					ImGui::DrawRect(ImVec2(100, 100), ImVec4(0.f, 0.f, 1.0f, 1.0f), "test", ImVec4(0.f, 0.f, 0.f, 1.0f), 3.0f);
				}

				ImGui::SameLine();

				{
					ImGui::DrawRect(ImVec2(100, 100), ImVec4(1.0f, 1.0f, 0.f, 1.0f), "test", ImVec4(0.f, 0.f, 0.f, 1.0f), 3.0f);
				}
			}

			{
				ImGui::Text((char*)u8"平均FPS %.3lf", aegis::Timer::Get_FPS());
			}

			ImGui::End();
		}

		// Drag Test
		{
			static aegis::vector<aegis::string> names = { "Bobby", "Beatrice", "Betty", "Brianna", "Barry", "Bernard", "Bibi", "Blaine", "Bryn" };

			static int32 cnt = 0;

			if (ImGui::Begin("Drag Test"))
			{
				enum Mode
				{
					Mode_Copy,
					Mode_Move,
					Mode_Swap
				};

				auto tex_file = TextureManager::Get_Instance()->Get_TextureFile().begin();

				TEXTURE tex;

				for (cnt = 0; cnt < 9; cnt++)
				{
					ImGui::PushID(cnt);
					{
						if ((cnt % 3) != 0)
							ImGui::SameLine();

						uint64 pos = tex_file->second.Path.find_last_of("/");

						aegis::string tex_name = tex_file->second.Path.substr(pos + 1);

						ImGui::BeginGroup();
						{
							{
								auto hash = std::hash<aegis::string>()(tex_name);

								ImTextureID image = aegis::Cast(TextureManager::Get_Instance()->GetShaderResourceView(hash));

								auto wh = TextureManager::Get_Instance()->Get_WH(hash);

								//ImVec2 size =ImVec2(wh->x / 5.0f, wh->y / 5.0f);
								const ImVec2 size = ImVec2(64.0f, 64.0f);

								//ImGui::ImageButton(image, size);

								if ((tex_name.size() * 4.0f) < (size.x * 0.6f))
								{
									ImGui::ImageButton(image, size);

									ImGui::Indent(size.x * 0.6f - tex_name.size() * 4.0f);
								}
								else
								{
									//ImGui::Indent((tex_name.size() * 4.0f - size.x) * 0.5f);

									ImGui::ImageButton(image, size);

									//ImGui::Indent(tex_name.size() * 4.0f - size.x );
									//ImGui::Indent(size.x * 0.6f - tex_name.size() * 4.0f);
								}
							}

							// Our buttons are both drag sources and drag targets here!
							if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
							{
								ImGui::SetDragDropPayload("DND_DEMO_CELL", &cnt, sizeof(int32));    // Set payload to carry the index of our item (could be anything)
								//{ ImGui::Text("%s", names[cnt].c_str()); }
								{
									ImGui::Text("%s", tex_name.c_str());
								}
								ImGui::EndDragDropSource();
							}

							ImGui::Text("%s", tex_name.c_str());

							ImGui::Spacing();

						}
						ImGui::EndGroup();

						tex_file++;
					}
					ImGui::PopID();
				}

				ImGui::End();
			}

			if (ImGui::Begin("Drop Test"))
			{
				static aegis::vector<aegis::string> names_temp = { "Bobby", "Beatrice", "Betty", "Brianna", "Barry", "Bernard", "Bibi", "Blaine", "Bryn" };

				enum Mode
				{
					Mode_Copy,
					Mode_Move,
					Mode_Swap
				};
				static int32 mode = 0;
				if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
				if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
				if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }

				auto start = TextureManager::Get_Instance()->Get_TextureFile().begin();

				for (cnt = 0; cnt < names_temp.size(); cnt++)
				{
					ImGui::PushID(cnt);
					if ((cnt % 3) != 0)
						ImGui::SameLine();
					ImGui::Button(names_temp[cnt].c_str(), ImVec2(names_temp[cnt].size() * 8.0f, 60));

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
						{
							//IM_ASSERT(payload->DataSize == sizeof(int32));
							//IM_ASSERT(payload->DataSize == sizeof(char*));
							//int32 payload_n = *(const int32*)payload->Data;

							aegis::string str = reinterpret_cast<char*>(payload->Data);

							if (mode == Mode_Copy)
							{
								uint64 pos = start->second.Path.find_last_of("/");

								aegis::string tex_name = start->second.Path.substr(pos + 1);

								//names_temp[cnt] = tex_name;
								names_temp[cnt] = str.c_str();
							}
							/*if (mode == Mode_Move)
							{
								names_temp[cnt] = names[payload_n];
								names[payload_n] = "";
							}*/
							/*if (mode == Mode_Swap)
							{
								const auto tmp = names[cnt];
								names[cnt] = names[payload_n];
								names[payload_n] = tmp;
							}*/
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopID();

					start++;
				}

				ImGui::End();
			}
		}

		{
			ImGuiWindowFlags flag = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

			ImVec2 size(150.0f, 50.f);
			ImVec2 pos(SCREEN_WIDTH * 0.5f - size.x * 0.5f, 20.0f);

			ImGui::SetNextWindowSize(size);
			ImGui::SetNextWindowPos(pos);

			ImGui::Begin("PLAY", nullptr, flag);

			static int32 radio = 1;
			ImGui::RadioButton("PLAT", &radio, 1); ImGui::SameLine();
			ImGui::RadioButton("STOP", &radio, 0);

			radio ? CManager::Get_Instance()->Set_Play_Enable(true) : CManager::Get_Instance()->Set_Play_Enable(false);

			ImGui::End();
		}

		{
			// タイトルバーありの場合、imageyよりWindowSizeがImVec2(17, 40)分大きければ丁度いい
			// タイトルバーなしの場合、imageyよりWindowSizeがImVec2(13, 16)分大きければ丁度いい

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

			{
				ImGui::Begin("folder_tree");

				printFile("asset");

				ImGui::Text((char*)u8"メモリ使用量");

				ImGui::Text((char*)u8"%s %s %s %s", ICON_FA_COG, ICON_FA_COGS, ICON_FA_COINS, ICON_FA_COLUMNS);

				ImGui::End();
			}

			// ライトの設定(ディレクショナルライトではない)
			{
				Light_Setting();
			}

			// インスペクター
			{
				Draw_Inspector(s);
			}

			// テクスチャにインポート
			Texture_Import();

			// テクスチャの削除
			Texture_Delete();

			// モデルにインポート
			Model_Import();

			// モデルの削除
			Model_Delete();

			Setting();

			File();
		}
	}
	
	void My_imgui::Update(void)
	{
		{
			if (ImGui::GetIO().WantCaptureMouse)
			{
				Mouse_Over_Enable = true;
			}
			else
			{
				Mouse_Over_Enable = false;
			}
		}
	}
	
	void My_imgui::Uninit(void)
	{
		Details::Uninit();
		ImGui::DestroyContext();

		{
			std::pair< aegis::vector<aegis::string>, aegis::vector<aegis::string> > temp;

			temp.swap(Component_Items);
		}
	}
	
	void My_imgui::Begin()
	{
		Details::Begin();
		ImGui::NewFrame();

		ImGuizmo::BeginFrame();
	}
	
	void My_imgui::End()
	{
		// Rendering
		ImGui::Render();
		ImGui::EndFrame();
	}
	
	void My_imgui::Render(void)
	{
		Details::Render();
	}
	
	
	
	void My_imgui::Draw_Inspector(const aegis::string& name)
	{
		//static const char* item_current = nullptr;
		//static const char* temp_current = nullptr;

		static std::unique_ptr<const char> item_current;
		static std::unique_ptr<const char> temp_current;

		bool flag = false;
		if (old_name != name)
		{
			flag = true;

			item_current = nullptr;
			temp_current = nullptr;
		}

		auto object = SCENE::Get_Game_Object(name);

		if (nullptr != object)
		{
			ImGui::Begin("Inspector");

			ImGui::Text(object->Get_Object_Name().c_str());

			static bool enable = true;
			ImGui::Checkbox("Enable", &enable);

			// 3Dギズモ
			{
				XMMATRIX mtr;

				const auto camera1 = SCENE::Get_Game_Object<CCamera>("camera");
				const auto camera2 = SCENE::Get_Game_Object<DEBUG_CAMERA>("camera");

				{
					if (!camera1.expired() && Empty_weak_ptr<CCamera>(camera1))
					{
						mtr = camera1.lock()->Get_Camera_View();
					}
					else
					{
						mtr = camera2.lock()->Get_Camera_View();
					}
				}

				XMFLOAT4X4 mat44;
				XMStoreFloat4x4(&mat44, mtr);

				float32 view[16] = { mat44._11, mat44._12, mat44._13,mat44._14,
									mat44._21, mat44._22, mat44._23,mat44._24,
									mat44._31, mat44._32, mat44._33,mat44._34,
									mat44._41, mat44._42, mat44._43,mat44._44
				};

				{
					if (!camera1.expired() && Empty_weak_ptr<CCamera>(camera1))
					{
						mtr = camera1.lock()->Get_Camera_Projection();
					}
					else
					{
						mtr = camera2.lock()->Get_Camera_Projection();
					}
				}

				XMStoreFloat4x4(&mat44, mtr);

				float32 pro[16] = { mat44._11, mat44._12, mat44._13,mat44._14,
									mat44._21, mat44._22, mat44._23,mat44._24,
									mat44._31, mat44._32, mat44._33,mat44._34,
									mat44._41, mat44._42, mat44._43,mat44._44
				};

				mtr = XMMatrixTranslation(object->Get_Transform().Get_Position().x, object->Get_Transform().Get_Position().y, object->Get_Transform().Get_Position().z);
				XMStoreFloat4x4(&mat44, mtr);

				static float32 pos[16] = { mat44._11, mat44._12, mat44._13,mat44._14,
										mat44._21, mat44._22, mat44._23,mat44._24,
										mat44._31, mat44._32, mat44._33,mat44._34,
										mat44._41, mat44._42, mat44._43,mat44._44
				};

				if (flag)
				{
					mtr = XMMatrixScaling(object->Get_Transform().Get_Scaling().x, object->Get_Transform().Get_Scaling().y, object->Get_Transform().Get_Scaling().z);
					mtr *= XMMatrixTranslation(object->Get_Transform().Get_Position().x, object->Get_Transform().Get_Position().y, object->Get_Transform().Get_Position().z);
					XMStoreFloat4x4(&mat44, mtr);

					pos[0] = mat44._11, pos[1] = mat44._12, pos[2] = mat44._13, pos[3] = mat44._14;
					pos[4] = mat44._21, pos[5] = mat44._22, pos[6] = mat44._23, pos[7] = mat44._24;
					pos[8] = mat44._31, pos[9] = mat44._32, pos[10] = mat44._33, pos[11] = mat44._34;
					pos[12] = mat44._41, pos[13] = mat44._42, pos[14] = mat44._43, pos[15] = mat44._44;

				}

				EditTransform(view, pro, pos, enable, object);
			}

			{
				{
					if (ImGui::BeginCombo((char*)u8"コンポーネント", item_current.get(), ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
					{
						for (int32 n = 0; n < Component_Items.first.size(); n++)
						{
							bool is_selected = (item_current.get() == Component_Items.first.at(n).c_str());
							//if (ImGui::Selectable(Component_Items.first.at(n), is_selected))
							if (ImGui::Selectable(Component_Items.first.at(n).c_str(), is_selected))
							{
								//item_current = Component_Items.first.at(n).c_str();
								//temp_current = Component_Items.second.at(n).c_str();

								item_current.reset(Component_Items.first.at(n).c_str());
								temp_current.reset(Component_Items.second.at(n).c_str());
							}
						}
						ImGui::EndCombo();
					}
				}

				{
					Draw_Components(object->GetComponents());
				}

				if (ImGui::Button("Add Component"))
				{
					if (nullptr != temp_current)
					{
						aegis::string s(temp_current.get());

						Add_Component(object, s);
					}
				}

				if (ImGui::Button("Delete Component"))
				{
					if (nullptr != temp_current)
					{
						aegis::string s(temp_current.get());

						Delete_Component(object, s);
					}
				}
			}

			ImGui::End();
		}
	}
	
	void EditTransform(const float32* cameraView, float32* cameraProjection, float32* matrix, bool enable, GameObject* object)
	{
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		float32 snap[3] = { 0.001f, 0.001f, 0.001f };

		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		{
			aegis::Transform transform = object->Get_Transform();

			std::array<float32, 3> Translation{ transform.Get_Position().x, transform.Get_Position().y, transform.Get_Position().z };
			std::array<float32, 3>  Rotation = { transform.Get_Rotation().x, transform.Get_Rotation().y, transform.Get_Rotation().z };
			float32 R[3] = { transform.Get_Rotation().x, transform.Get_Rotation().y, transform.Get_Rotation().z };
			std::array<float32, 3>  Scale = { transform.Get_Scaling().x, transform.Get_Scaling().y, transform.Get_Scaling().z };

			//ImGuizmo::DecomposeMatrixToComponents(matrix, Translation, Rotation, Scale);

			bool isTransEdit = ImGui::DragFloat3((char*)u8"トランスフォーム", Translation.data(), 0.01f);
			bool isRotateEdit = ImGui::DragFloat3((char*)u8"回転", Rotation.data(), 0.01f);
			bool isScaleEdit = ImGui::DragFloat3((char*)u8"スケール", Scale.data(), 0.01f);

			transform.Set_Position(reinterpret_cast<Vector3*>(Translation.data()));
			transform.Set_Rotation(reinterpret_cast<Vector3*>(Rotation.data()));
			transform.Set_Scaling(reinterpret_cast<Vector3*>(Scale.data()));

			Vector3 pos = transform.Get_Position();
			Vector3 rotate = transform.Get_Rotation();
			Vector3 scale = transform.Get_Scaling();

			for (int i = 0; i < 3; i++)
			{
				Translation[i] = pos[i];
				Rotation[i] = rotate[i];
				Scale[i] = scale[i];
			}

			const std::array<float32, 3> Translation_Ans{ transform.Get_Position().x, transform.Get_Position().y, transform.Get_Position().z };
			const std::array<float32, 3>  Rotation_Ans = { transform.Get_Rotation().x, transform.Get_Rotation().y, transform.Get_Rotation().z };
			const std::array<float32, 3>  Scale_Ans = { transform.Get_Scaling().x, transform.Get_Scaling().y, transform.Get_Scaling().z };

			ImGuizmo::DecomposeMatrixToComponents(matrix, Translation.data(), Rotation.data(), Scale.data());


			//ImGuizmo::RecomposeMatrixFromComponents(Translation.data(), Rotation.data(), Scale.data(), matrix);

			switch (mCurrentGizmoOperation)
			{
				case ImGuizmo::TRANSLATE:
				{
					if (Translation_Ans != Translation
						&& !isTransEdit)
					{
						Vector3 vec(Translation[0], Translation[1], Translation[2]);
						transform.Set_Position(vec);
					}
				}
				break;
				case ImGuizmo::ROTATE:
				{
					if (Rotation_Ans != Rotation
						&& !isRotateEdit)
					{
						Vector3 vec(Rotation[0], Rotation[1], Rotation[2]);
						transform.Set_Rotation(vec);
					}
				}
				break;
				case ImGuizmo::SCALE:
				{
					if (Scale_Ans != Scale
						&& !isScaleEdit)
					{
						Vector3 vec(Scale[0], Scale[1], Scale[2]);
						transform.Set_Scaling(vec);
					}
				}
				break;
				default:
					break;
			}

			ImGuizmo::RecomposeMatrixFromComponents(Translation.data(), Rotation.data(), Scale.data(), matrix);

			//transform.Set_Position(reinterpret_cast<Vector3*>(Translation.data()));
			//transform.Set_Rotation(reinterpret_cast<Vector3*>(Rotation.data()));
			//transform.Set_Scaling(reinterpret_cast<Vector3*>(Scale.data()));

			if (enable)
			{
				//Vector3 vec1(Translation[0], Translation[1], Translation[2]);
				Vector3 vec2/*(Rotation[0], Rotation[1], Rotation[2])*/;
				//Vector3 vec3(Scale[0], Scale[1], Scale[2]);

				//transform.Set_Position(vec1);
				//object->Set_Rotation(vec2);
				//transform.Set_Scaling(vec3);

				ImGui::DragFloat3("Rotation", R, 0.2f, -180.f, 180.f);
				vec2 = Vector3(R[0], R[1], R[2]);
				transform.Set_Rotation(vec2);
			}
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, ImGuizmo::LOCAL, matrix, NULL, &snap[0], NULL, NULL);
	}
	
	void My_imgui::Draw_Components(aegis::vector< std::weak_ptr<COMPONENT> >* components)
	{
		ImGui::Spacing();
		ImGui::Text((char*)u8"コンポーネント");
		ImGui::Spacing();

		for (const auto& com : *components)
		{
			ImGui::Spacing();
			com.lock()->Draw_Inspector();
			ImGui::Spacing();
		}
	}
	
	void My_imgui::Texture_Import()
	{
		static bool flag = false;
		static bool flag2 = false;

		static aegis::string file_name;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

		if (Texture_Import_Enable)
		{
			static char check;

			ImGui::SetNextWindowSize(ImVec2(360, 167), ImGuiCond_Appearing);

			if (ImGui::Begin((char*)u8"テクスチャ インポート", &Texture_Import_Enable, window_flags))
			{
				ImGui::Indent(15.0f);

				ImGui::InputText((char*)u8"テクスチャ名", reinterpret_cast<std::string*>(&file_name));
				ImGui::SameLine(); HelpMarker((char*)u8"対応フォーマットは 'png' 'jpg''dds'");

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Indent(100);

				ImVec2 size(100, 40);

				if (ImGui::Button((char*)u8"インポート", size))
				{
					check = Texture_File_Check(file_name);

					if (1 == check)
					{
						TextureManager::Get_Instance()->Add(file_name);

						ImGui::Text((char*)u8"テクスチャが読み込まれました");
					}
					else
					{
						flag2 = true;
					}
				}
				ImGui::End();
			}

			if (flag2)
			{
				ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.5f - 120.0f, SCREEN_HEIGHT * 0.5f - 55.0f), ImGuiCond_Appearing);

				if (ImGui::Begin((char*)u8"エラー", &flag2, window_flags))
				{
					ImGui::Indent(20.0f);

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					switch (check)
					{
						case -1:
							ImGui::Text((char*)u8"テクスチャ名が入力されてないです");
							break;

						case -2:
							ImGui::Text((char*)u8"既に読み込んでいるテクスチャです");
							break;

						case -3:
							ImGui::Text((char*)u8"テクスチャが存在しないです");
							break;

						default:
							break;
					}

					ImGui::End();
				}
			}

			flag = true;
		}
		else
		{
			if (flag)
			{
				file_name = "";

				flag = false;
			}

			flag2 = false;
		}
	}
	
	const char My_imgui::Texture_File_Check(const aegis::string& file_name)
	{
		if (file_name.empty())
		{
			// テクスチャ名が入力されていない
			return -1;
		}

		uint64 file = std::hash<aegis::string>()(file_name);//

		if (TextureManager::Get_Instance()->Get_TextureFile().find(file) != TextureManager::Get_Instance()->Get_TextureFile().end())
		{
			// 既に読み込んでいるテクスチャ
			return -2;
		}

		// ファイルがあるかの判定
		{
			aegis::string path = "./asset/texture/";

			path += file_name;

			bool flag = std::filesystem::exists(path);
			if (false == flag)
			{
				// ファイルが存在しない
				return -3;
			}
		}

		return 1;
	}
	
	void My_imgui::Texture_Delete()
	{
		static bool flag = true;
		static bool flag2 = false;

		static aegis::string file_name;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

		if (Texture_Delete_Enable)
		{
			static char check;

			ImGui::SetNextWindowSize(ImVec2(360, 167), ImGuiCond_Appearing);

			if (ImGui::Begin((char*)u8"テクスチャ 削除", &Texture_Delete_Enable, window_flags))
			{
				ImGui::Indent(15.0f);

				ImGui::InputText((char*)u8"テクスチャ名", reinterpret_cast<std::string*>(&file_name));

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Indent(100);

				ImVec2 size(100, 40);

				if (ImGui::Button((char*)u8"削除", size))
				{
					check = Texture_File_Check(file_name);

					if (-1 != check)
					{
						TextureManager::Get_Instance()->Unload(file_name);

						ImGui::Text((char*)u8"テクスチャが削除されました");
					}
					else
					{
						flag2 = true;
					}
				}
				ImGui::End();
			}

			if (flag2)
			{
				ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.5f - 120.0f, SCREEN_HEIGHT * 0.5f - 55.0f), ImGuiCond_Appearing);

				if (ImGui::Begin((char*)u8"エラー", &flag2, window_flags))
				{
					ImGui::Indent(20.0f);

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					switch (check)
					{
						case -1:
							ImGui::Text((char*)u8"テクスチャ名が入力されてないです");
							break;

						case -3:
							ImGui::Text((char*)u8"テクスチャが存在しないです");
							break;

						default:
							break;
					}
					ImGui::End();
				}
			}

			flag = true;
		}
		else
		{
			if (flag)
			{
				file_name.clear();

				flag = false;
			}

			flag2 = false;
		}
	}
	
	void My_imgui::Model_Import()
	{
		static bool flag = false;
		static bool flag2 = false;

		static aegis::string file_name;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

		if (Model_Import_Enable)
		{
			static char check;

			ImGui::SetNextWindowSize(ImVec2(360, 167), ImGuiCond_Appearing);

			ImGui::Begin((char*)u8"モデル インポート", &Model_Import_Enable, window_flags);
			{
				ImGui::Indent(15.0f);

				ImGui::InputText((char*)u8"モデル名", reinterpret_cast<std::string*>(&file_name));
				ImGui::SameLine(); HelpMarker((char*)u8"対応フォーマットは 'fbx'");

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Indent(100);

				ImVec2 size(100, 40);

				if (ImGui::Button((char*)u8"インポート", size))
				{
					check = Model_File_Check(file_name);

					if (1 == check)
					{
						MODEL_MANEGER::Get_Instance()->Add(file_name);

						ImGui::Text((char*)u8"モデルが読み込まれました");
					}
					else
					{
						flag2 = true;
					}
				}
				ImGui::End();
			}

			if (flag2)
			{
				ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.5f - 120.0f, SCREEN_HEIGHT * 0.5f - 55.0f), ImGuiCond_Appearing);

				if (ImGui::Begin((char*)u8"エラー", &flag2, window_flags))
				{
					ImGui::Indent(20.0f);

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					switch (check)
					{
						case -1:
							ImGui::Text((char*)u8"モデル名が入力されてないです");
							break;

						case -2:
							ImGui::Text((char*)u8"既に読み込んでいるモデルです");
							break;

						case -3:
							ImGui::Text((char*)u8"モデルが存在しないです");
							break;

						default:
							break;
					}

					ImGui::End();
				}
			}

			flag = true;
		}
		else
		{
			if (flag)
			{
				file_name.clear();

				flag = false;
			}

			flag2 = false;
		}
	}
	
	const char My_imgui::Model_File_Check(const aegis::string& file_name)
	{
		if (file_name.empty())
		{
			// モデル名が入力されていない
			return -1;
		}

		uint64 file = std::hash<aegis::string>()(file_name);//

		if (MODEL_MANEGER::Get_Instance()->Get_ModelFile().find(file) != MODEL_MANEGER::Get_Instance()->Get_ModelFile().end())
		{
			// 既に読み込んでいるモデル
			return -2;
		}

		// ファイルがあるかの判定
		{
			aegis::string path = "./asset/model/";

			path += file_name;

			bool flag = std::filesystem::exists(path);
			if (false == flag)
			{
				// ファイルが存在しない
				return -3;
			}
		}

		return 1;
	}
	
	void My_imgui::Model_Delete()
	{
		static bool flag = true;
		static bool flag2 = false;

		static aegis::string file_name;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

		if (Model_Delete_Enable)
		{
			static char check;

			ImGui::SetNextWindowSize(ImVec2(360, 167), ImGuiCond_Appearing);

			ImGui::Begin((char*)u8"モデル 削除", &Model_Delete_Enable, window_flags);
			{
				ImGui::Indent(15.0f);

				ImGui::InputText((char*)u8"モデル名", reinterpret_cast<std::string*>(&file_name));

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Indent(100);

				ImVec2 size(100, 40);

				if (ImGui::Button((char*)u8"削除", size))
				{
					check = Texture_File_Check(file_name);

					if (-1 != check)
					{
						TextureManager::Get_Instance()->Unload(file_name);

						ImGui::Text((char*)u8"モデルが削除されました");
					}
					else
					{
						flag2 = true;
					}
				}
			}
			ImGui::End();

			{
				if (flag2)
				{
					ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.5f - 120.0f, SCREEN_HEIGHT * 0.5f - 55.0f), ImGuiCond_Appearing);

					ImGui::Begin((char*)u8"エラー", &flag2, window_flags);

					ImGui::Indent(20.0f);

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					switch (check)
					{
						case -1:
							ImGui::Text((char*)u8"モデル名が入力されてないです");
							break;

						case -3:
							ImGui::Text((char*)u8"モデルが存在しないです");
							break;

						default:
							break;
					}

					ImGui::End();
				}
			}

			flag = true;
		}
		else
		{
			if (flag)
			{
				file_name = "";

				flag = false;
			}

			flag2 = false;
		}
	}
	
	void My_imgui::Setting()
	{
		if (Setting_Enable)
		{
			float32 color[] = { BOUNDING::Get_Default_Color().r, BOUNDING::Get_Default_Color().g, BOUNDING::Get_Default_Color().b };

			ImGui::Begin("Setting", &Setting_Enable);

			{
				ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueWheel;

				ImGui::ColorPicker3((char*)u8"カラー", color, flag);
			}

			ImGui::End();

			COLOR c;
			c.r = color[0];
			c.g = color[1];
			c.b = color[2];
			c.a = 1.0f;

			BOUNDING::Set_Default_Color(c);
		}
	}
	
	void My_imgui::File()
	{
		///return;

		// タイトルバーありの場合、imageyよりWindowSizeがImVec2(17, 40)分大きければ丁度いい
		// タイトルバーなしの場合、imageyよりWindowSizeがImVec2(13, 16)分大きければ丁度いい

		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

		char i = 0;

		{
			//ImGui::SetNextWindowSize(ImVec2(256 + 17, 256 + 40), ImGuiCond_Once);

			ImGui::Begin("Texture");

			auto start = TextureManager::Get_Instance()->Get_TextureData_Start();
			auto end = TextureManager::Get_Instance()->Get_TextureData_End();

			for (auto tex = start; tex != end; tex++)
			{
				ImTextureID image = aegis::Cast(tex->second->ShaderResourceView.get());

				if (i < 3)
				{
					ImGui::SameLine();

					i++;
				}
				else
				{
					i = 0;

					ImGui::NewLine();
				}


				ImGui::Image(image, ImVec2(96, 96));

				ImGui::SameLine();

				ImGui::Dummy(ImVec2(10, 96));
			}

			ImGui::End();
		}
	}
	
	void My_imgui::Light_Setting()
	{
		// ライトの設定(ディレクショナルライトではない)

		auto light_manager = CManager::Get_Instance()->Get_Scene()->Get_Light_Manager();
		auto lights = light_manager->Get_Lights();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);

		ImGui::Begin("Light", nullptr, window_flags);

		if (ImGui::TreeNode("Light Tree"))
		{
			ImGuiTreeNodeFlags_ node_flags = ImGuiTreeNodeFlags_None;

			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

			for (int32 i = 0; i < lights->size(); i++)
			{
				// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.

				//if(1 == lights->at(i).Enable)
				{
					bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Light %d", i);
					if (node_open)
					{
						{
							bool flag = lights->at(i).Enable;
							float32 Position[3] = { lights->at(i).Position.x, lights->at(i).Position.y, lights->at(i).Position.z };
							float32 Color[3] = { lights->at(i).Color.r, lights->at(i).Color.g, lights->at(i).Color.b };

							UINT Type = lights->at(i).Type;//

							ImGui::Checkbox("Enable", &flag);
							ImGui::DragFloat3("Position", Position, 0.01f);
							ImGui::DragFloat3("Color", Color, 0.01f, 0.f);

							////
							{
								aegis::vector<const char*> Items;

								Items.emplace_back((char*)u8"ポイントライト");
								Items.emplace_back((char*)u8"スポットライト");
								Items.emplace_back((char*)u8"無効");


								const char* item_current;

								switch (lights->at(i).Type)
								{
									case (UINT)LIGHT_TYPE::POINT:
										item_current = Items[(UINT)LIGHT_TYPE::POINT];
										break;

									case (UINT)LIGHT_TYPE::SPOT:
										item_current = Items[(UINT)LIGHT_TYPE::SPOT];
										break;

									case (UINT)LIGHT_TYPE::NONE:
										item_current = Items[(UINT)LIGHT_TYPE::NONE];
										break;
								}

								bool is_selected = false;

								{
									if (ImGui::BeginCombo((char*)u8"ライトの種類", item_current, ImGuiComboFlags_NoArrowButton))
									{
										for (int32 n = 0; n < Items.size(); n++)
										{
											//bool is_selected = (item_current == Items[n]);
											if (ImGui::Selectable(Items[n], is_selected))
											{
												item_current = Items[n];
												is_selected = true;
											}
											//if (is_selected)
											//	ImGui::SetItemDefaultFocus();
										}
										ImGui::EndCombo();
									}
								}

								if (is_selected)
								{
									if (Items[0] == item_current)
									{
										lights->at(i).Type = (UINT)LIGHT_TYPE::POINT;
									}

									if (Items[1] == item_current)
									{
										lights->at(i).Type = (UINT)LIGHT_TYPE::SPOT;
									}

									if (Items[2] == item_current)
									{
										lights->at(i).Type = (UINT)LIGHT_TYPE::NONE;
									}
								}
							}
							////

							lights->at(i).Enable = flag;
							lights->at(i).Position = Vector3(Position[0], Position[1], Position[2]);
							lights->at(i).Color = COLOR(Color[0], Color[1], Color[2], 0.0f);
						}

						ImGui::TreePop();
					}
				}

			}

			ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
			ImGui::TreePop();
		}

		ImGui::End();
	}
	
	void My_imgui::Add_Component(GameObject* object, const aegis::string s)
	{
		ImGui::Text("%s", s.c_str());
	
		auto scene = CManager::Get_Instance()->Get_Scene();
		//auto comp = object->Get_Component();
	
		if (aegis::string::npos != s.find("AABB"))
		{
			//comp->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(object));
			object->AddComponent<BOUNDING_AABB>();
			return;
		}
	
		if (aegis::string::npos != s.find("OBB"))
		{
			//comp->Add_Component<BOUNDING_OBB>(scene->Get_Game_Object(object));
			object->AddComponent<BOUNDING_OBB>();
			return;
		}
	
		if (aegis::string::npos != s.find("球"))
		{
			//comp->Add_Component<BOUNDING_SHPERE>(scene->Get_Game_Object(object));
			object->AddComponent<BOUNDING_SHPERE>();
			return;
		}
	
		if (aegis::string::npos != s.find("カプセル"))
		{
			//comp->Add_Component<BOUNDING_CAPSULE>(scene->Get_Game_Object(object));
			object->AddComponent<BOUNDING_CAPSULE>();
			return;
		}
	}
	
	void My_imgui::Delete_Component(GameObject* object, const aegis::string s)
	{
		ImGui::Text("%s", s.c_str());
	
		auto scene = CManager::Get_Instance()->Get_Scene();
		//auto comp = object->Get_Component();
	
		COMPONENT* component = nullptr;
	
		if (aegis::string::npos != s.find("AABB"))
		{
			//component = comp->Get_Component<BOUNDING_AABB>();
			component = object->GetComponent<BOUNDING_AABB>();
		}
	
		if (aegis::string::npos != s.find("OBB"))
		{
			//component = comp->Get_Component<BOUNDING_OBB>();
			component = object->GetComponent<BOUNDING_OBB>();
		}
	
		if (aegis::string::npos != s.find("球"))
		{
			//component = comp->Get_Component<BOUNDING_SHPERE>();
			component = object->GetComponent<BOUNDING_SHPERE>();
		}
	
		if (aegis::string::npos != s.find("カプセル"))
		{
			//component = comp->Get_Component<BOUNDING_CAPSULE>();
			component = object->GetComponent<BOUNDING_CAPSULE>();
		}
	
		if(nullptr == component) return;
	
		component->SetDestroy();
	}
}

void ImGui::DrawRect(const ImVec2& size, const ImVec4& color, const char* text, const ImVec4& text_color, const aegis::float32 frame_size, const ImVec4& frame_color)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImGuiStyle& style = ImGui::GetStyle();
	const ImVec2 pos = ImGui::GetCurrentWindow()->DC.CursorPos;
	const ImRect bb(pos, pos + size);

	// 枠線の描画
	{
		const ImU32 col = ImGui::GetColorU32(frame_color);

		draw_list->AddRect(bb.Min, bb.Max, col, 0.0f, ImDrawCornerFlags_All, frame_size);
	}

	// 矩形の描画
	{
		if (nullptr == text) text = "";
		const ImVec2 label_size = ImGui::CalcTextSize(text, NULL, true);

		const ImU32 col = ImGui::GetColorU32(color);

		draw_list->AddRectFilled(bb.Min, bb.Max, col);

		const ImVec4 color = style.Colors[ImGuiCol_Text];

		style.Colors[ImGuiCol_Text] = text_color;

		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, text, NULL, &label_size, style.ButtonTextAlign, &bb);

		style.Colors[ImGuiCol_Text] = color;

		ImGui::Dummy(ImVec2(size.x - frame_size * 2.0f, size.y - frame_size * 2.0f));
	}
}

#endif // _DEBUG