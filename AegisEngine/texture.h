#pragma once

#ifndef TEXYUTE_H
#define TEXYUTE_H

#include	"Renderer.h"

constexpr const int TEXTURE_SIZE_MAX = 8192;

typedef struct {
	string Name;
	XMINT2 WH;

}TEXTURE_FILE;


struct TEXTURE_DATA {
	unique_ptr<ID3D11ShaderResourceView, Release> Resource;
	XMINT2 WH;

	TEXTURE_DATA() : WH(XMINT2(0, 0)) {};

	TEXTURE_DATA(XMINT2 wh) : WH(wh) {};

	TEXTURE_DATA(int w, int h) : WH(XMINT2(w, h)) {};

	template<class T>
	void serialize(T& archive) {
		archive(WH);
		archive(Resource);
	}
};

//========================================
// �e�N�X�`���}�l�[�W���[�N���X
//========================================
class TEXTURE_MANEGER {
private:
	static map<string, unique_ptr<ID3D11ShaderResourceView, Release> > TextureResource;

	//! �e�N�X�`�����ꗗ
	static unordered_set<string> TexturePath;					// �e�N�X�`���̃p�X
	static unordered_map<size_t, TEXTURE_DATA> TextureFiles;	// �e�N�X�`���f�[�^

	static void Load();		// �e�N�X�`���̓ǂݍ���

public:

	static void Init();
	static void Uninit();

	static void Add(const string& const file_name, const float width, const float height);
	static void Unload(const string& const file_name);

	static XMINT2* const Get_WH(const string& const file_name);

	static ID3D11ShaderResourceView* const GetShaderResourceView(const string& const file_name);

	static const unordered_set<string>& Get_TexturePath();

	template<class T>
	void serialize(T& archive) {
		archive(TexturePath);
		archive(TextureFiles);
	}
};

//========================================
// �e�N�X�`���N���X
//========================================
class TEXTURE {
private:

	string FileName;

public:
	TEXTURE();
	TEXTURE(const string& file_name);
	~TEXTURE() {};

	void Set_Texture();											// �e�N�X�`����ݒ�(Directx11)
	void Set_Texture_Name(const string& file_name);				// �e�N�X�`�����̐ݒ�
	const string& Get_Texture_Name();							// �e�N�X�`�����̎擾

	XMINT2* const Get_WH();
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