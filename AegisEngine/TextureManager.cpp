#include "TextureManager.h"
#include "manager.h"

#include "TextureImporter.h"
#include "include/engine/core/AllocatorWrapper.h"

using namespace aegis;

std::unique_ptr<TextureManager> TextureManager::Texture_Manager;

bool TextureManager::Init()
{
	/*{
		std::unique_ptr<aegis::render::TextureData> temp;
		temp.reset(new aegis::render::TextureData);

		temp->ShaderResourceView.reset(aegis::allocatorWrapper::allocate<aegis::ShaderResourceView>());

		temp.reset();
	}*/

	if (nullptr == Texture_Manager.get())
	{
		Texture_Manager = std::make_unique<TextureManager>();
	}

	bool flag;

	{
		std::ifstream file("texture.dat", std::ios::binary);

		flag = file.is_open();

		if (flag)
		{
			uint64 size = std::filesystem::file_size("texture.dat");

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
		Texture_Manager->Monitor = std::make_unique<FILE_CHANGE_MONITOR>("./asset/texture");
		if (false == Texture_Manager->Monitor->Init())
		{
			return false;
		}
	}
#endif // _DEBUG

	return true;
}

void TextureManager::Uninit()
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

	///for (auto tex = TextureData.begin(); tex != TextureData.end(); tex++)
	///{
	///	tex->second->;
	///}
	TextureData.clear();
}

void TextureManager::Update()
{
#ifdef _DEBUG
	Load_Check();

	CRenderer* render = CRenderer::getInstance();

	aegis::wstring path;			// ファイル名(パス付き)
	aegis::wstring file_name;		// ファイル名(パスなし)
	aegis::wstring type;
	uint64 pos;
	uint64 first;			// 

	while (false == Monitor->Get_FileStack_Empty())
	{
		file_name = Monitor->Pop_FileStack();

		path = L"./asset/texture/" + file_name;

		{
			// ファイルが更新された

			pos = file_name.find_last_of(L".");
			type = file_name.substr(pos + 1);

			TextureImporter* textureImporter = TextureImporter::getInstance();

			first = std::hash<aegis::string>()(wstringTostring(file_name).c_str());

			aegis::render::TextureData* texture = aegis::allocatorWrapper::allocate<aegis::render::TextureData>();
			texture->ShaderResourceView.reset(textureImporter->GetShaderResourceView(path));

			TextureData.try_emplace(first, texture);
		}
	}
#endif // _DEBUG
}

void TextureManager::Default_Load(const bool flag)
{
	CRenderer* render = CRenderer::getInstance();

	int width = 0, height = 0;
	aegis::string path;			// ファイル名(パス付き) 
	aegis::string file_name;		// ファイル名(パスなし)
	aegis::string type;
	uint64 first;			// 
	uint64 pos;

	aegis::wstring name;

	std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/Default/texture");
	for (auto file : e) {

		// 一つ一つのファイル名(パス付き)
		path = file.path().string();

		// 置換
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		first = std::hash<aegis::string>()(file_name);//

		// バイナリファイルがない
		if (false == flag)
		{
			// テクスチャの登録
			Default_Texture_File[first] = path;

			TextureData[first]->Cnt = 0;
		}

		// テクスチャの読み込み
		{
			pos = file_name.find_last_of(".");
			type = file_name.substr(pos + 1);

			HRESULT hr;

			// char から wchar_t への変換
			name = stringTowstring(path);

			TextureImporter* textureImporter = TextureImporter::getInstance();
			aegis::render::TextureData* texture = aegis::allocatorWrapper::allocate<aegis::render::TextureData>();
			texture->ShaderResourceView.reset(textureImporter->GetShaderResourceView(name));
			textureImporter->GetSize(name, &width, &height);

			TextureData.try_emplace(first, texture);
			TextureData[first]->WH.x = width;
			TextureData[first]->WH.y = height;
		}
	}
}

void TextureManager::Load(const bool flag)
{
	CRenderer* render = CRenderer::getInstance();

	// バイナリファイルがない
	if (false == flag)
	{
		int width = 0, height = 0;
		aegis::string path;			// ファイル名(パス付き) 
		aegis::string file_name;		// ファイル名(パスなし)
		aegis::string type;
		uint64 first;			// 
		uint64 pos;

		const std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/texture");
		for (auto& file : e)
		{
			// 一つ一つのファイル名(パス付き)
			path = file.path().string();

			// 置換
			replace(path.begin(), path.end(), '\\', '/');

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			first = std::hash<aegis::string>()(file_name);//

			//バイナリファイルがない
			{
				//テクスチャの登録
				TextureFile[first].Path = path;
			}

			// テクスチャの読み込み
			if (TextureFile.find(first) != TextureFile.end())
			{
				aegis::wstring name;

				pos = file_name.find_last_of(".");
				type = file_name.substr(pos + 1);

				// char から wchar_t への変換
				name = stringTowstring(path);

				TextureImporter* textureImporter = TextureImporter::getInstance();

				aegis::render::TextureData* texture = aegis::allocatorWrapper::allocate<aegis::render::TextureData>();
				texture->ShaderResourceView.reset(textureImporter->GetShaderResourceView(name));
				textureImporter->GetSize(name, &width, &height);

				TextureData.try_emplace(first, texture);
				auto& data = TextureData.at(first);
				data->WH.x = width;
				data->WH.y = height;
				data->Cnt = 0;
			}
		}
	}
	else
	{
		int width = 0, height = 0;
		aegis::string path;			// ファイル名(パス付き) 
		aegis::string file_name;		// ファイル名(パスなし)
		aegis::string type;
		uint64 first;
		uint64 pos;

		aegis::wstring name;

		for (auto f : TextureFile)
		{
			path = f.second.Path;

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			first = std::hash<aegis::string>()(file_name);//

			pos = path.find_last_of(".");
			type = path.substr(pos + 1);

			// char から wchar_t への変換
			name = stringTowstring(path);

			TextureImporter* textureImporter = TextureImporter::getInstance();
			aegis::render::TextureData* texture = aegis::allocatorWrapper::allocate<aegis::render::TextureData>();
			texture->ShaderResourceView.reset(textureImporter->GetShaderResourceView(name));
			textureImporter->GetSize(name, &width, &height);

			TextureData.try_emplace(f.first, texture);
			auto& data = TextureData.at(first);
			data->WH.x = width;
			data->WH.y = height;
			data->Cnt = 0;
		}
	}
}

void TextureManager::Add(const aegis::string& file_name)
{
	CRenderer* render = CRenderer::getInstance();

	int width = 0, height = 0;

	aegis::string path;	// ファイル名
	aegis::string type;
	uint64 first;			// 
	uint64 pos;

	pos = file_name.find_last_of("/");

	path = file_name.substr(pos + 1);

	// テクスチャの読み込み
	{
		aegis::wstring name;

		pos = path.find_last_of(".");
		type = path.substr(pos + 1);

		// char から wchar_t への変換
		name = stringTowstring("asset/texture/" + path);

		TextureImporter* textureImporter = TextureImporter::getInstance();

		first = std::hash<aegis::string>()(file_name);
		TextureFile[first].Path = "asset/texture/" + path;

		aegis::render::TextureData* texture = aegis::allocatorWrapper::allocate<aegis::render::TextureData>();
		texture->ShaderResourceView.reset(textureImporter->GetShaderResourceView(name));
		textureImporter->GetSize(name, &width, &height);

		TextureData.try_emplace(first, texture);
		auto& data = TextureData.at(first);
		data->WH.x = width;
		data->WH.y = height;
		data->Cnt = 0;
	}
}

const bool TextureManager::Unload(const aegis::string& const file_name)
{
	uint64 first = std::hash<aegis::string>()(file_name);//

#ifdef _DEBUG
	if (0 != TextureData[first]->Cnt)
	{
		// 参照しているものがある
		return false;
	}
#endif // _DEBUG

	///TextureData[first]->Reset();

	TextureData.erase(first);

	TextureFile.erase(first);

	return true;
}

#ifdef _DEBUG
#include "Timer.h"
double fps = 0.0;

void TextureManager::Load_Check()
{
	fps += Timer::Get_DeltaTime();

	if (10.0 < fps)
	{
		fps = 0.0;

		// 状況を更新
		Monitor->readChanges();
	}
}
#endif // _DEBUG

void TextureManager::Add_ReferenceCnt(const uint64 file)
{
	TextureData[file]->Cnt++;
}

void TextureManager::Sub_ReferenceCnt(const uint64 file)
{
	TextureData[file]->Cnt--;

#ifdef _DEBUG
	if (TextureData[file]->Cnt < 0)
	{
		FAILDE_ASSERT;
		return;
	}
#endif // _DEBUG
}

Int2* const TextureManager::Get_WH(const uint64 file)
{
	if (TextureData.find(file) != TextureData.end())
	{
		return &TextureData[file]->WH;
	}

	return nullptr;
}

aegis::ShaderResourceView* TextureManager::GetShaderResourceView(const uint64 file)
{
	if (TextureData.find(file) != TextureData.end())
	{
		return TextureData[file]->ShaderResourceView.get();
	}

	return nullptr;
}

unordered_map<uint64, TEXTURE_FILE>& TextureManager::Get_TextureFile()
{
	return TextureFile;
}

const unordered_map<uint64, aegis::uniquePtr<aegis::render::TextureData>>::iterator TextureManager::Get_TextureData_Start()
{
	return TextureData.begin();
}

const unordered_map<uint64, aegis::uniquePtr<aegis::render::TextureData>>::iterator TextureManager::Get_TextureData_End()
{
	return TextureData.end();
}


TextureManager* TextureManager::Get_Instance()
{
	return Texture_Manager.get();
}