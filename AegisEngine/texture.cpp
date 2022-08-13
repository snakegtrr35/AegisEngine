#include "texture.h"
#include "TextureManager.h"

using namespace aegis;

TEXTURE::TEXTURE() : FileName("none"), File(std::hash<aegis::string>()(FileName))
{
}

TEXTURE::TEXTURE(std::string_view file_name) : FileName(file_name), File(std::hash<std::string_view>()(file_name))
{
	TextureManager::Get_Instance()->Add_ReferenceCnt(File);
}

//========================================
// テクスチャを設定(Directx11)
//========================================
void TEXTURE::Set_Texture(void)
{
	ShaderResourceView* shaderResouceView = TextureManager::Get_Instance()->GetShaderResourceView(File);

	CRenderer::getInstance()->PSSetShaderResources(0, 1, &shaderResouceView);
}

//========================================
// テクスチャ名の設定
//========================================
void TEXTURE::Set_Texture_Name(const aegis::string& file_name)
{
	if (file_name != FileName)
	{
		TextureManager::Get_Instance()->Sub_ReferenceCnt(File);
		FileName = file_name;
		File = std::hash<aegis::string>()(file_name);
		TextureManager::Get_Instance()->Add_ReferenceCnt(File);
	}
}

//========================================
// テクスチャ名の取得
//========================================
const aegis::string& TEXTURE::Get_Texture_Name(void)
{
	return FileName;
}

aegis::Int2* const TEXTURE::Get_WH()
{
	return TextureManager::Get_Instance()->Get_WH(File);
}


aegis::unordered_map<aegis::wstring, aegis::uniquePtr<aegis::ShaderResourceView>> FONT::FontResource;
aegis::SamplerState* FONT::SamplerState = nullptr;


void FONT::Init()
{
	//FONT::Load_Font();
}

void FONT::Uninit()
{
	for (auto tex = FontResource.begin(); tex != FontResource.end(); tex++)
	{
		tex->second->Release();
	}
	FontResource.clear();
};

void FONT::Load_Font()
{
	CRenderer* render = CRenderer::getInstance();

	// フォントデータ
	aegis::wstring Font;
	{
		aegis::string Font01("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\ -+*=/^.,;'\"!?()[]{}");

		aegis::string Font02("あいおうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをんがぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽゃゅょっ");

		aegis::string Font03("アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲンガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポャュョッ");

		Font01 = Font01 + Font02;

		Font01 = Font01 + Font03;

		Font = stringTowstring(Font01);
	}

	// フォントハンドルの生成
	int fontSize = 128;
	int fontWeight = 1000;
	LOGFONTW lf =
	{
		fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
		//(CHAR)"ＭＳ Ｐ明朝"
		//(CHAR)"ＭＳ 明朝"
		//(CHAR)"ＭＳ ゴシック"
		(CHAR)"メイリオ"
	};
	HFONT hFont = CreateFontIndirectW(&lf);

	// 現在のウィンドウに適用
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	const int gradFlag = GGO_GRAY4_BITMAP;
	// 階調の最大値
	int grad = 0;
	switch (gradFlag)
	{
	case GGO_GRAY2_BITMAP:
		grad = 4;
		break;
	case GGO_GRAY4_BITMAP:
		grad = 16;
		break;
	case GGO_GRAY8_BITMAP:
		grad = 64;
		break;
	}

	//フォントを書き込むテクスチャ作成
	Texture2DDesc fontTextureDesc{};
	fontTextureDesc.MipLevels = 1;
	fontTextureDesc.ArraySize = 1;
	fontTextureDesc.Format = Format::RGBA8Unorm;
	fontTextureDesc.SampleDesc.Count = 1;
	fontTextureDesc.SampleDesc.Quality = 0;
	fontTextureDesc.Usage = Usage::Dynamic;
	fontTextureDesc.BindFlags = BindFlag::ShaderResource;
	fontTextureDesc.CPUAccessFlags = CpuAccessFlag::Write;
	fontTextureDesc.MiscFlags = 0;

	// シェーダ用にサンプラを作成する
	{
		SamplerDesc samDesc{};
		samDesc.Filter = Filter::Linear;
		samDesc.Address = AddressMode::Wrap;
		samDesc.MaxAnisotropy = 1;
		samDesc.ComparisonFunc = ComparisonFunc::Always;
		samDesc.MaxLOD = Math::Float32Max;

		SamplerState = render->CreateSampler(samDesc);
	}

	UINT code;
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	GLYPHMETRICS gm;
	CONST MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size;
	BYTE* pMono = nullptr;

	std::unique_ptr<Texture2D> font_texture = nullptr;
	MappedSubresource hMappedResource;

	aegis::wstring f;

	for (auto font : Font)
	{
		// フォントビットマップ取得

		code = (UINT)font;

		size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, nullptr, &mat);
		pMono = new BYTE[size];

		GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

		//================================================================================

		//フォントを書き込むテクスチャ作成
		//フォントの幅と高さ
		fontTextureDesc.Width = gm.gmCellIncX;
		fontTextureDesc.Height = tm.tmHeight;

		font_texture.reset(render->CreateTexture2D(fontTextureDesc, nullptr));

		// フォント情報をテクスチャに書き込む部分
		hMappedResource = render->Map(font_texture.get());

		// ここで書き込む
		BYTE* pBits = (BYTE*)hMappedResource.pData;
		// フォント情報の書き込み
		// iOfs_x, iOfs_y : 書き出し位置(左上)
		// iBmp_w, iBmp_h : フォントビットマップの幅高
		// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
		int iOfs_x = gm.gmptGlyphOrigin.x;
		int iOfs_y = tm.tmAscent - gm.gmptGlyphOrigin.y;
		int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
		int iBmp_h = gm.gmBlackBoxY;
		int Level = 17;
		int x, y;
		DWORD Alpha, Color;
		memset(pBits, 0, hMappedResource.RowPitch * tm.tmHeight);
		for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
		{
			for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
			{
				Alpha =
					(255 * pMono[x - iOfs_x + iBmp_w * (y - iOfs_y)])
					/ (Level - 1);
				Color = 0x00ffffff | (Alpha << 24);
				memcpy(
					(BYTE*)pBits
					+ hMappedResource.RowPitch * y + 4 * x,
					&Color,
					sizeof(DWORD));
			}
		}
		render->Unmap(font_texture.get());
		//不要なので削除
		SAFE_DELETE_ARRAY(pMono);

		// ShaderResourceViewの情報を作成する
		ShaderResourceViewDesc srvDesc{};
		srvDesc.Format = fontTextureDesc.Format;
		srvDesc.ViewDimension = SrvDimension::Texture2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

		ShaderResourceView* shaderResourceView;

		// シェーダーリソースの作成
		shaderResourceView = render->CreateShaderResourceView(font_texture.get(), srvDesc);

		f.push_back(font);

		FontResource[f].reset(shaderResourceView);

		f.clear();
	}
}

void FONT::Load_Font(const aegis::wstring& one_character)
{
	CRenderer* render = CRenderer::getInstance();

	// フォントハンドルの生成
	int fontSize = 128;
	int fontWeight = 1000;
	LOGFONTW lf =
	{
		fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
		//(CHAR)"ＭＳ Ｐ明朝"
		//(CHAR)"ＭＳ 明朝"
		//(CHAR)"ＭＳ ゴシック"
		(CHAR)"メイリオ"
	};
	HFONT hFont = CreateFontIndirectW(&lf);

	// 現在のウィンドウに適用
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// フォントビットマップ取得
	wchar_t font = one_character.front();

	UINT code = (UINT)font;

	const int gradFlag = GGO_GRAY4_BITMAP;
	// 階調の最大値
	int grad = 0;
	switch (gradFlag)
	{
	case GGO_GRAY2_BITMAP:
		grad = 4;
		break;
	case GGO_GRAY4_BITMAP:
		grad = 16;
		break;
	case GGO_GRAY8_BITMAP:
		grad = 64;
		break;
	}

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	GLYPHMETRICS gm;
	CONST MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };

	DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, nullptr, &mat);
	BYTE* pMono = new BYTE[size];

	GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

	//================================================================================

	//フォントの幅と高さ
	int fontWidth = gm.gmCellIncX;
	int fontHeight = tm.tmHeight;

	//フォントを書き込むテクスチャ作成
	Texture2DDesc fontTextureDesc{};
	fontTextureDesc.Width = fontWidth;
	fontTextureDesc.Height = fontHeight;
	fontTextureDesc.MipLevels = 1;
	fontTextureDesc.ArraySize = 1;
	fontTextureDesc.Format = Format::RGBA8Unorm;
	fontTextureDesc.SampleDesc.Count = 1;
	fontTextureDesc.SampleDesc.Quality = 0;
	fontTextureDesc.Usage = Usage::Dynamic;
	fontTextureDesc.BindFlags = BindFlag::ShaderResource;
	fontTextureDesc.CPUAccessFlags = CpuAccessFlag::Write;
	fontTextureDesc.MiscFlags = 0;

	std::unique_ptr<Texture2D> font_texture = nullptr;

	font_texture.reset(render->CreateTexture2D(fontTextureDesc, nullptr));

	// フォント情報をテクスチャに書き込む部分
	MappedSubresource hMappedResource = render->Map(font_texture.get());

	// ここで書き込む
	BYTE* pBits = (BYTE*)hMappedResource.pData;
	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
	int iOfs_x = gm.gmptGlyphOrigin.x;
	int iOfs_y = tm.tmAscent - gm.gmptGlyphOrigin.y;
	int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
	int iBmp_h = gm.gmBlackBoxY;
	int Level = 17;
	int x, y;
	DWORD Alpha, Color;
	memset(pBits, 0, hMappedResource.RowPitch * tm.tmHeight);
	for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
	{
		for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
		{
			Alpha =
				(255 * pMono[x - iOfs_x + iBmp_w * (y - iOfs_y)])
				/ (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy(
				(BYTE*)pBits
				+ hMappedResource.RowPitch * y + 4 * x,
				&Color,
				sizeof(DWORD));
		}
	}
	render->Unmap(font_texture.get());
	//不要なので削除
	SAFE_DELETE_ARRAY(pMono);

	// ShaderResourceViewの情報を作成する
	ShaderResourceViewDesc srvDesc{};
	srvDesc.Format = fontTextureDesc.Format;
	srvDesc.ViewDimension = SrvDimension::Texture2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

	ShaderResourceView* shaderResourceView;

	shaderResourceView = render->CreateShaderResourceView(font_texture.get(), srvDesc);

	aegis::wstring f;
	f.push_back(font);

	FontResource[f].reset(shaderResourceView);
}

void FONT::Add_Font(const aegis::wstring& one_character)
{
	for (auto tex = FontResource.begin(); tex != FontResource.end(); tex++)
	{
		if (one_character == tex->first)
		{
			return;
		}
	}
}

ShaderResourceView* FONT::Get_Font_Resource(const aegis::wstring& one_character)
{
	if (FontResource.end() != FontResource.find(one_character))
	{
		return FontResource[one_character].get();
	}
	else
	{
		return nullptr;
	}
}

SamplerState* FONT::Get_SamplerState()
{
	return SamplerState;
}