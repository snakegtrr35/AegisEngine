#pragma once

#ifndef TEXYUTE_H
#define TEXYUTE_H

#include	"Renderer.h"

//========================================
// テクスチャクラス
//========================================
class TEXTURE {
private:

	string FileName;

public:
	TEXTURE();
	TEXTURE(const string& file_name);
	~TEXTURE() {};

	void Set_Texture();											// テクスチャを設定(Directx11)
	void Set_Texture_Name(const string& file_name);				// テクスチャ名の設定
	const string& Get_Texture_Name();							// テクスチャ名の取得

	XMINT2* const Get_WH();
};

//========================================
// テクスチャマネージャークラス
//========================================
class TEXTURE_MANEGER {
private:
	static map<string, unique_ptr<ID3D11ShaderResourceView, Release> > TextureResource;

	static void Load();		// テクスチャの読み込み

public:

	static void Init();
	static void Uninit();

	static void Add(const string& const file_name);
	static void Unload(const string& const file_name = "");			// テクスチャの解放

	static XMINT2* const Get_WH(const string& const file_name);

	static ID3D11ShaderResourceView* const GetShaderResourceView(const string& const file_name);
};


class FONT {
private:
	static map<wstring, unique_ptr<ID3D11ShaderResourceView, Release>> FontResource;
	static ID3D11SamplerState* SamplerState;

	static void Load_Font();
	static void Load_Font(const wstring& one_character);

public:

	static void Init();

	static void Uninit();

	static void Add_Font(const wstring& one_character);

	static ID3D11ShaderResourceView* Get_Font_Resource(const wstring& one_character);

	static ID3D11SamplerState* Get_SamplerState();
};

#endif // !TEXYUTE_H