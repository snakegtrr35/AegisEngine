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

	// デフォルトの画像データの読み込み
	Texture_Manager->Default_Load(flag);

	// 画像データの読み込み
	Texture_Manager->Load(flag);

	//{
	//	// 対象のディレクトリを監視用にオープンする.
	//	// 共有ディレクトリ使用可、対象フォルダを削除可
	//	// 非同期I/O使用
	//	hDir = CreateFile(
	//		"./asset/texture", // 監視先
	//		FILE_LIST_DIRECTORY,
	//		FILE_SHARE_READ | FILE_SHARE_WRITE,
	//		nullptr,
	//		OPEN_EXISTING,
	//		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, // ReadDirectoryChangesW用
	//		nullptr
	//	);

	//	if (hDir == INVALID_HANDLE_VALUE) {
	//		FAILDE_ASSERT;
	//	}


	//	// 変更されたファイルのリストを記録するためのバッファ
	//	// 最初のReadDirectoryChangesWの通知から次のReadDirectoryChangesWまでの
	//	// 間に変更されたファイルの情報を格納できるだけのサイズが必要
	//	// バッファオーバーとしてもファイルに変更が発生したことは感知できるが、
	//	// なにが変更されたかは通知できない。
	//	std::vector<BYTE> buf(bufsiz);

	//	pBuf = &buf[0];

	//	// 非同期I/Oの完了待機用, 手動リセットモード。
	//	// 変更通知のイベント発報とキャンセル完了のイベント発報の
	//	// 2つのイベントソースがあるためイベントの流れが予想できず
	//	// 自動リセットイベントにするのは危険。
	//	hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	//	ResetEvent(hEvent);
	//}

	//{
	//	// 非同期I/O
	//	// ループ中でのみ使用・待機するので、ここ(スタック)に置いても安全.
	//	OVERLAPPED olp = { 0 };

	//	olp.hEvent = hEvent;

	//	// 監視条件 (FindFirstChangeNotificationと同じ)
	//	DWORD filter =
	//		//FILE_NOTIFY_CHANGE_FILE_NAME |  // ファイル名の変更
	//		//FILE_NOTIFY_CHANGE_DIR_NAME |  // ディレクトリ名の変更
	//		//FILE_NOTIFY_CHANGE_ATTRIBUTES |  // 属性の変更
	//		//FILE_NOTIFY_CHANGE_SIZE |  // サイズの変更
	//		FILE_NOTIFY_CHANGE_LAST_WRITE;    // 最終書き込み日時の変更

	//	// 変更を監視する.
	//	// 初回呼び出し時にシステムが指定サイズでバッファを確保し、そこに変更を記録する
	//	// 完了通知後もシステムは変更を追跡しており、後続のReadDirectoryChangeWの
	//	// 呼び出しで、前回通知後からの変更をまとめて受け取ることができる
	//	// バッファがあふれた場合はサイズ0で応答が返される
	//	ReadDirectoryChangesW(
	//		hDir,   // 対象ディレクトリ
	//		pBuf,   // 通知を格納するバッファ
	//		bufsiz, // バッファサイズ
	//		FALSE,   // サブディレクトリを対象にするか?
	//		filter, // 変更通知を受け取るフィルタ
	//		NULL,   // (結果サイズ, 非同期なので未使用)
	//		&olp,   // 非同期I/Oバッファ
	//		NULL    // (完了ルーチン, 未使用)
	//	);
	//}

#ifdef _DEBUG
	{
		Texture_Manager->monitor = make_unique<FileChangeMonitor>();
		Texture_Manager->monitor->init("./asset/texture");

		// 状況を更新
		Texture_Manager->monitor->readChanges();
	}
#endif // _DEBUG
}

void TEXTURE_MANEGER::Uninit()
{
#ifdef _DEBUG
	//解放
	// 解放する
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

	string path;			// ファイル名(パス付き)
	string file_name;		// ファイル名(パスなし)
	string type;
	size_t pos;
	DWORD time;				// ファイルの最終更新時間
	size_t first;			// 

	filesystem::directory_iterator e = filesystem::directory_iterator("./asset/texture");
	for (auto file : e)
	{
		// 一つ一つのファイル名(パス付き)
		path = file.path().string();

		// 置換
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		time = Get_File_Time(path);

		first = hash<string>()(file_name);//

		if (TextureFile.find(first) != TextureFile.end())
		{
			if (time != TextureFile[first].Time)
			{
				// ファイルが更新された
				wstring name;
				HRESULT hr;
				ID3D11ShaderResourceView* ShaderResourceView;

				pos = file_name.find_last_of(".");
				type = file_name.substr(pos + 1, 3);

				// char から wchar_t への変換
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
				else	// jpg か png
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
				TextureFile[first].Time = Get_File_Time(path);
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
		// 参照しているこのがある
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
		//	// 非同期I/O
		//	// ループ中でのみ使用・待機するので、ここ(スタック)に置いても安全.
		//	OVERLAPPED olp = { 0 };

		//	olp.hEvent = hEvent;

		//	// 監視条件 (FindFirstChangeNotificationと同じ)
		//	DWORD filter =
		//		//FILE_NOTIFY_CHANGE_FILE_NAME |  // ファイル名の変更
		//		//FILE_NOTIFY_CHANGE_DIR_NAME |  // ディレクトリ名の変更
		//		//FILE_NOTIFY_CHANGE_ATTRIBUTES |  // 属性の変更
		//		//FILE_NOTIFY_CHANGE_SIZE |  // サイズの変更
		//		FILE_NOTIFY_CHANGE_LAST_WRITE;    // 最終書き込み日時の変更

		//	// 変更を監視する.
		//	// 初回呼び出し時にシステムが指定サイズでバッファを確保し、そこに変更を記録する
		//	// 完了通知後もシステムは変更を追跡しており、後続のReadDirectoryChangeWの
		//	// 呼び出しで、前回通知後からの変更をまとめて受け取ることができる
		//	// バッファがあふれた場合はサイズ0で応答が返される
		//	if (FALSE == ReadDirectoryChangesW(
		//		hDir,   // 対象ディレクトリ
		//		pBuf,   // 通知を格納するバッファ
		//		bufsiz, // バッファサイズ
		//		FALSE,   // サブディレクトリを対象にするか?
		//		filter, // 変更通知を受け取るフィルタ
		//		NULL,   // (結果サイズ, 非同期なので未使用)
		//		&olp,   // 非同期I/Oバッファ
		//		NULL    // (完了ルーチン, 未使用)
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

		// 状況を更新
		monitor->readChanges();

		// 変更があればログとして吐き出す
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