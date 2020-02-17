#ifdef _DEBUG

#include	"imgui/imgui.h"
#include	"imgui/imgui_impl_dx11.h"
#include	"imgui/imgui_impl_win32.h"
#include	"imgui/imgui_stdlib.h"

#include	"My_imgui.h"

#include	"imgui/ImGuizmo.h"

#include	"Scene.h"
#include	"manager.h"
#include	"ShadowMap.h"
#include	"Texture_Manager.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"Component.h"
#include	"Component_Manager.h"
#include	"Light.h"

#include	"Enemy.h"
#include	"Bullet.h"

#include	"Bounding.h"
#include	"Bounding_Aabb.h"
#include	"Bounding_Obb.h"
#include	"Bounding_Sphere.h"

extern float radius;

static string old_name;

void EditTransform(const float* cameraView, float* cameraProjection, float* matrix, bool enable, GAME_OBJECT* object);

extern XMFLOAT3 position;
extern float t;

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

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());

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
	{
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_default_window)
		{
			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 10.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);

			if (ImGui::Button("Button"))
				counter++;

			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

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
						//if (ImGui::BeginMenu("Object"))
						{
							if (ImGui::MenuItem("Texture"))
							{
								Texture_Import_Enable = true;
							}
							if (ImGui::MenuItem("model"))
							{
								int a = 0;
							}

							//ImGui::EndMenu();
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
								int a = 0;
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
			ImGui::ShowStyleEditor();
			ImGui::End();
		}

		static string s = GAME_OBJECT::Get_Object_Name_Map().begin()->/*second.*/c_str();

		old_name = s;

		{
			// ライトの設定
			{
				static bool f = false;

				ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoResize;

				LIGHT* light = CRenderer::Get_Light();

				static float vec4_Direction[] = { light->Direction.x, light->Direction.y, light->Direction.z, light->Direction.w };
				static float vec4_Position[] = { light->Position.x, light->Position.y, light->Position.z, light->Position.w };
				static float vec4_Diffuse[] = { light->Diffuse.r, light->Diffuse.g, light->Diffuse.b, light->Diffuse.a };
				static float vec4_Ambient[] = { light->Ambient.r, light->Ambient.g, light->Ambient.b, light->Ambient.a };
				static float vec4_Specular[] = { light->Specular.r, light->Specular.g, light->Specular.b, light->Specular.a };

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
				light->Direction = XMFLOAT4(vec4_Direction[0], vec4_Direction[1], vec4_Direction[2], vec4_Direction[3]);
				light->Position = XMFLOAT4(vec4_Position[0], vec4_Position[1], vec4_Position[2], vec4_Position[3]);
				light->Diffuse = COLOR(vec4_Diffuse[0], vec4_Diffuse[1], vec4_Diffuse[2], vec4_Diffuse[3]);
				light->Ambient = COLOR(vec4_Ambient[0], vec4_Ambient[1], vec4_Ambient[2], vec4_Ambient[3]);
				light->Specular = COLOR(vec4_Specular[0], vec4_Specular[1], vec4_Specular[2], vec4_Specular[3]);
				CRenderer::SetLight(light);
			}

			// レンダリングテクスチャ
			{
				// タイトルバーありの場合、imageyよりWindowSizeがImVec2(17, 40)分大きければ丁度いい
				// タイトルバーなしの場合、imageyよりWindowSizeがImVec2(13, 16)分大きければ丁度いい

				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

				{
					ImGui::SetNextWindowSize(ImVec2(512 + 17, 512 + 40), ImGuiCond_Once);

					ImGui::Begin("Shadow", nullptr, window_flags);

					ImTextureID image = CManager::Get_Instance()->Get_ShadowMap()->Get();

					ImGui::Image(image, ImVec2(512, 512));

					ImGui::End();
				}
			}

			// オブジェクト一覧
			{
				ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoTitleBar;
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				auto map = GAME_OBJECT::Get_Object_Name_Map();

				vector<string> Object_Name_List;
				Object_Name_List.reserve(map.size());

				for (auto object : map)
				{
					Object_Name_List.emplace_back(object);
				}

				sort(Object_Name_List.begin(), Object_Name_List.end());

				ImGui::Begin("World", nullptr, window_flag);

				ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

				{
					int i = 0;
					for (auto object : Object_Name_List)
					{
						// マウスボタン : 0 = left, 1 = right, 2 = middle + extras
						string str = object.c_str();

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
			const weak_ptr<DEBUG_CAMERA> camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

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
			}

			{
				float Pos[3] = { position.x, position.y, position.z };


				ImGui::DragFloat3("Position", Pos, 0.01f);
				ImGui::DragFloat("time", &t, 0.001f, 0.f, 1.0f);

				position = XMFLOAT3(Pos[0], Pos[1], Pos[2]);
			}

			ImGui::End();
		}

		{
			ImGuiWindowFlags flag = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

			ImVec2 size(150.0f, 50.f);
			ImVec2 pos(SCREEN_WIDTH * 0.5f - size.x * 0.5f, 20.0f);

			ImGui::SetNextWindowSize(size);
			ImGui::SetNextWindowPos(pos);

			ImGui::Begin("PLAY", nullptr, flag);

			static int radio = 1;
			ImGui::RadioButton("PLAT", &radio, 1); ImGui::SameLine();
			ImGui::RadioButton("STOP", &radio, 0);

			radio ? CManager::Get_Instance()->Set_Play_Enable(true) : CManager::Get_Instance()->Set_Play_Enable(false);

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

		COMPONENT_MANEGER::Set_Draw_Enable_All(Debug_Draw_Enable);
	}
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
	// Rendering
	ImGui::Render();
	ImGui::EndFrame();
}

void My_imgui::Render(void)
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

const bool My_imgui::Get_Mouse_Over_Enable()
{
	return Mouse_Over_Enable;
}



void My_imgui::Draw_Inspector(const string& name)
{
	static const char* item_current = nullptr;
	static const char* temp_current = nullptr;

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

			float view[16] = { mat44._11, mat44._12, mat44._13,mat44._14,
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

			float pro[16] = { mat44._11, mat44._12, mat44._13,mat44._14,
								mat44._21, mat44._22, mat44._23,mat44._24,
								mat44._31, mat44._32, mat44._33,mat44._34,
								mat44._41, mat44._42, mat44._43,mat44._44
			};

			mtr = XMMatrixTranslation(object->Get_Position()->x, object->Get_Position()->y, object->Get_Position()->z);
			XMStoreFloat4x4(&mat44, mtr);

			static float pos[16] = { mat44._11, mat44._12, mat44._13,mat44._14,
									mat44._21, mat44._22, mat44._23,mat44._24,
									mat44._31, mat44._32, mat44._33,mat44._34,
									mat44._41, mat44._42, mat44._43,mat44._44
			};

			if (flag)
			{
				mtr = XMMatrixScaling(object->Get_Scaling()->x, object->Get_Scaling()->y, object->Get_Scaling()->z);
				mtr *= XMMatrixTranslation(object->Get_Position()->x, object->Get_Position()->y, object->Get_Position()->z);
				XMStoreFloat4x4(&mat44, mtr);

				pos[0] = mat44._11, pos[1] = mat44._12, pos[2] = mat44._13, pos[3] = mat44._14;
				pos[4] = mat44._21, pos[5] = mat44._22, pos[6] = mat44._23, pos[7] = mat44._24;
				pos[8] = mat44._31, pos[9] = mat44._32, pos[10] = mat44._33, pos[11] = mat44._34;
				pos[12] = mat44._41, pos[13] = mat44._42, pos[14] = mat44._43, pos[15] = mat44._44;
				
			}

			EditTransform(view, pro, pos, enable, object);
		}

		{
			/*static const char* item_current = nullptr;
			static const char* temp_current = nullptr;*/

			{
				static ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

				if (ImGui::BeginCombo((char*)u8"コンポーネント", item_current, flags)) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < Component_Items.first.size(); n++)
					{
						bool is_selected = (item_current == Component_Items.first.at(n));
						if (ImGui::Selectable(Component_Items.first.at(n), is_selected))
						{
							item_current = Component_Items.first.at(n);
							temp_current = Component_Items.second.at(n);
						}
					}
					ImGui::EndCombo();
				}
			}

			{
				Draw_Components(object->Get_Component()->Get_All_Components());
			}

			if (ImGui::Button("Add Component"))
			{
				if (nullptr != temp_current)
				{
					string s(temp_current);

					Add_Component(object, s);
				}
			}

			if (ImGui::Button("Delete Component"))
			{
				if (nullptr != temp_current)
				{
					string s(temp_current);

					Delete_Component(object, s);
				}
			}
		}

		ImGui::End();
	}
}

void EditTransform(const float* cameraView, float* cameraProjection, float* matrix, bool enable, GAME_OBJECT* object)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static float snap[3] = { 0.001f, 0.001f, 0.001f };

	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	{
		float	Translation[3] = { object->Get_Position()->x, object->Get_Position()->y, object->Get_Position()->z };
		float	Rotation[3] = { object->Get_Rotation()->x, object->Get_Rotation()->y, object->Get_Rotation()->z };
		float	R[3] = { object->Get_Rotation()->x, object->Get_Rotation()->y, object->Get_Rotation()->z };
		float	Scale[3] = { object->Get_Scaling()->x, object->Get_Scaling()->y, object->Get_Scaling()->z };

		ImGuizmo::DecomposeMatrixToComponents(matrix, Translation, Rotation, Scale);

		ImGui::DragFloat3((char*)u8"トランスフォーム", Translation, 0.01f);
		ImGui::DragFloat3((char*)u8"回転", Rotation, 0.1f);
		ImGui::DragFloat3((char*)u8"スケール", Scale, 0.01f);

		ImGuizmo::RecomposeMatrixFromComponents(Translation, Rotation, Scale, matrix);

		if (enable)
		{
			XMFLOAT3 vec1(Translation[0], Translation[1], Translation[2]);
			XMFLOAT3 vec2/*(Rotation[0], Rotation[1], Rotation[2])*/;
			XMFLOAT3 vec3(Scale[0], Scale[1], Scale[2]);

			object->Set_Position(vec1);
			//object->Set_Rotation(vec2);
			object->Set_Scaling(vec3);

			ImGui::DragFloat3("Rotation", R, 0.2f, -180.f, 180.f);
			vec2 = XMFLOAT3(R[0], R[1], R[2]);
			object->Set_Rotation(vec2);
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, ImGuizmo::LOCAL, matrix, NULL, &snap[0], NULL, NULL);
}

void My_imgui::Draw_Components(const vector<COMPONENT*>& components)
{
	ImGui::Spacing();
	ImGui::Text((char*)u8"コンポーネント");
	ImGui::Spacing();

	for (const auto& com : components)
	{
		ImGui::Spacing();
		com->Draw_Inspector();
		ImGui::Spacing();
	}
}

void My_imgui::Texture_Import()
{
	static bool flag = true;
	static bool flag2 = false;

	static string file_name;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

	if(Texture_Import_Enable)
	{
		static char check;

		ImGui::SetNextWindowSize(ImVec2(360, 167), ImGuiCond_Appearing);

		ImGui::Begin((char*)u8"テクスチャ インポート", &Texture_Import_Enable, window_flags);
		{
			ImGui::Indent(15.0f);

			ImGui::InputText((char*)u8"テクスチャ名", &file_name);
			ImGui::SameLine(); HelpMarker((char*)u8"対応フォーマットは 'png' 'jpg''dds'");

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Indent(100);

			ImVec2 size(100, 40);

			if (ImGui::Button((char*)u8"インポート", size))
			{
				Erroer_Message("テスト");

				check = File_Check(file_name);

				if (1 == check)
				{
					TEXTURE_MANEGER::Get_Instance()->Add(file_name);

					ImGui::Text((char*)u8"テクスチャが読み込まれました");
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

const char My_imgui::File_Check(const string& file_name)
{
	if (file_name.empty())
	{
		// テクスチャ名が入力されていない
		return -1;
	}

	if (TEXTURE_MANEGER::Get_Instance()->Get_TextureFile().find(file_name) != TEXTURE_MANEGER::Get_Instance()->Get_TextureFile().end())
	{
		// 既に読み込んでいるテクスチャ
		return -2;
	}

	// ファイルがあるかの判定
	{
		string path = "./asset/texture/";

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

	static string file_name;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;

	if (Texture_Import_Enable)
	{
		static char check;

		ImGui::SetNextWindowSize(ImVec2(360, 167), ImGuiCond_Appearing);

		ImGui::Begin((char*)u8"テクスチャ 削除", &Texture_Import_Enable, window_flags);
		{
			ImGui::Indent(15.0f);

			ImGui::InputText((char*)u8"テクスチャ名", &file_name);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Indent(100);

			ImVec2 size(100, 40);

			if (ImGui::Button((char*)u8"削除", size))
			{
				check = File_Check(file_name);

				if (-1 != check)
				{
					TEXTURE_MANEGER::Get_Instance()->Unload(file_name);

					ImGui::Text((char*)u8"テクスチャが削除されました");
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
		float color[] = { BOUNDING::Get_Default_Color().r, BOUNDING::Get_Default_Color().g, BOUNDING::Get_Default_Color().b };

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
	// タイトルバーありの場合、imageyよりWindowSizeがImVec2(17, 40)分大きければ丁度いい
	// タイトルバーなしの場合、imageyよりWindowSizeがImVec2(13, 16)分大きければ丁度いい

	//ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

	char i = 0;

	{
		//ImGui::SetNextWindowSize(ImVec2(256 + 17, 256 + 40), ImGuiCond_Once);

		ImGui::Begin("Texture");

		auto texture = TEXTURE_MANEGER::Get_Instance()->Get_TextureData_Start();
		auto end = TEXTURE_MANEGER::Get_Instance()->Get_TextureData_End();

		for(; texture != end; texture++)
		{
			ImTextureID image = texture->second.Resource.get();

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

		for (int i = 0; i < lights->size(); i++)
		{
			// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.

			//if(1 == lights->at(i).Enable)
			{
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Light %d", i);
				if (node_open)
				{
					{
						bool flag = lights->at(i).Enable;
						float Position[3] = { lights->at(i).Position.x, lights->at(i).Position.y, lights->at(i).Position.z };
						float Color[3] = { lights->at(i).Color.r, lights->at(i).Color.g, lights->at(i).Color.b };
						float Radius = lights->at(i).Radius;

						UINT Type = lights->at(i).Type;//

						ImGui::Checkbox("Enable", &flag);
						ImGui::DragFloat3("Position", Position, 0.01f);
						ImGui::DragFloat3("Color", Color, 0.01f, 0.f, 1.0f);
						ImGui::DragFloat("Radius", &Radius, 0.01f, 0.f, 1000.0f);

						////
						{
							vector<const char*> Items;

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
								static ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

								if (ImGui::BeginCombo((char*)u8"ライトの種類", item_current, flags))
								{
									for (int n = 0; n < Items.size(); n++)
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
						lights->at(i).Position = XMFLOAT3(Position[0], Position[1], Position[2]);
						lights->at(i).Color = COLOR(Color[0], Color[1], Color[2], 0.0f);
						lights->at(i).Radius = Radius;
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

void My_imgui::Add_Component(GAME_OBJECT* object, const string s)
{
	ImGui::Text("%s", s.c_str());

	auto scene = CManager::Get_Instance()->Get_Scene();
	auto comp = object->Get_Component();

	if (string::npos != s.find("AABB"))
	{
		comp->Add_Component<BOUNDING_AABB>(scene->Get_Game_Object(object));
		return;
	}

	if (string::npos != s.find("OBB"))
	{
		comp->Add_Component<BOUNDING_OBB>(scene->Get_Game_Object(object));
		return;
	}

	if (string::npos != s.find("球"))
	{
		comp->Add_Component<BOUNDING_SHPERE>(scene->Get_Game_Object(object));
		return;
	}
}

void My_imgui::Delete_Component(GAME_OBJECT* object, const string s)
{
	ImGui::Text("%s", s.c_str());

	auto scene = CManager::Get_Instance()->Get_Scene();
	auto comp = object->Get_Component();

	COMPONENT* component = nullptr;

	if (string::npos != s.find("AABB"))
	{
		component = comp->Get_Component<BOUNDING_AABB>();
	}

	if (string::npos != s.find("OBB"))
	{
		component = comp->Get_Component<BOUNDING_OBB>();
	}

	if (string::npos != s.find("球"))
	{
		component = comp->Get_Component<BOUNDING_SHPERE>();
	}

	if(nullptr == component) return;

	component->SetDestroy();
}

#endif // _DEBUG