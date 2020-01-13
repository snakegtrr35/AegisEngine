#include	"Texture_Manager.h"
#include	"Renderer.h"

#include	"Library/DirectXTex/WICTextureLoader.h"
#include	"Library/DirectXTex/DDSTextureLoader.h"

unordered_map<string, string>			TEXTURE_MANEGER::Default_Texture_File;
unordered_map<string, TEXTURE_FILE>		TEXTURE_MANEGER::TextureFile;
unordered_map<string, TEXTURE_DATA>		TEXTURE_MANEGER::TextureData;


void TEXTURE_MANEGER::Init()
{
	bool flag;

	{
		TEXTURE_MANEGER t;

		std::ifstream file("texture.dat", std::ios::binary);

		flag = file.is_open();

		if (flag)
		{
			cereal::BinaryInputArchive archive(file);
			archive(t);
		}
	}

	// �f�t�H���g�̉摜�f�[�^�̓ǂݍ���
	Default_Load(flag);

	// �摜�f�[�^�̓ǂݍ���
	Load(flag);
}

void TEXTURE_MANEGER::Uninit()
{
	{
		TEXTURE_MANEGER t;

		std::ofstream file("texture.dat", std::ios::binary);

		cereal::BinaryOutputArchive archive(file);
		archive(t);
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
	string path;			// �t�@�C����(�p�X�t��)
	string file_name;		// �t�@�C����(�p�X�Ȃ�)
	string type;
	size_t pos;
	DWORD time;				// �t�@�C���̍ŏI�X�V����

	std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/texture");
	for (auto file : e)
	{
		// ���̃t�@�C����(�p�X�t��)
		path = file.path().string();

		// �u��
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		time = Get_File_Time(path);

		if (TextureFile.find(file_name) != TextureFile.end())
		{
			if (time != TextureFile[file_name].Time)
			{
				// �t�@�C�����X�V���ꂽ
				wstring name;
				HRESULT hr;
				ID3D11ShaderResourceView* ShaderResourceView;

				pos = file_name.find_last_of(".");
				type = file_name.substr(pos + 1, 3);

				// char ���� wchar_t �ւ̕ϊ�
				name = stringTowstring("asset/texture/" + file_name);

				if ("dds" == type)	// dds
				{
					hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, nullptr, nullptr);
					if (FAILED(hr))
					{
						FAILDE_ASSERT;
						return;
					}
				}
				else	// jpg �� png
				{
					hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, nullptr, nullptr);
					if (FAILED(hr))
					{
						FAILDE_ASSERT;
						return;
					}
				}

				TextureFile[file_name].Time = time;

				TextureData[file_name].Resource.reset(ShaderResourceView);
			}
		}
	}
}

void TEXTURE_MANEGER::Default_Load(const bool flag)
{
	int width, height;

	string path;			// �t�@�C����(�p�X�t��) 
	string file_name;		// �t�@�C����(�p�X�Ȃ�)
	string type;

	size_t pos;

	ID3D11ShaderResourceView* ShaderResourceView;
	wstring name;

	std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/Default/texture");
	for (auto file : e) {

		// ���̃t�@�C����(�p�X�t��)
		path = file.path().string();

		// �u��
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		// �o�C�i���t�@�C�����Ȃ�
		if (false == flag)
		{
			// �e�N�X�`���̓o�^
			Default_Texture_File[file_name] = path;

			TextureData[file_name].Cnt = 0;
		}

		// �e�N�X�`���̓ǂݍ���
		{
			pos = file_name.find_last_of(".");
			type = file_name.substr(pos + 1);

			HRESULT hr;

			// char ���� wchar_t �ւ̕ϊ�
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
			else	// jpg �� png
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

void TEXTURE_MANEGER::Load(const bool flag)
{
	// �o�C�i���t�@�C��������
	if (true == flag)
	{

		int width, height;

		string path;			// �t�@�C����(�p�X�t��) 
		string file_name;		// �t�@�C����(�p�X�Ȃ�)
		string type;

		size_t pos;

		std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/texture");
		for (auto file : e) {

			// ���̃t�@�C����(�p�X�t��)
			path = file.path().string();

			// �u��
			replace(path.begin(), path.end(), '\\', '/');

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			////�o�C�i���t�@�C�����Ȃ�
			//if (false == flag)
			//{
			//	//�e�N�X�`���̓o�^
			//	TextureFile[file_name].Path = path;
			//	TextureFile[file_name].Time = Get_File_Time(path);

			//	TextureData[file_name].Cnt = 0;
			//}

			// �e�N�X�`���̓ǂݍ���
			if (TextureFile.find(file_name) != TextureFile.end())
			{
				ID3D11ShaderResourceView* ShaderResourceView;
				HRESULT hr;
				wstring name;

				pos = file_name.find_last_of(".");
				type = file_name.substr(pos + 1);

				// char ���� wchar_t �ւ̕ϊ�
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
				else	// jpg �� png
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
				TextureData[file_name].Cnt = 0;
			}
		}
	}
}

void TEXTURE_MANEGER::Add(const string& file_name)
{
	int width, height;

	string path;	// �t�@�C����
	string type;

	size_t pos;

	{
		pos = file_name.find_last_of("/");

		path = file_name.substr(pos + 1);

		// �e�N�X�`���̓ǂݍ���
		{
			ID3D11ShaderResourceView* ShaderResourceView;
			HRESULT hr;
			wstring name;

			pos = path.find_last_of(".");
			type = path.substr(pos + 1);

			// char ���� wchar_t �ւ̕ϊ�
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
			else	// jpg �� png
			{
				hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}

			TextureFile[file_name].Path = "asset/texture/" + path;
			TextureFile[file_name].Time = Get_File_Time(path);

			TextureData[file_name].Resource.reset(ShaderResourceView);
			TextureData[file_name].WH.x = width;
			TextureData[file_name].WH.y = height;
		}
	}
}

const bool TEXTURE_MANEGER::Unload(const string& const file_name)
{
#ifdef _DEBUG
	if (TextureData[file_name].Cnt != 0)
	{
		// �Q�Ƃ��Ă��邱�̂�����
		return false;
	}
#endif // _DEBUG

	TextureData[file_name].Resource.reset(nullptr);
	TextureData[file_name].Resource.release();

	TextureData.erase(file_name);

	TextureFile.erase(file_name);

	return true;
}

DWORD TEXTURE_MANEGER::Get_File_Time(const string& path)
{
	HANDLE hFile;
	FILETIME ftFileTime, ftLocalFileTime;
	SYSTEMTIME stFileTime;

	hFile = CreateFile(
		path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);

	GetFileTime(hFile, NULL, NULL, &ftFileTime);
	FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime);
	FileTimeToSystemTime(&ftLocalFileTime, &stFileTime);

	CloseHandle(hFile);

	// ���t��UNIX�^�C���X�^���v�ɕϊ�
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

unordered_map<string, TEXTURE_FILE>& TEXTURE_MANEGER::Get_TextureFile()
{
	return TextureFile;
}

unordered_map<string, TEXTURE_DATA>::iterator TEXTURE_MANEGER::Get_TextureData_Start()
{
	return TextureData.begin();
}

unordered_map<string, TEXTURE_DATA>::iterator TEXTURE_MANEGER::Get_TextureData_End()
{
	return TextureData.end();
}