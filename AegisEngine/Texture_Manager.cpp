#include	"Texture_Manager.h"
#include	"Renderer.h"
#include	"manager.h"

#include	"Library/DirectXTex/WICTextureLoader.h"
#include	"Library/DirectXTex/DDSTextureLoader.h"

unique_ptr<TEXTURE_MANEGER> TEXTURE_MANEGER::Texture_Manager;

//static HANDLE hDir;
//static void* pBuf;
//static HANDLE hEvent;
//static size_t bufsiz = 512;

void TEXTURE_MANEGER::Init()
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

	// �f�t�H���g�̉摜�f�[�^�̓ǂݍ���
	Texture_Manager->Default_Load(flag);

	// �摜�f�[�^�̓ǂݍ���
	Texture_Manager->Load(flag);

	//{
	//	// �Ώۂ̃f�B���N�g�����Ď��p�ɃI�[�v������.
	//	// ���L�f�B���N�g���g�p�A�Ώۃt�H���_���폜��
	//	// �񓯊�I/O�g�p
	//	hDir = CreateFile(
	//		"./asset/texture", // �Ď���
	//		FILE_LIST_DIRECTORY,
	//		FILE_SHARE_READ | FILE_SHARE_WRITE,
	//		nullptr,
	//		OPEN_EXISTING,
	//		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, // ReadDirectoryChangesW�p
	//		nullptr
	//	);

	//	if (hDir == INVALID_HANDLE_VALUE) {
	//		FAILDE_ASSERT;
	//	}


	//	// �ύX���ꂽ�t�@�C���̃��X�g���L�^���邽�߂̃o�b�t�@
	//	// �ŏ���ReadDirectoryChangesW�̒ʒm���玟��ReadDirectoryChangesW�܂ł�
	//	// �ԂɕύX���ꂽ�t�@�C���̏����i�[�ł��邾���̃T�C�Y���K�v
	//	// �o�b�t�@�I�[�o�[�Ƃ��Ă��t�@�C���ɕύX�������������Ƃ͊��m�ł��邪�A
	//	// �Ȃɂ��ύX���ꂽ���͒ʒm�ł��Ȃ��B
	//	std::vector<BYTE> buf(bufsiz);

	//	pBuf = &buf[0];

	//	// �񓯊�I/O�̊����ҋ@�p, �蓮���Z�b�g���[�h�B
	//	// �ύX�ʒm�̃C�x���g����ƃL�����Z�������̃C�x���g�����
	//	// 2�̃C�x���g�\�[�X�����邽�߃C�x���g�̗��ꂪ�\�z�ł���
	//	// �������Z�b�g�C�x���g�ɂ���̂͊댯�B
	//	hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	//	ResetEvent(hEvent);
	//}

	//{
	//	// �񓯊�I/O
	//	// ���[�v���ł̂ݎg�p�E�ҋ@����̂ŁA����(�X�^�b�N)�ɒu���Ă����S.
	//	OVERLAPPED olp = { 0 };

	//	olp.hEvent = hEvent;

	//	// �Ď����� (FindFirstChangeNotification�Ɠ���)
	//	DWORD filter =
	//		//FILE_NOTIFY_CHANGE_FILE_NAME |  // �t�@�C�����̕ύX
	//		//FILE_NOTIFY_CHANGE_DIR_NAME |  // �f�B���N�g�����̕ύX
	//		//FILE_NOTIFY_CHANGE_ATTRIBUTES |  // �����̕ύX
	//		//FILE_NOTIFY_CHANGE_SIZE |  // �T�C�Y�̕ύX
	//		FILE_NOTIFY_CHANGE_LAST_WRITE;    // �ŏI�������ݓ����̕ύX

	//	// �ύX���Ď�����.
	//	// ����Ăяo�����ɃV�X�e�����w��T�C�Y�Ńo�b�t�@���m�ۂ��A�����ɕύX���L�^����
	//	// �����ʒm����V�X�e���͕ύX��ǐՂ��Ă���A�㑱��ReadDirectoryChangeW��
	//	// �Ăяo���ŁA�O��ʒm�ォ��̕ύX���܂Ƃ߂Ď󂯎�邱�Ƃ��ł���
	//	// �o�b�t�@�����ӂꂽ�ꍇ�̓T�C�Y0�ŉ������Ԃ����
	//	ReadDirectoryChangesW(
	//		hDir,   // �Ώۃf�B���N�g��
	//		pBuf,   // �ʒm���i�[����o�b�t�@
	//		bufsiz, // �o�b�t�@�T�C�Y
	//		FALSE,   // �T�u�f�B���N�g����Ώۂɂ��邩?
	//		filter, // �ύX�ʒm���󂯎��t�B���^
	//		NULL,   // (���ʃT�C�Y, �񓯊��Ȃ̂Ŗ��g�p)
	//		&olp,   // �񓯊�I/O�o�b�t�@
	//		NULL    // (�������[�`��, ���g�p)
	//	);
	//}

#ifdef _DEBUG
	{
		Texture_Manager->monitor = make_unique<FileChangeMonitor>();
		Texture_Manager->monitor->init("./asset/texture");

		// �󋵂��X�V
		Texture_Manager->monitor->readChanges();
	}
#endif // _DEBUG
}

void TEXTURE_MANEGER::Uninit()
{
#ifdef _DEBUG
	//���
	// �������
	monitor->release();
	//CancelIo(hDir);

	//CloseHandle(hEvent);
	//CloseHandle(hDir);
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
#endif // _DEBUG

	string path;			// �t�@�C����(�p�X�t��)
	string file_name;		// �t�@�C����(�p�X�Ȃ�)
	string type;
	size_t pos;
	DWORD time;				// �t�@�C���̍ŏI�X�V����
	size_t first;			// 

	filesystem::directory_iterator e = filesystem::directory_iterator("./asset/texture");
	for (auto file : e)
	{
		// ���̃t�@�C����(�p�X�t��)
		path = file.path().string();

		// �u��
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		time = Get_File_Time(path);

		first = hash<string>()(file_name);//

		if (TextureFile.find(first) != TextureFile.end())
		{
			if (time != TextureFile[first].Time)
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

				//TextureFile[file_name].Time = time;
				//TextureData[file_name].Resource.reset(ShaderResourceView);

				TextureFile[first].Time = time;//
				TextureData[first].Resource.reset(ShaderResourceView);//
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
	size_t first;			// 
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

		first = hash<string>()(file_name);//

		// �o�C�i���t�@�C�����Ȃ�
		if (false == flag)
		{
			// �e�N�X�`���̓o�^
			Default_Texture_File[first] = path;

			TextureData[first].Cnt = 0;
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

			//TextureData[file_name].Resource.reset(ShaderResourceView);
			//TextureData[file_name].WH.x = width;
			//TextureData[file_name].WH.y = height;

			TextureData[first].Resource.reset(ShaderResourceView);//
			TextureData[first].WH.x = width;//
			TextureData[first].WH.y = height;//
		}
	}
}

void TEXTURE_MANEGER::Load(const bool flag)
{
	// �o�C�i���t�@�C�����Ȃ�
	if (false == flag)
	{

		int width, height;

		string path;			// �t�@�C����(�p�X�t��) 
		string file_name;		// �t�@�C����(�p�X�Ȃ�)
		string type;
		size_t first;			// 
		size_t pos;

		std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/texture");
		for (auto file : e)
		{
			// ���̃t�@�C����(�p�X�t��)
			path = file.path().string();

			// �u��
			replace(path.begin(), path.end(), '\\', '/');

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			first = hash<string>()(file_name);//

			//�o�C�i���t�@�C�����Ȃ�
			{
				//�e�N�X�`���̓o�^
				TextureFile[first].Path = path;
				TextureFile[first].Time = Get_File_Time(path);
			}

			// �e�N�X�`���̓ǂݍ���
			if (TextureFile.find(first) != TextureFile.end())
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

				//TextureData[file_name].Resource.reset(ShaderResourceView);
				//TextureData[file_name].WH.x = width;
				//TextureData[file_name].WH.y = height;
				//TextureData[file_name].Cnt = 0;

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
		string path;			// �t�@�C����(�p�X�t��) 
		string file_name;		// �t�@�C����(�p�X�Ȃ�)
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

	string path;	// �t�@�C����
	string type;
	size_t first;			// 
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

			//TextureFile[file_name].Path = "asset/texture/" + path;
			//TextureFile[file_name].Time = Get_File_Time(path);

			//TextureData[file_name].Resource.reset(ShaderResourceView);
			//TextureData[file_name].WH.x = width;
			//TextureData[file_name].WH.y = height;

			first = hash<string>()(file_name);//
			TextureFile[first].Path = "asset/texture/" + path;
			TextureFile[first].Time = Get_File_Time(path);

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
	if (TextureData[first].Cnt != 0)
	{
		// �Q�Ƃ��Ă��邱�̂�����
		return false;
	}
#endif // _DEBUG

	TextureData[first].Resource.reset(nullptr);
	//TextureData[first].Resource.release();

	TextureData.erase(first);

	TextureFile.erase(first);

	return true;
}

#include	"Timer.h"
double fps = 0.0;

#ifdef _DEBUG
void TEXTURE_MANEGER::Load_Check()
{
	fps += TIMER::Get_DeltaTime();

	if (20.0 < fps)
	{
		fps = 0.0;

		//{
		//	// �񓯊�I/O
		//	// ���[�v���ł̂ݎg�p�E�ҋ@����̂ŁA����(�X�^�b�N)�ɒu���Ă����S.
		//	OVERLAPPED olp = { 0 };

		//	olp.hEvent = hEvent;

		//	// �Ď����� (FindFirstChangeNotification�Ɠ���)
		//	DWORD filter =
		//		//FILE_NOTIFY_CHANGE_FILE_NAME |  // �t�@�C�����̕ύX
		//		//FILE_NOTIFY_CHANGE_DIR_NAME |  // �f�B���N�g�����̕ύX
		//		//FILE_NOTIFY_CHANGE_ATTRIBUTES |  // �����̕ύX
		//		//FILE_NOTIFY_CHANGE_SIZE |  // �T�C�Y�̕ύX
		//		FILE_NOTIFY_CHANGE_LAST_WRITE;    // �ŏI�������ݓ����̕ύX

		//	// �ύX���Ď�����.
		//	// ����Ăяo�����ɃV�X�e�����w��T�C�Y�Ńo�b�t�@���m�ۂ��A�����ɕύX���L�^����
		//	// �����ʒm����V�X�e���͕ύX��ǐՂ��Ă���A�㑱��ReadDirectoryChangeW��
		//	// �Ăяo���ŁA�O��ʒm�ォ��̕ύX���܂Ƃ߂Ď󂯎�邱�Ƃ��ł���
		//	// �o�b�t�@�����ӂꂽ�ꍇ�̓T�C�Y0�ŉ������Ԃ����
		//	if (FALSE == ReadDirectoryChangesW(
		//		hDir,   // �Ώۃf�B���N�g��
		//		pBuf,   // �ʒm���i�[����o�b�t�@
		//		bufsiz, // �o�b�t�@�T�C�Y
		//		FALSE,   // �T�u�f�B���N�g����Ώۂɂ��邩?
		//		filter, // �ύX�ʒm���󂯎��t�B���^
		//		NULL,   // (���ʃT�C�Y, �񓯊��Ȃ̂Ŗ��g�p)
		//		&olp,   // �񓯊�I/O�o�b�t�@
		//		NULL    // (�������[�`��, ���g�p)
		//	))
		//	{
		//		return;
		//	}

		//	wstring FileName;

		//	FILE_NOTIFY_INFORMATION* pInfo = (FILE_NOTIFY_INFORMATION*)pBuf;
		//	switch (pInfo->Action)
		//	{
		//	case FILE_ACTION_MODIFIED:
		//		FileName = wstring(pInfo->FileName);

		//		FileName.pop_back();

		//		Load_Flag = true;

		//		break;
		//	}
		//}

		int a;

		// �󋵂��X�V
		monitor->readChanges();

		// �ύX������΃��O�Ƃ��ēf���o��
		while (monitor->getFileAcctionStackCount() > 0)
		{
			auto fileAction = monitor->popFileAcctionStack();
			wstring actionType = L"";

			/*switch (fileAction.m_actionType)
			{
			case FileActionType::kAdded:
				actionType = L"Added";

				a = 0;

				break;
			case FileActionType::kRemoved:
				actionType = L"Removed";

				a = 0;

				break;
			case FileActionType::kModified:
				actionType = L"Modified";

				a = 0;

				break;
			case FileActionType::kRenamedOld:
				actionType = L"RenamedOld";

				a = 0;

				break;
			case FileActionType::kRenamedNew:
				actionType = L"RenamedNew";

				a = 0;

				break;
			default:
				break;
			}*/
		}
	}
}
#endif // _DEBUG

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
	//for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	//{
	//	if (file_name == g_TextureFiles[i].Name)
	//	{
	//		return &g_TextureFiles[i].WH;
	//	}
	//}

	if (TextureData.find(file) != TextureData.end())
	{
		return &TextureData[file].WH;
	}

	return nullptr;
}

ID3D11ShaderResourceView* const TEXTURE_MANEGER::GetShaderResourceView(const size_t file)
{
	//for (auto tex = TextureResource.begin(); tex != TextureResource.end(); tex++)
	//{
	//	if (file_name == tex->first)
	//	{
	//		return tex->second.get();
	//	}
	//}

	if (TextureData.find(file) != TextureData.end())
	{
		return TextureData[file].Resource.get();
	}

	return nullptr;
}

//unordered_map<string, TEXTURE_FILE>& TEXTURE_MANEGER::Get_TextureFile()
//{
//	return TextureFile;
//}
unordered_map<size_t, TEXTURE_FILE>& TEXTURE_MANEGER::Get_TextureFile()
{
	return TextureFile;
}

//unordered_map<string, TEXTURE_DATA>::iterator TEXTURE_MANEGER::Get_TextureData_Start()
//{
//	return TextureData.begin();
//}
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