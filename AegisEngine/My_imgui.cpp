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

#include	"common.h"

extern float radius;

static string old_name;

static BoundingFrustum Frustum;
static BoundingBox Aabb;

void EditTransform(const float* cameraView, float* cameraProjection, float* matrix, bool enable, GAME_OBJECT* object);

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
		BoundingFrustum::CreateFromMatrix(Frustum, XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f), float(SCREEN_WIDTH / SCREEN_HEIGHT), 1.0f, 100.0f));
		Frustum.Origin.z = 0.0f;
		{
			XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(180.f), XMConvertToRadians(0.f));
			XMVECTOR rotate = XMQuaternionRotationMatrix(matrix);
			Frustum.Transform(Frustum, 1.0f, rotate, XMVectorSet(0.f, 0.f, 0.f, 0.f));
		}
	}

	{
		Aabb.Center = XMFLOAT3(0.f, 0.f, 0.f);
		Aabb.Extents = XMFLOAT3(5.f, 5.f, 5.f);
	}
}

void My_imgui::Draw(void)
{
	//fps = ImGui::GetIO().Framerate;

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

			ImGui::Text("Application average %.6f ms/frame (%.1f FPS)", (1000.0f / ImGui::GetIO().Framerate) /** 0.001f*/, ImGui::GetIO().Framerate);

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

				//// ライトの設定
				//LIGHT light;
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

				ImTextureID image = CManager::Get_ShadowMap()->Get();

				ImGui::Image(image, ImVec2(512, 512));

				ImGui::End();
			}

			{
				ImGui::SetNextWindowSize(ImVec2(512 + 17, 512 + 40), ImGuiCond_Once);

				ImGui::Begin("Depth", nullptr, window_flags);

				ImTextureID image = CRenderer::Get();

				ImGui::Image(image, ImVec2(512, 512));

				ImGui::End();
			}

			{
				ImGui::SetNextWindowSize(ImVec2(512 + 17, 512 + 40), ImGuiCond_Once);

				ImGui::Begin("Diffeuse", nullptr, window_flags);

				ImTextureID image = CRenderer::Get2();

				ImGui::Image(image, ImVec2(512, 512));

				ImGui::End();
			}

			{
				ImGui::SetNextWindowSize(ImVec2(512 + 17, 512 + 40), ImGuiCond_Once);

				ImGui::Begin("Normal", nullptr, window_flags);

				ImTextureID image = CRenderer::Get3();

				ImGui::Image(image, ImVec2(512, 512));

				ImGui::End();
			}
			}

			// オブジェクト一覧
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

		/*{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

			ImGui::SetNextWindowSize(ImVec2(512 + 17, 512 + 40), ImGuiCond_Once);

			ImGui::Begin("Test", nullptr, window_flags);

			{
				float Translation[3] = { Aabb.Center.x, Aabb.Center.y, Aabb.Center.z };
				float Extents[3] = { Aabb.Extents.x, Aabb.Extents.y, Aabb.Extents.z };

				//ImGui::DragFloat3("Translation", Translation, 0.01f);
				//ImGui::DragFloat3("Extents", Extents, 0.1f);

				Aabb.Center = XMFLOAT3(Translation[0], Translation[1], Translation[2]);
				Aabb.Extents = XMFLOAT3(Extents[0], Extents[1], Extents[2]);
			}

			{
				switch (Frustum.Contains(Aabb))
				{
					case ContainmentType::CONTAINS:
						ImGui::Text((char*)u8"含まれている");
						break;

					case ContainmentType::DISJOINT:
						ImGui::Text((char*)u8"重なっている");
						break;

					case ContainmentType::INTERSECTS:
						ImGui::Text((char*)u8"含まれていない");
						break;
				}

				if (ContainmentType::CONTAINS == Frustum.Contains(Aabb))
				{
					ImGui::Text((char*)u8"含まれている");
				}
				else

				if((ContainmentType::DISJOINT == Frustum.Contains(Aabb)))
				{
					ImGui::Text((char*)u8"重なっている");
				}

				if ((ContainmentType::INTERSECTS == Frustum.Contains(Aabb)))
				{
					ImGui::Text((char*)u8"含まれていない");
				}
			}

			{
				static float Trans[3] = { 0.f, 0.f, 0.f };
				static float Rotate[3] = { 0.f, 180.f, 0.f };

				ImGui::DragFloat3("Trans", Trans, 0.01f);
				ImGui::DragFloat3("Rotate", Rotate, 0.1f);

				XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotate[0]), XMConvertToRadians(Rotate[1]), XMConvertToRadians(Rotate[2]));
				XMVECTOR rotate = XMQuaternionRotationMatrix(matrix);
				Frustum.Transform(Frustum, 1.0f, rotate, XMVectorSet(XMConvertToRadians(Trans[0]), XMConvertToRadians(Trans[1]), XMConvertToRadians(Trans[2]), 0.f));
			}

			ImGui::End();
		}*/

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
	bool flag = false;
	if (old_name != name) flag = true;

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

			CCamera* camera1;
			DEBUG_CAMERA* camera2;

			{
				camera1 = SCENE::Get_Game_Object<CCamera>("camera");
				if (nullptr == camera1)
				{
					camera2 = SCENE::Get_Game_Object<DEBUG_CAMERA>("camera");
					mtr = camera2->Get_Camera_View();
				}
				else
				{
					mtr = camera1->Get_Camera_View();
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
				if (nullptr == camera1)
				{
					mtr = camera2->Get_Camera_Projection();
				}
				else
				{
					mtr = camera1->Get_Camera_Projection();
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
				mtr = XMMatrixTranslation(object->Get_Position()->x, object->Get_Position()->y, object->Get_Position()->z);
				XMStoreFloat4x4(&mat44, mtr);

				pos[0] = mat44._11, pos[1] = mat44._12, pos[2] = mat44._13, pos[3] = mat44._14;
				pos[4] = mat44._21, pos[5] = mat44._22, pos[6] = mat44._23, pos[7] = mat44._24;
				pos[8] = mat44._31, pos[9] = mat44._32, pos[10] = mat44._33, pos[11] = mat44._34;
				pos[12] = mat44._41, pos[13] = mat44._42, pos[14] = mat44._43, pos[15] = mat44._44;
				
			}

			EditTransform(view, pro, pos, enable, object);
		}

		static int clicked = 0;
		if (ImGui::Button("Add Component"))
			clicked++;
		if (clicked & 1)
		{
			ImGui::SameLine();
			ImGui::Text("Thanks for clicking me!");
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

		ImGui::DragFloat3("Translation", Translation, 0.01f);
		ImGui::DragFloat3("Rotate", Rotation, 0.1f);
		ImGui::DragFloat3("Scale", Scale, 0.01f);

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
					TEXTURE_MANEGER::Add(file_name);

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

	if (TEXTURE_MANEGER::Get_TextureFile().find(file_name) != TEXTURE_MANEGER::Get_TextureFile().end())
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
					TEXTURE_MANEGER::Unload(file_name);

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

		auto texture = TEXTURE_MANEGER::Get_TextureData_Start();
		auto end = TEXTURE_MANEGER::Get_TextureData_End();

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

#endif // _DEBUG