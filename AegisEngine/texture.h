#pragma once

#ifndef TEXYUTE_H
#define TEXYUTE_H

#include	"Renderer.h"
#include	"WICTextureLoader.h"

// 先生が作ったやつ
class CTexture
{
public:
	void Load( const char *FileName );
	void Unload();

	ID3D11ShaderResourceView* GetShaderResourceView(){ return m_ShaderResourceView; }


private:
	ID3D11Texture2D*			m_Texture;
	ID3D11ShaderResourceView*	m_ShaderResourceView;
};



//========================================
// テクスチャクラス
//========================================
class TEXTURE {
private:

	string FileName;

public:
	TEXTURE();
	TEXTURE(const string& const file_name);
	~TEXTURE() {
		//Uninit();
	};
	
	//void Uninit() {};

	void Set_Texture();												// テクスチャを設定(Directx11)
	void Set_Texture_Name(const string& const file_name);			// テクスチャ名の設定
	const string& Get_Texture_Name();								// テクスチャ名の取得

	XMINT2* const Get_WH();
};

//========================================
// テクスチャマネージャークラス
//========================================
class TEXTURE_MANEGER {
private:
	static map<string, ID3D11ShaderResourceView*> TextureResource;

	static void Load();														// テクスチャの読み込み

public:

	static void Init();
	static void Uninit();

	static void Add(const string& const file_name);
	static void Unload(const string& const file_name = "");				// テクスチャの解放

	static XMINT2* const Get_WH(const string& const file_name);

	static ID3D11ShaderResourceView* const GetShaderResourceView(const string& const file_name);
};


class FONT {
private:
	static map<string, ID3D11ShaderResourceView*> FontResource;
	static ID3D11SamplerState* SamplerState;
	static unsigned short FontCnt;

	static void Load_Font();
	static void Load_Font(const string one_character);

public:

	static void Init();

	static void Uninit() {
		for (auto font : FontResource)
		{
			font.second->Release();
		}
		FontResource.clear();
	};

	static void Add_Font(const string one_character);

	static ID3D11ShaderResourceView* Get_Font_Resource(const string one_character);

	static ID3D11SamplerState* Get_SamplerState();
};

#endif // !TEXYUTE_H