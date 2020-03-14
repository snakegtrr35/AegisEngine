#pragma once

#ifndef TEXYUTE_H
#define TEXYUTE_H

//========================================
// �e�N�X�`���N���X
//========================================
class TEXTURE {
private:

	string FileName;
	size_t File;

public:
	TEXTURE();
	TEXTURE(const string& file_name);
	~TEXTURE() {};

	void Set_Texture();											// �e�N�X�`����ݒ�(Directx11)
	void Set_Texture_Name(const string& file_name);				// �e�N�X�`�����̐ݒ�
	const string& Get_Texture_Name();							// �e�N�X�`�����̎擾

	XMINT2* const Get_WH();

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(FileName);
		ar(File);
	}
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