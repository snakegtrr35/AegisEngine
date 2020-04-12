#pragma once

#ifndef TEXTURE_MANEGER_H
#define TEXTURE_MANEGER_H


#ifdef _DEBUG
#include	<queue>

class FILE_CHANGE_MONITOR {
private:
	string m_directoryName;
	const DWORD m_bufferSize = 1024 * 2;
	HANDLE m_directoryHandle;
	HANDLE m_eventHandle;
	vector<BYTE> m_buf;
	OVERLAPPED Olp;
	set<wstring> m_fileActions;

	// 変更の監視を開始
	bool beginReadChanges();

public:

	FILE_CHANGE_MONITOR(const string& directoryName);

	~FILE_CHANGE_MONITOR();

	bool Get_FileStack_Empty();

	// ファイル変更履歴キューから情報を取り出す
	wstring Pop_FileStack();

	// 初期化
	bool Init();

	// 変更を読み取る
	void readChanges();

	set<wstring>* Get();
};
#endif // _DEBUG





constexpr const int TEXTURE_SIZE_MAX = 16384;

struct TEXTURE_FILE {
	string Path;		//! テクスチャファイルのファイルパス

	TEXTURE_FILE() {}

	template<class T>
	void serialize(T& archive) {
		archive(Path);
	}
};

struct TEXTURE_DATA {
	unique_ptr<ID3D11ShaderResourceView, Release> Resource;		//! リソースデータ
	XMINT2 WH;													//!	テクスチャの幅と高さ
	UINT Cnt;											//! 参照回数

	TEXTURE_DATA() : WH(XMINT2(0, 0)), Cnt(0) {}
};

//========================================
// テクスチャマネージャークラス
//========================================
class TEXTURE_MANEGER {
private:

	static unique_ptr<TEXTURE_MANEGER> Texture_Manager;

	unordered_map<size_t, string> Default_Texture_File;			//! デフォルトのテクスチャのファイルパス
	unordered_map<size_t, TEXTURE_FILE> TextureFile;			//! テクスチャのファイルデータ
	unordered_map<size_t, TEXTURE_DATA> TextureData;			//! テクスチャデータ

	void Default_Load(const bool flag);							// デフォルトのテクスチャの読み込み
	void Load(const bool flag);									// テクスチャの読み込み

#ifdef _DEBUG
	void Load_Check();
#endif // _DEBUG

	std::mutex isLoadedMutex;

#ifdef _DEBUG
	unique_ptr<FILE_CHANGE_MONITOR> Monitor;
#endif // _DEBUG

public:
	TEXTURE_MANEGER() {};
	~TEXTURE_MANEGER() { Uninit(); }


	static bool Init();

	void Update();

	void Uninit();

	static TEXTURE_MANEGER* Get_Instance();

	void Add(const string& file_name);
	const bool Unload(const string& const file_name);

	void Add_ReferenceCnt(const size_t file);
	void Sub_ReferenceCnt(const size_t file);
	
	XMINT2* const Get_WH(const size_t file);

	ID3D11ShaderResourceView* const GetShaderResourceView(const size_t file);

	unordered_map<size_t, TEXTURE_FILE>& Get_TextureFile();

	const unordered_map<size_t, TEXTURE_DATA>::iterator Get_TextureData_Start();
	const unordered_map<size_t, TEXTURE_DATA>::iterator Get_TextureData_End();

	template<class T>
	void serialize(T& archive) {
		archive(Default_Texture_File);
		archive(TextureFile);
	}
};

#endif // ! TEXTURE_MANEGER_H