#include	"texture.h"
#include	"Texture_Manager.h"
#include	"Renderer.h"



//static TEXTURE_FILE g_TextureFiles[] = {
//	{"number.png", XMINT2(512, 512) },
//	{"number02.png", XMINT2(320, 32) },
//
//	{"title.png", XMINT2(1920, 1080) },
//	{"game_clear.png", XMINT2(1920, 1080) },
//	{"game_over.png", XMINT2(1920, 1080) },
//
//	{"field004.png", XMINT2(128, 128) },
//	{"shadow000.jpg", XMINT2(80, 80) },
//	{"Explosion.png", XMINT2(360, 360) },
//	{"arrow.png", XMINT2(665, 95) },
//	{"Reticule.png", XMINT2(512, 512) },
//
//	{"hp_rod.png", XMINT2(64, 256) },
//	{"hp.png", XMINT2(2, 2) },
//	{"bullet_icon.png", XMINT2(64, 128) },
//
//	{"pause.png", XMINT2(512, 512) },
//	{"select.png", XMINT2(60, 60) },
//
//	{"UVCheckerMap01-512.png", XMINT2(512, 512) },
//	{"UVCheckerMap01-1024.png", XMINT2(1024, 1024) },
//
//	{"asphalt01-pattern.jpg", XMINT2(1000, 1000) },
//
//	{"asult_rifl.png", XMINT2(512, 512) },
//	{"Bazooka.png", XMINT2(512, 512) },
//
//	{"go.png", XMINT2(256, 128) },
//
//	{"sky.png", XMINT2(8192, 4096) },
//};


TEXTURE::TEXTURE()
{
	FileName = "none";
}

TEXTURE::TEXTURE(const string& file_name)
{
	FileName = file_name;
	TEXTURE_MANEGER::Add_ReferenceCnt(FileName);
}

//========================================
// テクスチャを設定(Directx11)
//========================================
void TEXTURE::Set_Texture(void)
{
	ID3D11ShaderResourceView* shader_resouce_view = TEXTURE_MANEGER::GetShaderResourceView(FileName);

	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &shader_resouce_view);
}

//========================================
// テクスチャ名の設定
//========================================
void TEXTURE::Set_Texture_Name(const string& const file_name)
{
	TEXTURE_MANEGER::Sub_ReferenceCnt(FileName);
	FileName = file_name;
	TEXTURE_MANEGER::Add_ReferenceCnt(FileName);
}

//========================================
// テクスチャ名の取得
//========================================
const string& TEXTURE::Get_Texture_Name(void)
{
	return FileName;
}

XMINT2* const TEXTURE::Get_WH()
{
	return TEXTURE_MANEGER::Get_WH(FileName);
}


map<wstring,unique_ptr<ID3D11ShaderResourceView, Release>> FONT::FontResource;
ID3D11SamplerState* FONT::SamplerState = nullptr;

//wstring stringTowstring(string& font);

void FONT::Init()
{
	FONT::Load_Font();
}

void FONT::Uninit()
{
	for (auto tex = FontResource.begin(); tex != FontResource.end(); tex++)
	{
		tex->second.reset(nullptr);
	}
	FontResource.clear();
};

void FONT::Load_Font()
{
	// フォントデータ
	wstring Font;
	{
		string Font01("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 \+-*=/^.,;'\"!?()[]{}");

		string Font02("あいおうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをんがぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽゃゅょっ");

		string Font03("アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲンガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポャュョッ");

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
		//(WCHAR)"ＭＳ Ｐ明朝"
		//(WCHAR)"ＭＳ 明朝"
		//(WCHAR)"ＭＳ ゴシック"
		(WCHAR)"メイリオ"
	};
	HFONT hFont = CreateFontIndirectW(&lf);

	// 現在のウィンドウに適用
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	const int gradFlag = GGO_GRAY8_BITMAP;
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
	D3D11_TEXTURE2D_DESC fontTextureDesc;
	ZeroMemory(&fontTextureDesc, sizeof(fontTextureDesc));
	fontTextureDesc.MipLevels = 1;
	fontTextureDesc.ArraySize = 1;
	fontTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	fontTextureDesc.SampleDesc.Count = 1;
	fontTextureDesc.SampleDesc.Quality = 0;
	fontTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	fontTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	fontTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fontTextureDesc.MiscFlags = 0;

	// シェーダ用にサンプラを作成する
	{
		D3D11_SAMPLER_DESC samDesc;
		ZeroMemory(&samDesc, sizeof(samDesc));
		samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.MaxAnisotropy = 1;
		samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samDesc.MaxLOD = D3D11_FLOAT32_MAX;

		ID3D11SamplerState* samplerState = nullptr;

		CRenderer::GetDevice()->CreateSamplerState(&samDesc, &samplerState);
	}

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = fontTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

	ID3D11ShaderResourceView* ShaderResourceView;

	for (auto font : Font)
	{
		//// フォントビットマップ取得

		UINT code = (UINT)font;

		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		GLYPHMETRICS gm;
		CONST MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };

		DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
		BYTE* pMono = new BYTE[size];

		GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

		//================================================================================

		//フォントの幅と高さ
		int fontWidth = gm.gmCellIncX;
		int fontHeight = tm.tmHeight;

		//フォントを書き込むテクスチャ作成
		fontTextureDesc.Width = fontWidth;
		fontTextureDesc.Height = fontHeight;

		ID3D11Texture2D* font_texture;

		HRESULT hr = CRenderer::GetDevice()->CreateTexture2D(&fontTextureDesc, NULL, &font_texture);

		//デバイスコンテキスト
		auto deviceContext = CRenderer::GetDeviceContext();

		// フォント情報をテクスチャに書き込む部分
		D3D11_MAPPED_SUBRESOURCE hMappedResource;
		hr = deviceContext->Map(
			font_texture,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&hMappedResource);
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
		deviceContext->Unmap(font_texture, 0);
		//不要なので削除
		delete[] pMono;

		// シェーダーリソースの作成
		CRenderer::GetDevice()->CreateShaderResourceView(font_texture, &srvDesc, &ShaderResourceView);

		wstring a;
		a.push_back(font);

		FontResource[a].reset(ShaderResourceView);

		a.clear();
	}
}

void FONT::Load_Font(const wstring& one_character)
{
	// フォントハンドルの生成
	int fontSize = 128;
	int fontWeight = 1000;
	LOGFONTW lf =
	{
		fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
		//(WCHAR)"ＭＳ Ｐ明朝"
		//(WCHAR)"ＭＳ 明朝"
		//(WCHAR)"ＭＳ ゴシック"
		(WCHAR)"メイリオ"
	};
	HFONT hFont = CreateFontIndirectW(&lf);

	// 現在のウィンドウに適用
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	//HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// フォントビットマップ取得
	wchar_t font = (wchar_t)one_character.c_str();

	UINT code = (UINT)font;

	const int gradFlag = GGO_GRAY8_BITMAP;
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

	DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
	BYTE* pMono = new BYTE[size];

	GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

	//================================================================================

	//フォントの幅と高さ
	int fontWidth = gm.gmCellIncX;
	int fontHeight = tm.tmHeight;

	//フォントを書き込むテクスチャ作成
	D3D11_TEXTURE2D_DESC fontTextureDesc;
	ZeroMemory(&fontTextureDesc, sizeof(fontTextureDesc));
	fontTextureDesc.Width = fontWidth;
	fontTextureDesc.Height = fontHeight;
	fontTextureDesc.MipLevels = 1;
	fontTextureDesc.ArraySize = 1;
	fontTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	fontTextureDesc.SampleDesc.Count = 1;
	fontTextureDesc.SampleDesc.Quality = 0;
	fontTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	fontTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	fontTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fontTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* font_texture;

	HRESULT hr = CRenderer::GetDevice()->CreateTexture2D(&fontTextureDesc, NULL, &font_texture);

	//デバイスコンテキスト
	auto deviceContext = CRenderer::GetDeviceContext();

	// フォント情報をテクスチャに書き込む部分
	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = deviceContext->Map(
		font_texture,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource);
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
	deviceContext->Unmap(font_texture, 0);
	//不要なので削除
	delete[] pMono;

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = fontTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

	ID3D11ShaderResourceView* ShaderResourceView;

	CRenderer::GetDevice()->CreateShaderResourceView(font_texture, &srvDesc, &ShaderResourceView);

	wstring a;
	a.push_back(font);

	FontResource[a].reset(ShaderResourceView);

	a.clear();
}

void FONT::Add_Font(const wstring& one_character)
{
	for (auto tex = FontResource.begin(); tex != FontResource.end(); tex++)
	{
		if (one_character == tex->first)
		{
			return;
		}
	}
}

ID3D11ShaderResourceView* FONT::Get_Font_Resource(const wstring& one_character)
{
	for (auto tex = FontResource.begin(); tex != FontResource.end(); tex++)
	{
		if (one_character == tex->first)
		{
			return FontResource[one_character].get();
		}
	}

	return nullptr;
}

ID3D11SamplerState* FONT::Get_SamplerState()
{
	return SamplerState;
}