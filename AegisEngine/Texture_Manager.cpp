#include	"Texture_Manager.h"
#include	"Renderer.h"

#include	"Library/DirectXTex/WICTextureLoader.h"
#include	"Library/DirectXTex/DDSTextureLoader.h"

unordered_map<string, string>			TEXTURE_MANEGER::Default_Texture_File;
unordered_map<string, TEXTURE_FILE>		TEXTURE_MANEGER::TextureFile;
unordered_map<string, TEXTURE_DATA>		TEXTURE_MANEGER::TextureData;


void TEXTURE_MANEGER::Init()
{
	{
		std::ofstream file("texture.dat", std::ios::binary);

			cereal::BinaryOutputArchive archive(file);
			//archive(Default_Texture_File);
			archive(TextureFile);
			archive(TextureData);
	}

	// デフォルトの画像データの読み込み
	Default_Load();

	// 画像データの読み込み
	Load();
}

void TEXTURE_MANEGER::Uninit()
{
	{
		TEXTURE_MANEGER t;

		std::ifstream file("texture.dat", std::ios::binary);

		if (file.is_open())
		{
			cereal::BinaryInputArchive archive(file);
			archive(t);
		}
	}

	Default_Texture_File.clear();
	TextureFile.clear();

	for (auto tex = TextureData.begin(); tex != TextureData.end(); tex++)
	{
		tex->second.Resource.reset(nullptr);
	}
	TextureData.clear();
}

void TEXTURE_MANEGER::Updata()
{
	string path;			// ファイル名(パス付き)
	string file_name;		// ファイル名(パスなし)
	string type;
	size_t pos;
	DWORD time;				// ファイルの最終更新時間

	std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/texture");
	for (auto file : e)
	{
		// 一つ一つのファイル名(パス付き)
		path = file.path().string();

		// 置換
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		time = Get_File_Time(path);

		if (time != TextureFile[file_name].Time)
		{
			// ファイルが更新された
			wstring name;
			HRESULT hr;
			ID3D11ShaderResourceView* ShaderResourceView;

			pos = file_name.find_last_of(".");
			type = file_name.substr(pos + 1, 3);

			file_name = "asset/texture/" + file_name;

			// char から wchar_t への変換
			name = stringTowstring(file_name);

			if ("dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}
			else	// jpg か png
			{
				hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, nullptr, nullptr);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}

			//TextureData[file_name].Resource.reset(nullptr);
			TextureData[file_name].Resource.reset(ShaderResourceView);
		}
	}
}

void TEXTURE_MANEGER::Default_Load()
{
	UINT width, height;

	string path;			// ファイル名(パス付き) 
	string file_name;		// ファイル名(パスなし)
	string type;

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

		// テクスチャ名の登録
		Default_Texture_File[file_name] = path;

		// テクスチャの読み込み
		{
			pos = file_name.find_last_of(".");
			type = file_name.substr(pos + 1);

			HRESULT hr;

			// char から wchar_t への変換
			name = stringTowstring("asset/Default/texture/" + file_name);

			if ("dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView);
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

			TextureData[file_name].Resource.reset(ShaderResourceView);
			TextureData[file_name].WH.x = width;
			TextureData[file_name].WH.y = height;
		}
	}
}

void TEXTURE_MANEGER::Load()
{
	UINT width, height;

	string path;			// ファイル名(パス付き) 
	string file_name;		// ファイル名(パスなし)
	string type;

	size_t pos;

	std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/texture");
	for (auto file : e) {

		// 一つ一つのファイル名(パス付き)
		path = file.path().string();

		// 置換
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		// 既に読み込んだテクスチャのスキップ
		if (TextureData.find(file_name) != TextureData.end())
			continue;

		// テクスチャ名の登録
		TextureFile[file_name].Path = path;
		TextureFile[file_name].Time = Get_File_Time(path);

		// テクスチャの読み込み
		{
			ID3D11ShaderResourceView* ShaderResourceView;
			HRESULT hr;
			wstring name;

			pos = file_name.find_last_of(".");
			type = file_name.substr(pos + 1);

			// char から wchar_t への変換
			name = stringTowstring("asset/texture/" + file_name);

			if ("dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView);
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

			TextureData[file_name].Resource.reset(ShaderResourceView);
			TextureData[file_name].WH.x = width;
			TextureData[file_name].WH.y = height;
		}
	}
}

void TEXTURE_MANEGER::Add(const string& const file_name, const float width, const float height)
{
	ID3D11ShaderResourceView* ShaderResourceView;

	// char から wchar_t への変換
	wstring file = stringTowstring(file_name);

	// 置換
	replace(file.begin(), file.end(), '\\', '/');

	HRESULT hr;

	hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), file.c_str(), nullptr, &ShaderResourceView, nullptr, nullptr);
	if (FAILED(hr))
	{
		return;
	}

	TextureData[file_name].Resource.reset(ShaderResourceView);
	TextureData[file_name].WH.x = width;
	TextureData[file_name].WH.y = height;

	//TextureResource[file_name].reset(ShaderResourceView);
}

const bool TEXTURE_MANEGER::Unload(const string& const file_name)
{
#ifdef _DEBUG
	if (TextureData[file_name].Cnt == 0)
	{
		// 参照しているこのがある
		return false;
	}
#endif // _DEBUG

	TextureData[file_name].Resource.reset(nullptr);
	TextureData.erase(file_name);

	return true;
}

DWORD TEXTURE_MANEGER::Get_File_Time(const string& path)
{
	HANDLE hFile;
	FILETIME ftFileTime, ftLocalFileTime;
	SYSTEMTIME stFileTime;

	hFile = CreateFile(
		path.c_str(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);

	GetFileTime(hFile, NULL, NULL, &ftFileTime);
	FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime);
	FileTimeToSystemTime(&ftLocalFileTime, &stFileTime);

	CloseHandle(hFile);

	// 日付をUNIXタイムスタンプに変換
	time_t rawtime;
	tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	timeinfo->tm_year = stFileTime.wYear - 1900;
	timeinfo->tm_mon = stFileTime.wMonth - 1;    //months since January - [0,11]
	timeinfo->tm_mday = stFileTime.wDay;          //day of the month - [1,31] 
	timeinfo->tm_hour = stFileTime.wHour;         //hours since midnight - [0,23]
	timeinfo->tm_min = stFileTime.wMinute;          //minutes after the hour - [0,59]
	timeinfo->tm_sec = stFileTime.wSecond;          //seconds after the minute - [0,59]

	return (DWORD)(mktime(timeinfo));
}

void TEXTURE_MANEGER::Add_ReferenceCnt(const string& const file_name)
{
	TextureData[file_name].Cnt++;
}

void TEXTURE_MANEGER::Sub_ReferenceCnt(const string& const file_name)
{
	TextureData[file_name].Cnt--;

#ifdef _DEBUG
	if (TextureData[file_name].Cnt < 0)
	{
		FAILDE_ASSERT;
		return;
	}
#endif // _DEBUG
}

XMINT2* const TEXTURE_MANEGER::Get_WH(const string& file_name)
{
	//for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	//{
	//	if (file_name == g_TextureFiles[i].Name)
	//	{
	//		return &g_TextureFiles[i].WH;
	//	}
	//}

	if (TextureData.find(file_name) != TextureData.end())
	{
		return &TextureData[file_name].WH;
	}

	return nullptr;
}

ID3D11ShaderResourceView* const TEXTURE_MANEGER::GetShaderResourceView(const string& const file_name)
{
	//for (auto tex = TextureResource.begin(); tex != TextureResource.end(); tex++)
	//{
	//	if (file_name == tex->first)
	//	{
	//		return tex->second.get();
	//	}
	//}

	if (TextureData.find(file_name) != TextureData.end())
	{
		return TextureData[file_name].Resource.get();
	}

	return nullptr;
}

const unordered_map<string, TEXTURE_FILE>& TEXTURE_MANEGER::Get_TextureFile()
{
	return TextureFile;
}