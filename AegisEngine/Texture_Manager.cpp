#include	"Texture_Manager.h"
#include	"Renderer.h"
#include	"manager.h"

#include	"external/DirectXTex/WICTextureLoader.h"
#include	"external/DirectXTex/DDSTextureLoader.h"

unique_ptr<TEXTURE_MANEGER> TEXTURE_MANEGER::Texture_Manager;

bool TEXTURE_MANEGER::Init()
{
	if (nullptr == Texture_Manager.get())
	{
		Texture_Manager = make_unique<TEXTURE_MANEGER>();
	}

	bool flag;

	{
		std::ifstream file("texture.dat", std::ios::binary);

		flag = file.is_open();

		if (flag)
		{
			size_t size = filesystem::file_size("texture.dat");

			if (0 < size)
			{
				cereal::BinaryInputArchive archive(file);
				archive(*Texture_Manager.get());
			}
		}
	}

	// デフォルトの画像データの読み込み
	Texture_Manager->Default_Load(flag);

	// 画像データの読み込み
	Texture_Manager->Load(flag);

#ifdef _DEBUG
	{
		Texture_Manager->Monitor = make_unique<FILE_CHANGE_MONITOR>("./asset/texture");
		if (false == Texture_Manager->Monitor->Init())
		{
			return false;
		}
	}
#endif // _DEBUG

	return true;
}

void TEXTURE_MANEGER::Uninit()
{
#ifdef _DEBUG
	Monitor.reset(nullptr);
#endif // _DEBUG

	{
		std::ofstream file("texture.dat", std::ios::binary);

		cereal::BinaryOutputArchive archive(file);
		archive(*Texture_Manager.get());
	}

	Default_Texture_File.clear();
	TextureFile.clear();

	for (auto tex = TextureData.begin(); tex != TextureData.end(); tex++)
	{
		tex->second.Resource.reset(nullptr);
	}
	TextureData.clear();
}

void TEXTURE_MANEGER::Update()
{
#ifdef _DEBUG
	Load_Check();

	wstring path;			// ファイル名(パス付き)
	wstring file_name;		// ファイル名(パスなし)
	wstring type;
	size_t pos;
	size_t first;			// 

	while (false == Monitor->Get_FileStack_Empty())
	{
		file_name = Monitor->Pop_FileStack();

		path = L"./asset/texture/" + file_name;

		{
			// ファイルが更新された
			wstring name;
			HRESULT hr;
			ID3D11ShaderResourceView* ShaderResourceView;

			pos = file_name.find_last_of(L".");
			type = file_name.substr(pos + 1);

			if (L"dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), path.c_str(), nullptr, &ShaderResourceView, nullptr, nullptr);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}
			else	// jpg か png
			{
				hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), path.c_str(), nullptr, &ShaderResourceView, nullptr, nullptr);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}

			first = hash<string>()( wstringTostring(file_name));

			TextureData[first].Resource.reset(ShaderResourceView);
		}
	}
#endif // _DEBUG
}

void TEXTURE_MANEGER::Default_Load(const bool flag)
{
	int width, height;

	string path;			// ファイル名(パス付き) 
	string file_name;		// ファイル名(パスなし)
	string type;
	size_t first;			// 
	size_t pos;

	ID3D11ShaderResourceView* ShaderResourceView;
	wstring name;

	std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/Default/texture");
	for (auto file : e) {

		// 一つ一つのファイル名(パス付き)
		path = file.path().string();

		// 置換
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		first = hash<string>()(file_name);//

		// バイナリファイルがない
		if (false == flag)
		{
			// テクスチャの登録
			Default_Texture_File[first] = path;

			TextureData[first].Cnt = 0;
		}

		// テクスチャの読み込み
		{
			pos = file_name.find_last_of(".");
			type = file_name.substr(pos + 1);

			HRESULT hr;

			// char から wchar_t への変換
			name = stringTowstring(path);

			if ("dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}
			else	// jpg か png
			{
				hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}

			TextureData[first].Resource.reset(ShaderResourceView);
			TextureData[first].WH.x = width;
			TextureData[first].WH.y = height;
		}
	}
}

void TEXTURE_MANEGER::Load(const bool flag)
{
	// バイナリファイルがない
	if (false == flag)
	{

		int width, height;

		string path;			// ファイル名(パス付き) 
		string file_name;		// ファイル名(パスなし)
		string type;
		size_t first;			// 
		size_t pos;

		std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/texture");
		for (auto file : e)
		{
			// 一つ一つのファイル名(パス付き)
			path = file.path().string();

			// 置換
			replace(path.begin(), path.end(), '\\', '/');

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			first = hash<string>()(file_name);//

			//バイナリファイルがない
			{
				//テクスチャの登録
				TextureFile[first].Path = path;
			}

			// テクスチャの読み込み
			if (TextureFile.find(first) != TextureFile.end())
			{
				ID3D11ShaderResourceView* ShaderResourceView;
				HRESULT hr;
				wstring name;

				pos = file_name.find_last_of(".");
				type = file_name.substr(pos + 1);

				// char から wchar_t への変換
				name = stringTowstring(path);

				if ("dds" == type)	// dds
				{
					hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
					if (FAILED(hr))
					{
						FAILDE_ASSERT;
						return;
					}
				}
				else	// jpg か png
				{
					hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
					if (FAILED(hr))
					{
						FAILDE_ASSERT;
						return;
					}
				}

				TextureData[first].Resource.reset(ShaderResourceView);
				TextureData[first].WH.x = width;
				TextureData[first].WH.y = height;
				TextureData[first].Cnt = 0;
			}
		}
	}
	else
	{
		int width, height;
		string path;			// ファイル名(パス付き) 
		string file_name;		// ファイル名(パスなし)
		string type;
		size_t first;
		size_t pos;

		ID3D11ShaderResourceView* ShaderResourceView;
		HRESULT hr;
		wstring name;

		for (auto f : TextureFile)
		{
			path = f.second.Path;

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			first = hash<string>()(file_name);//

			pos = path.find_last_of(".");
			type = path.substr(pos + 1);

			// char から wchar_t への変換
			name = stringTowstring(path);

			if ("dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}
			else	// jpg か png
			{
				hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}

			TextureData[first].Resource.reset(ShaderResourceView);
			TextureData[first].WH.x = width;
			TextureData[first].WH.y = height;
			TextureData[first].Cnt = 0;
		}
	}
}

void TEXTURE_MANEGER::Add(const string& file_name)
{
	int width, height;

	string path;	// ファイル名
	string type;
	size_t first;			// 
	size_t pos;

	{
		pos = file_name.find_last_of("/");

		path = file_name.substr(pos + 1);

		// テクスチャの読み込み
		{
			ID3D11ShaderResourceView* ShaderResourceView;
			HRESULT hr;
			wstring name;

			pos = path.find_last_of(".");
			type = path.substr(pos + 1);

			// char から wchar_t への変換
			name = stringTowstring("asset/texture/" + path);

			if ("dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}
			else	// jpg か png
			{
				hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}

			first = hash<string>()(file_name);
			TextureFile[first].Path = "asset/texture/" + path;

			TextureData[first].Resource.reset(ShaderResourceView);
			TextureData[first].WH.x = width;
			TextureData[first].WH.y = height;
		}
	}
}

const bool TEXTURE_MANEGER::Unload(const string& const file_name)
{
	size_t first = hash<string>()(file_name);//

#ifdef _DEBUG
	if (0 != TextureData[first].Cnt)
	{
		// 参照しているものがある
		return false;
	}
#endif // _DEBUG

	TextureData[first].Resource.reset(nullptr);

	TextureData.erase(first);

	TextureFile.erase(first);

	return true;
}

#ifdef _DEBUG
#include	"Timer.h"
double fps = 0.0;

void TEXTURE_MANEGER::Load_Check()
{
	fps += TIMER::Get_DeltaTime();

	if (10.0 < fps)
	{
		fps = 0.0;

		// 状況を更新
		Monitor->readChanges();
	}
}
#endif // _DEBUG

void TEXTURE_MANEGER::Add_ReferenceCnt(const size_t file)
{
	TextureData[file].Cnt++;
}

void TEXTURE_MANEGER::Sub_ReferenceCnt(const size_t file)
{
	TextureData[file].Cnt--;

#ifdef _DEBUG
	if (TextureData[file].Cnt < 0)
	{
		FAILDE_ASSERT;
		return;
	}
#endif // _DEBUG
}

XMINT2* const TEXTURE_MANEGER::Get_WH(const size_t file)
{
	if (TextureData.find(file) != TextureData.end())
	{
		return &TextureData[file].WH;
	}

	return nullptr;
}

ID3D11ShaderResourceView* const TEXTURE_MANEGER::GetShaderResourceView(const size_t file)
{
	if (TextureData.find(file) != TextureData.end())
	{
		return TextureData[file].Resource.get();
	}

	return nullptr;
}

unordered_map<size_t, TEXTURE_FILE>& TEXTURE_MANEGER::Get_TextureFile()
{
	return TextureFile;
}

const unordered_map<size_t, TEXTURE_DATA>::iterator TEXTURE_MANEGER::Get_TextureData_Start()
{
	return TextureData.begin();
}

const unordered_map<size_t, TEXTURE_DATA>::iterator TEXTURE_MANEGER::Get_TextureData_End()
{
	return TextureData.end();
}


TEXTURE_MANEGER* TEXTURE_MANEGER::Get_Instance()
{
	return Texture_Manager.get();
}