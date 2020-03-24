#pragma once

#ifndef TEXTURE_MANEGER_H
#define TEXTURE_MANEGER_H


#ifdef _DEBUG
#include	<queue>

enum class FileActionType
{
	kAdded = FILE_ACTION_ADDED,
	kRemoved = FILE_ACTION_REMOVED,
	kModified = FILE_ACTION_MODIFIED,
	kRenamedOld = FILE_ACTION_RENAMED_OLD_NAME,
	kRenamedNew = FILE_ACTION_RENAMED_NEW_NAME,
};

//struct FileAction
//{
//	FileActionType m_actionType;
//	wstring m_fileName;
//};

class FileChangeMonitor
{
	string m_directoryName;
	const size_t m_bufferSize = 1024 * 8;
	HANDLE m_directoryHandle = nullptr;
	HANDLE m_eventHandle = nullptr;
	std::vector<unsigned char> m_buf;
	OVERLAPPED m_olp;
	//std::queue<FileAction> m_fileActions;
	std::set<wstring> m_fileActions;
	//std::vector<FileAction> m_fileActions;

	// 変更の監視を開始
	bool beginReadChanges()
	{
		ResetEvent(m_eventHandle);

		m_olp = { 0 };
		m_olp.hEvent = m_eventHandle;

		const DWORD filter =
			/*FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_ATTRIBUTES |
			FILE_NOTIFY_CHANGE_SIZE |*/
			FILE_NOTIFY_CHANGE_LAST_WRITE;
		if (!ReadDirectoryChangesW(m_directoryHandle, &m_buf[0], m_bufferSize, TRUE, filter, nullptr, &m_olp, nullptr))
		{
			return false;
		}

		return true;
	}

public:

	// ファイル変更履歴キューの要素数を数える
	int getFileAcctionStackCount() const
	{
		return m_fileActions.size();
	}

	// ファイル変更履歴キューから情報を取り出す
	wstring popFileAcctionStack()
	{
		//auto file_action = m_fileActions.front();
		//m_fileActions.pop();
		auto file_action = m_fileActions.begin();
		m_fileActions.erase(m_fileActions.begin());
		return *file_action;
	}

	// 初期化
	bool init(const string& directoryName)
	{
		m_directoryName = directoryName;

		m_directoryHandle = CreateFile(m_directoryName.c_str(), FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
			OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr);

		if (m_directoryHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		m_buf.reserve(m_bufferSize);
		m_buf.resize(m_bufferSize);

		m_eventHandle = CreateEvent(nullptr, TRUE, FALSE, nullptr);

		beginReadChanges();

		return true;
	}

	// 解放
	void release() const
	{
		CancelIo(m_directoryHandle);
		WaitForSingleObject(m_eventHandle, INFINITE);

		CloseHandle(m_eventHandle);
		CloseHandle(m_directoryHandle);
	}

	// 変更を読み取る
	void readChanges()
	{
		auto waitResult = WaitForSingleObject(m_eventHandle, 0);
		if (waitResult == WAIT_TIMEOUT)
		{
			return;
		}

		DWORD retsize = 0;
		if (!GetOverlappedResult(m_directoryHandle, &m_olp, &retsize, FALSE))
		{
			beginReadChanges();
			return;
		}

		if (retsize == 0)
		{
			beginReadChanges();
			return;
		}
		auto pData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&m_buf[0]);

		while (true)
		{
			//FileAction action;
			//action.m_actionType = static_cast<FileActionType>(pData->Action);
			//action.m_fileName = wstring(pData->FileName).substr(0, pData->FileNameLength / sizeof(wchar_t));

			wstring m_fileName = wstring(pData->FileName).substr(0, pData->FileNameLength / sizeof(wchar_t));

			//m_fileActions.push(action);
			m_fileActions.insert(m_fileName);

			if (pData->NextEntryOffset == 0)
			{
				beginReadChanges();
				return;
			}
			pData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
				reinterpret_cast<unsigned char*>(pData) + pData->NextEntryOffset);
		}
	}
};
#endif // _DEBUG





constexpr const int TEXTURE_SIZE_MAX = 8192;

struct TEXTURE_FILE {
	string Path;		//! テクスチャファイルのファイルパス
	DWORD Time;			//!	テクスチャファイルの最終更新時間(UNIX時間)

	TEXTURE_FILE() : Time(0) {}

	template<class T>
	void serialize(T& archive) {
		archive(Path);
		archive(Time);
	}
};

struct TEXTURE_DATA {
	unique_ptr<ID3D11ShaderResourceView, Release> Resource;		//! リソースデータ
	XMINT2 WH;													//!	テクスチャの幅と高さ
	unsigned int Cnt;											//! 参照回数

	TEXTURE_DATA() : WH(XMINT2(0, 0)), Cnt(0) {}
};

//========================================
// テクスチャマネージャークラス
//========================================
class TEXTURE_MANEGER {
private:

	static unique_ptr<TEXTURE_MANEGER> Texture_Manager;

	//unordered_map<string, string> Default_Texture_File;		//! デフォルトのテクスチャのファイルパス
	//unordered_map<string, TEXTURE_FILE> TextureFile;			//! テクスチャのファイルデータ
	//unordered_map<string, TEXTURE_DATA> TextureData;			//! テクスチャデータ
	unordered_map<size_t, string> Default_Texture_File;			//! デフォルトのテクスチャのファイルパス
	unordered_map<size_t, TEXTURE_FILE> TextureFile;			//! テクスチャのファイルデータ
	unordered_map<size_t, TEXTURE_DATA> TextureData;			//! テクスチャデータ

	void Default_Load(const bool flag);		// デフォルトのテクスチャの読み込み
	void Load(const bool flag);				// テクスチャの読み込み

	DWORD Get_File_Time(const string& path);

#ifdef _DEBUG
	void Load_Check();
#endif // _DEBUG

	std::mutex isLoadedMutex;

	//WORD Time;
	bool Load_Flag;

#ifdef _DEBUG
	unique_ptr<FileChangeMonitor> monitor;
#endif // _DEBUG

public:
	TEXTURE_MANEGER() : /*Time(0),*/ Load_Flag(false) {};
	~TEXTURE_MANEGER() { Uninit(); }


	static void Init();

	void Update();

	void Uninit();

	static TEXTURE_MANEGER* Get_Instance();

	void Add(const string& file_name);
	const bool Unload(const string& const file_name);

	void Add_ReferenceCnt(const size_t file);
	void Sub_ReferenceCnt(const size_t file);
	
	XMINT2* const Get_WH(const size_t file);

	ID3D11ShaderResourceView* const GetShaderResourceView(const size_t file);

	//const unordered_map<string, TEXTURE_FILE>& Get_TextureFile();
	unordered_map<size_t, TEXTURE_FILE>& Get_TextureFile();

	//const unordered_map<string, TEXTURE_DATA>& Get_TextureData();
	const unordered_map<size_t, TEXTURE_DATA>::iterator Get_TextureData_Start();
	const unordered_map<size_t, TEXTURE_DATA>::iterator Get_TextureData_End();

	template<class T>
	void serialize(T& archive) {
		archive(Default_Texture_File);
		archive(TextureFile);
	}
};

#endif // ! TEXTURE_MANEGER_H