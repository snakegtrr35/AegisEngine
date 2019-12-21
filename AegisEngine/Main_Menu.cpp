#include	"Scene.h"
#include	"ModelLoader.h"
#include	"Input.h"
#include	"manager.h"
#include	"Component.h"
#include	"audio_clip.h"
#include	"Math.h"

#include	"Fade.h"

#include	"Timer.h"

#include	"Bounding_Capsule.h"

string MAIN_MENU::Model_Name = "asset/model/herorifle.fbx";

static bool flag = false;
//static short cnt = 0; 

unique_ptr<BOUNDING> Capsule;

static unique_ptr<ID3D11Buffer, Release> pVertexBuffer;		// 頂点バッファ
static unique_ptr<ID3D11Buffer, Release> pIndexBuffer;		// インデックスバッファ

static unique_ptr<ID3D11Buffer, Release> pVB;		// 頂点バッファ
static unique_ptr<ID3D11Buffer, Release> pIB;		// インデックスバッファ

static UINT IndexNum = 0;
static UINT VertexNum = 0;

void D(const XMFLOAT3& position, const XMFLOAT3& rotate);

void Line(const XMFLOAT3& position, const XMFLOAT3& rotate);

string Replace_String(string& replacedStr, const string& from, const string& to)
{
	if (replacedStr.empty() || from.empty() || to.empty())
	{
		return replacedStr;
	}

	if (from.size() != to.size())
	{
		return replacedStr;
	}

	const UINT pos = replacedStr.find(from);
	const UINT len = from.length();

	if (replacedStr.size() < pos)
	{
		return replacedStr;
	}

	return replacedStr.replace(pos, len, to);
}


void MAIN_MENU::Init()
{
	bool flag;

	{
		const type_info& id = typeid(this);

		string name(id.name());
		
		// 置換
		Replace_String(name, "class ", "      ");
		Replace_String(name, "__ptr64", "       ");
		Replace_String(name, "*", " ");
		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());

		std::ifstream file(name + ".dat", std::ios::binary);

		flag = file.is_open();

		if (flag)
		{
			cereal::BinaryInputArchive archive(file);
			archive(*this);
		}
	}

	if (false == flag)
	{
		// カメラ
		{
			//auto camera = Add_Game_Object<CCamera>(LAYER_NAME::BACKGROUND, "camera");
			auto camera = Add_Game_Object<DEBUG_CAMERA>(LAYER_NAME::BACKGROUND, "camera");

			camera->Set_Viewing_Angle(80.0f);

			//camera->Set_Move_Enable(false);
			//camera->Set_Rotate_Enable(false);
		}

		////プレイヤー
		//{
		//	CMODEL* player = Add_Game_Object<CMODEL>(LAYER_NAME::GAMEOBJECT, "player");

		//	//string name("asset/model/herorifle.fbx");
		//	string name("asset/model/human01_Stop.fbx");
		//	//string name("asset/model/untitled.fbx");
		//	//string name("asset/model/Dragon 2.5_fbx.fbx");

		//	XMFLOAT3 f3;

		//	f3 = XMFLOAT3(0.f, 0.f, 0.f);
		//	player->Set_Position(f3);

		//	f3 = XMFLOAT3(0.f, 0.f, 0.f);
		//	player->Set_Rotation(f3);

		//	//f3 = XMFLOAT3(0.1f, 0.1f, 0.1f);
		//	f3 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//	player->Set_Scaling(f3);

		//	player->Load(name);
		//}

		{
			auto player = Add_Game_Object<PLAYER>(LAYER_NAME::GAMEOBJECT, "player");
		}

		{
			auto pmd = Add_Game_Object<MESH_DOOM>(LAYER_NAME::BACKGROUND, "sky_doom");
		}

		//{
		//	//Add_Game_Object<GRID>(LAYER_NAME::BACKGROUND, "grid");
		//}

		{
			Add_Game_Object<MESH_FIELD>(LAYER_NAME::GAMEOBJECT, "field");
		}

		//{
		//	Add_Game_Object<POLYGON_3D>(LAYER_NAME::GAMEOBJECT, "cube");
		//}

		// テキスト画像
		{
			XMFLOAT2 pos(50.0f, 50.0f);
			auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI, "text1");

			text->SetPosition(pos);

			text->SetSize(XMFLOAT4(20, 20, 20, 20));

			text->Edit("Hello HELL World!!!地球の未来にご奉仕するにゃん！");
		}



		//// 画像
		//{
		//	XMFLOAT2 pos(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.2f);

		//	SPRITE* title = Add_Game_Object<SPRITE>(LAYER_NAME::UI);

		//	title->SetPosition(pos);

		//	title->SetSize(XMFLOAT4(200, 200, 200, 200));

		//	title->SetTexture(string("UVCheckerMap01-512.png"));

		//	title->Set_Object_Name("sprite");
		//}

		//{
		//	auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI);

		//	XMFLOAT2 pos(100.0f, 300.0f);

		//	text->SetPosition(pos);

		//	text->SetSize(XMFLOAT4(20, 20, 20, 20));

		//	text->Set_Object_Name(string("delta_time"));

		//	//string time = to_string(Time);

		//	text->Edit("0.000");
		//}

		{
			auto text = Add_Game_Object<TEXTS>(LAYER_NAME::UI, "fps");

			XMFLOAT2 pos(100.0f, 340.0f);

			text->SetPosition(pos);

			text->SetSize(XMFLOAT4(20, 20, 20, 20));

			text->Edit("0.000");
		}

		{
			Add_Game_Object<BOUNDING_AABB>(LAYER_NAME::GAMEOBJECT, "aabb");
		}

		{
			Add_Game_Object<BOUNDING_OBB>(LAYER_NAME::GAMEOBJECT, "obb");
		}
	}
	
	Capsule.reset(new BOUNDING_CAPSULE());
	Capsule->Init();

	SCENE::Init();

	//cnt = 0;

	FADE::Start_FadeIn(60);
	flag = false;

	{
		//float Radius = 3.0f;
		//VertexNum = 10 * (UINT)Radius * 0.5;

		//// 頂点バッファの設定
		//if (nullptr == pVertexBuffer.get())
		//{
		//	VERTEX_3D* vertex = new VERTEX_3D[VertexNum];

		//	const float angle = XM_PI / (VertexNum - 1);

		//	for (int i = 0; i < VertexNum; i++)
		//	{
		//		vertex[i].Position = XMFLOAT3(cosf(angle * i) * Radius, sinf(angle * i) * Radius, 0.0f);
		//		vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//		vertex[i].Diffuse = XMFLOAT4(1.0f, 0.f, 0.f, 1.0f);
		//		vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		//	}

		//	// 頂点バッファの設定
		//	{
		//		ID3D11Buffer* buffer;

		//		D3D11_BUFFER_DESC bd;
		//		ZeroMemory(&bd, sizeof(bd));

		//		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		//		bd.Usage = D3D11_USAGE_DYNAMIC;
		//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//		bd.MiscFlags = 0;
		//		bd.StructureByteStride = 0;

		//		D3D11_SUBRESOURCE_DATA sd;
		//		sd.pSysMem = vertex;
		//		sd.SysMemPitch = 0;
		//		sd.SysMemSlicePitch = 0;

		//		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		//		pVertexBuffer.reset(buffer);
		//	}

		//	SAFE_DELETE_ARRAY(vertex);
		//}

		//// インデックスバッファの設定
		//if (nullptr == pIndexBuffer.get())
		//{
		//	IndexNum = VertexNum * 2;
		//	WORD* index_array = new WORD[IndexNum];

		//	for (int i = 0; i < VertexNum; i++)
		//	{
		//		index_array[i * 2] = i;
		//		index_array[i * 2 + 1] = (i + 1) % VertexNum;
		//	}

		//	// インデックスバッファの設定
		//	{
		//		ID3D11Buffer* buffer;

		//		D3D11_BUFFER_DESC bd;
		//		ZeroMemory(&bd, sizeof(bd));

		//		bd.ByteWidth = sizeof(WORD) * IndexNum;
		//		bd.Usage = D3D11_USAGE_DEFAULT;
		//		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//		bd.CPUAccessFlags = 0;
		//		bd.MiscFlags = 0;
		//		bd.StructureByteStride = 0;

		//		D3D11_SUBRESOURCE_DATA sd;
		//		ZeroMemory(&sd, sizeof(sd));
		//		sd.pSysMem = index_array;

		//		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		//		pIndexBuffer.reset(buffer);
		//	}

		//	SAFE_DELETE(index_array);
		//}
		//
		//// 頂点バッファの設定
		//if (nullptr == pVB.get())
		//{
		//	VERTEX_3D vertex[6];

		//	vertex[0].Position = XMFLOAT3(3.f, -2.5, 0.f);
		//	vertex[1].Position = XMFLOAT3(3.f, 2.5, 0.f);

		//	vertex[2].Position = XMFLOAT3(3.f, 2.5, 0.f);
		//	vertex[3].Position = XMFLOAT3(-3.f, -2.5, 0.f);

		//	vertex[4].Position = XMFLOAT3(-3.f, -2.5, 0.f);
		//	vertex[5].Position = XMFLOAT3(-3.f, 2.5, 0.f);

		//	for (int i = 0; i < 6; i++)
		//	{
		//		vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//		vertex[i].Diffuse = XMFLOAT4(1.0f, 0.f, 0.f, 1.0f);
		//		vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		//	}

		//	// 頂点バッファの設定
		//	{
		//		ID3D11Buffer* buffer;

		//		D3D11_BUFFER_DESC bd;
		//		ZeroMemory(&bd, sizeof(bd));

		//		bd.ByteWidth = sizeof(VERTEX_3D) * 6;
		//		bd.Usage = D3D11_USAGE_DYNAMIC;
		//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//		bd.MiscFlags = 0;
		//		bd.StructureByteStride = 0;

		//		D3D11_SUBRESOURCE_DATA sd;
		//		sd.pSysMem = vertex;
		//		sd.SysMemPitch = 0;
		//		sd.SysMemSlicePitch = 0;

		//		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		//		pVB.reset(buffer);
		//	}
		//}

		//// インデックスバッファの設定
		//if (nullptr == pIB.get())
		//{
		//	WORD index_array[] = {
		//		0, 1,
		//		1, 2,
		//		3, 4,
		//		4, 5
		//	};
		//		

		//	// インデックスバッファの設定
		//	{
		//		ID3D11Buffer* buffer;

		//		D3D11_BUFFER_DESC bd;
		//		ZeroMemory(&bd, sizeof(bd));

		//		bd.ByteWidth = sizeof(WORD) * 8;
		//		bd.Usage = D3D11_USAGE_DEFAULT;
		//		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//		bd.CPUAccessFlags = 0;
		//		bd.MiscFlags = 0;
		//		bd.StructureByteStride = 0;

		//		D3D11_SUBRESOURCE_DATA sd;
		//		ZeroMemory(&sd, sizeof(sd));
		//		sd.pSysMem = index_array;

		//		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		//		pIB.reset(buffer);
		//	}
		//}
	}
}

void MAIN_MENU::Draw()
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		//// 入力アセンブラに頂点バッファを設定
		//CRenderer::SetVertexBuffers(pVertexBuffer.get());

		//// 入力アセンブラにインデックスバッファを設定
		//CRenderer::SetIndexBuffer(pIndexBuffer.get());

		//// トポロジの設定
		//CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		//CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		//XMFLOAT3 p = XMFLOAT3(0, 2.5, 0);
		//XMFLOAT3 r = XMFLOAT3(0, 0, 0);

		//D(p, r);

		//p = XMFLOAT3(0, -2.5, 0);
		//r = XMFLOAT3(0, 0, 180);
		//D(p, r);

		//Line(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));

		//CRenderer::Set_Shader();
	}

	//Capsule->Draw();

	SCENE::Draw();
}

void MAIN_MENU::Update(float delta_time)
{
	SCENE::Update(delta_time);

	{
		static char cnt = 0;

		if (cnt == 3)
		{
			//{
			//	auto text = Get_Game_Object<TEXTS>("delta_time");

			//	auto time = TIMER::Get_DeltaTime();

			//	text->Edit(to_string(time));
			//}

			{
				auto text = Get_Game_Object<TEXTS>("fps");

				auto time = TIMER::Get_FPS();

				text->Edit(to_string(time));
			}

			cnt = 0;
		}

		cnt++;
	}

	if (FADE::End_Fade())
	{
		if(flag)
			SCENE_MANAGER::Set_Scene<GAME>();

		flag = true;
	}
}

void MAIN_MENU::Uninit()
{
	static bool flag = true;

	if(flag)
	{
		const type_info& id = typeid(this);

		string name(id.name());

		// 置換
		Replace_String(name, "class ", "      ");
		Replace_String(name, "__ptr64", "       ");
		Replace_String(name, "*", " ");
		name.erase(remove_if(name.begin(), name.end(), isspace), name.end());

		std::ofstream file(name + ".dat", std::ios::binary);

		bool f = file.is_open();

		cereal::BinaryOutputArchive archive(file);
		archive(*this);

		flag = false;
	}

	SCENE::Uninit();

	AUDIO_MANAGER::Stop_Sound_Object();
}

void D(const XMFLOAT3& position, const XMFLOAT3& rotate)
{
	auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMMATRIX world = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
	world *= XMMatrixTranslation(position.x, position.y, position.z);

	if (nullptr != camera01)

	{
		CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
	}
	else
	{
		CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
	}

	//CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
	CRenderer::GetDeviceContext()->Draw(VertexNum, 0);
}

void Line(const XMFLOAT3& position, const XMFLOAT3& rotate)
{
	// 入力アセンブラに頂点バッファを設定
	CRenderer::SetVertexBuffers(pVB.get());

	// 入力アセンブラにインデックスバッファを設定
	CRenderer::SetIndexBuffer(pIB.get());

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

	auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMMATRIX world = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
	world *= XMMatrixTranslation(position.x, position.y, position.z);

	if (nullptr != camera01)

	{
		CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
	}
	else
	{
		CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

		CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
	}

	CRenderer::GetDeviceContext()->DrawIndexed(8, 0, 0);
	//CRenderer::GetDeviceContext()->Draw(6, 0);

	CRenderer::Set_Shader();
}