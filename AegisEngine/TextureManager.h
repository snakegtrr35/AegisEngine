#pragma once

#ifndef TEXTURE_MANEGER_H
#define TEXTURE_MANEGER_H


#ifdef _DEBUG
#include <queue>
#include "Renderer.h"
#include "TextureTypeDefine.h"

class FILE_CHANGE_MONITOR {
private:
#ifdef UNICODE
	aegis::wstring m_directoryName;
#else
	aegis::string m_directoryName;
#endif // !UNICODE
	const DWORD m_bufferSize = 1024 * 2;
	HANDLE m_directoryHandle;
	HANDLE m_eventHandle;
	aegis::vector<BYTE> m_buf;
	OVERLAPPED Olp;
	aegis::unordered_set<aegis::wstring> m_fileActions;

	// 変更の監視を開始
	bool beginReadChanges();

public:

	FILE_CHANGE_MONITOR(const aegis::string& directoryName);

	~FILE_CHANGE_MONITOR();

	bool Get_FileStack_Empty();

	// ファイル変更履歴キューから情報を取り出す
	aegis::wstring Pop_FileStack();

	// 初期化
	bool Init();

	// 変更を読み取る
	void readChanges();

	aegis::unordered_set<aegis::wstring>* Get();
};
#endif // _DEBUG





constexpr const int TEXTURE_SIZE_MAX = 16384;

struct TEXTURE_FILE {
	aegis::string Path;		//! テクスチャファイルのファイルパス

	TEXTURE_FILE() {}

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Path", aegis::string(Path)));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		aegis::string s;
		archive(cereal::make_nvp("Path", s));
		Path.reserve(s.size());
		Path = s;
	}
};

//========================================
// テクスチャマネージャークラス
//========================================
class TextureManager {
private:

	static std::unique_ptr<TextureManager> Texture_Manager;

	aegis::unordered_map<aegis::uint64, aegis::string> Default_Texture_File;			//! デフォルトのテクスチャのファイルパス
	aegis::unordered_map<aegis::uint64, TEXTURE_FILE> TextureFile;			//! テクスチャのファイルデータ
	aegis::unordered_map<aegis::uint64, aegis::uniquePtr<aegis::render::TextureData>> TextureData;			//! テクスチャデータ

	void Default_Load(const bool flag);							// デフォルトのテクスチャの読み込み
	void Load(const bool flag);									// テクスチャの読み込み

#ifdef _DEBUG
	void Load_Check();
#endif // _DEBUG

	std::mutex isLoadedMutex;

#ifdef _DEBUG
	std::unique_ptr<FILE_CHANGE_MONITOR> Monitor;
#endif // _DEBUG

public:
	TextureManager() {};
	~TextureManager() { Uninit(); }


	static bool Init();

	void Update();

	void Uninit();

	static TextureManager* Get_Instance();

	void Add(const aegis::string& file_name);
	const bool Unload(const aegis::string& const file_name);

	void Add_ReferenceCnt(const aegis::uint64 file);
	void Sub_ReferenceCnt(const aegis::uint64 file);

	aegis::Int2* const Get_WH(const aegis::uint64 file);

	aegis::ShaderResourceView* GetShaderResourceView(const aegis::uint64 file);

	aegis::unordered_map<aegis::uint64, TEXTURE_FILE>& Get_TextureFile();

	const aegis::unordered_map<aegis::uint64, aegis::uniquePtr<aegis::render::TextureData>>::iterator Get_TextureData_Start();
	const aegis::unordered_map<aegis::uint64, aegis::uniquePtr<aegis::render::TextureData>>::iterator Get_TextureData_End();

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Default_Texture_File", Default_Texture_File),
				cereal::make_nvp("TextureFile", TextureFile)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("Default_Texture_File", Default_Texture_File),
				cereal::make_nvp("TextureFile", TextureFile)
		);
	}
};

#endif // ! TEXTURE_MANEGER_H