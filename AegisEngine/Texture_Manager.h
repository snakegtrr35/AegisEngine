#pragma once

#ifndef TEXTURE_MANEGER_H
#define TEXTURE_MANEGER_H

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

	unordered_map<string, string> Default_Texture_File;		//! デフォルトのテクスチャのファイルパス
	unordered_map<string, TEXTURE_FILE> TextureFile;		//! テクスチャのファイルデータ
	unordered_map<string, TEXTURE_DATA> TextureData;		//! テクスチャデータ

	std::mutex LoadMutex;

	static void Load2(TEXTURE_MANEGER* manager, const vector<string>* files);

	void Default_Load(const bool flag);						// デフォルトのテクスチャの読み込み
	void Load(const bool flag);								// テクスチャの読み込み

	DWORD Get_File_Time(const string& path);

public:
	TEXTURE_MANEGER() {};
	~TEXTURE_MANEGER() { Uninit(); }

	static TEXTURE_MANEGER* Get_Instance();
	static void Init();

	void Update();

	void Uninit();

	void Add(const string& file_name);
	const bool Unload(const string& const file_name);

	void Add_ReferenceCnt(const string& const file_name);
	void Sub_ReferenceCnt(const string& const file_name);
	
	XMINT2* const Get_WH(const string& const file_name);

	ID3D11ShaderResourceView* const GetShaderResourceView(const string& const file_name);

	unordered_map<string, TEXTURE_FILE>& Get_TextureFile();

	unordered_map<string, TEXTURE_DATA>::iterator Get_TextureData_Start();
	unordered_map<string, TEXTURE_DATA>::iterator Get_TextureData_End();

	template<class T>
	void serialize(T& archive) {
		archive(Default_Texture_File);
		archive(TextureFile);
	}
};

#endif // ! TEXTURE_MANEGER_H