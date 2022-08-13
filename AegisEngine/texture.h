#pragma once

#ifndef TEXYUTE_H
#define TEXYUTE_H

#include "RendererTypeDefine.h"

//========================================
// テクスチャクラス
//========================================
class TEXTURE {
private:

	aegis::string FileName;
	aegis::uint64 File;

public:
	TEXTURE();
	TEXTURE(std::string_view file_name);
	~TEXTURE() {};

	void Set_Texture();											// テクスチャを設定(Directx11)
	void Set_Texture_Name(const aegis::string& file_name);				// テクスチャ名の設定
	const aegis::string& Get_Texture_Name();							// テクスチャ名の取得

	aegis::Int2* const Get_WH();

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("FileName", std::string(FileName)),
				cereal::make_nvp("File", File)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		std::string s;
		archive(cereal::make_nvp("FileName", s));
		FileName.reserve(s.size());
		FileName = s;
		
		archive(cereal::make_nvp("File", File));
	}
};


class FONT {
private:
	static aegis::unordered_map<aegis::wstring, aegis::uniquePtr<aegis::ShaderResourceView>> FontResource;
	static aegis::SamplerState* SamplerState;

	static void Load_Font();
	static void Load_Font(const aegis::wstring& one_character);

public:

	static void Init();

	static void Uninit();

	static void Add_Font(const aegis::wstring& one_character);

	static aegis::ShaderResourceView* Get_Font_Resource(const aegis::wstring& one_character);

	static aegis::SamplerState* Get_SamplerState();


};

#endif // !TEXYUTE_H