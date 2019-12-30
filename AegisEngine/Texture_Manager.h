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
	static unordered_map<string, string> Default_Texture_File;		//! デフォルトのテクスチャのファイルパス
	static unordered_map<string, TEXTURE_FILE> TextureFile;			//! テクスチャのファイルデータ
	static unordered_map<string, TEXTURE_DATA> TextureData;			//! テクスチャデータ

	static void Default_Load(const bool flag);		// デフォルトのテクスチャの読み込み
	static void Load(const bool flag);				// テクスチャの読み込み

	static DWORD Get_File_Time(const string& path);

public:

	static void Init();
	static void Update();
	static void Uninit();

	static void Add(const string& file_name);
	static const bool Unload(const string& const file_name);

	static void Add_ReferenceCnt(const string& const file_name);
	static void Sub_ReferenceCnt(const string& const file_name);
	
	static XMINT2* const Get_WH(const string& const file_name);

	static ID3D11ShaderResourceView* const GetShaderResourceView(const string& const file_name);

	static unordered_map<string, TEXTURE_FILE>& Get_TextureFile();

	static unordered_map<string, TEXTURE_DATA>::iterator Get_TextureData_Start();
	static unordered_map<string, TEXTURE_DATA>::iterator Get_TextureData_End();

	template<class T>
	void serialize(T& archive) {
		archive(Default_Texture_File);
		archive(TextureFile);
	}
};

#endif // ! TEXTURE_MANEGER_H