#include	"Renderer.h"
#include	"texture.h"

#include	"Library/DirectXTex/WICTextureLoader.h"
#include	"Library/DirectXTex/DDSTextureLoader.h"


static TEXTURE_FILE g_TextureFiles[] = {
	{"number.png", XMINT2(512, 512) },
	{"number02.png", XMINT2(320, 32) },

	{"title.png", XMINT2(1920, 1080) },
	{"game_clear.png", XMINT2(1920, 1080) },
	{"game_over.png", XMINT2(1920, 1080) },

	{"field004.png", XMINT2(128, 128) },
	{"shadow000.jpg", XMINT2(80, 80) },
	{"Explosion.png", XMINT2(360, 360) },
	{"arrow.png", XMINT2(665, 95) },
	{"Reticule.png", XMINT2(512, 512) },

	{"hp_rod.png", XMINT2(64, 256) },
	{"hp.png", XMINT2(2, 2) },
	{"bullet_icon.png", XMINT2(64, 128) },

	{"pause.png", XMINT2(512, 512) },
	{"select.png", XMINT2(60, 60) },

	{"UVCheckerMap01-512.png", XMINT2(512, 512) },
	{"UVCheckerMap01-1024.png", XMINT2(1024, 1024) },

	{"asphalt01-pattern.jpg", XMINT2(1000, 1000) },

	{"asult_rifl.png", XMINT2(512, 512) },
	{"Bazooka.png", XMINT2(512, 512) },

	{"go.png", XMINT2(256, 128) },

	{"sky.png", XMINT2(8192, 4096) },
};

map<string, unique_ptr<ID3D11ShaderResourceView, Release> > TEXTURE_MANEGER::TextureResource;
//unordered_map<size_t, TEXTURE_DATA> TEXTURE_MANEGER::TextureFiles;//

unordered_set<string>					TEXTURE_MANEGER::TexturePath;//
unordered_map<size_t, TEXTURE_DATA>		TEXTURE_MANEGER::TextureFiles;//

// 読み込みテクスチャ数
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);


//================================================================================================================================================================

void TEXTURE_MANEGER::Init()
{
	bool flag;

	{
		std::ofstream file("texture.dat", std::ios::binary);

		flag = file.is_open();

		if (flag)
		{
			cereal::BinaryOutputArchive archive(file);
			archive(TexturePath);
			archive(TextureFiles);
		}
	}
	// デフォルトの画像データの読み込み
	Load();

	//hash<string> hasher;

	//for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	//{
	//	size_t hash = hasher(g_TextureFiles[i].Name);

	//	TextureFiles[hash].Resource.reset(TextureResource[g_TextureFiles[i].Name].get());
	//	TextureFiles[hash].WH.x = g_TextureFiles[i].WH.x;
	//	TextureFiles[hash].WH.y = g_TextureFiles[i].WH.y;
	//}
}

void TEXTURE_MANEGER::Uninit()
{
	{
		TEXTURE_MANEGER t;

		std::ifstream file("texture.dat", std::ios::binary);

		if (file.is_open())
		{
			cereal::BinaryInputArchive archive(file);
			archive(t);
		}
	}

	TexturePath.clear();

	for (auto tex = TextureResource.begin(); tex != TextureResource.end(); tex++)
	{
		tex->second.reset(nullptr);
	}
	TextureResource.clear();
}

void TEXTURE_MANEGER::Load()
{
	UINT width, height;

	string path;			// ファイル名(パス付き) 
	string file_name;		// ファイル名(パスなし)

	size_t pos;
	size_t h;				// ハッシュ値

	ID3D11ShaderResourceView* ShaderResourceView;
	wstring name;
	wstring type;
	hash<string> hasher;

	std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/Default/texture");
	for (auto f : e) {
		
		// 一つ一つのファイル名(パス付き)
		path = f.path().string();

		// 置換
		replace(path.begin(), path.end(), '\\', '/');

		pos = path.find_last_of("/");

		file_name = path.substr(pos + 1);

		//
		time_t rtime;
		struct tm* strtim;
		strtim = localtime(&rtime);

		filesystem::file_time_type file_time = filesystem::last_write_time(path);

		auto sec = chrono::duration_cast<chrono::seconds>(file_time.time_since_epoch());

		std::time_t t = sec.count();

		const tm* lt = std::localtime(&t);

		string s(asctime(lt));

		/*void print_datetime(fs::file_time_type tp)
		{
			auto sec = chrono::duration_cast<chrono::seconds>(tp.time_since_epoch());

			std::time_t t = sec.count();
			const tm* lt = std::localtime(&t);
			std::cout << std::put_time(lt, "%c") << std::endl;
		}*/

		h = hasher(file_name);
		// 既に読み込んだテクスチャのスキップ
		if (TextureFiles.find(h) != TextureFiles.end())
			continue;

		// テクスチャ名の登録
		TexturePath.insert(path);

		// テクスチャの読み込み
		{
			file_name = "asset/Default/texture/" + file_name;

			HRESULT hr;
			
			// char から wchar_t への変換
			name = stringTowstring(file_name);
			
			if (L"dds" == type)	// dds
			{
				hr = CreateDDSTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}
			else	// jpg か png
			{
				hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), name.c_str(), nullptr, &ShaderResourceView, &width, &height);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return;
				}
			}

			TextureFiles[h].Resource.reset(ShaderResourceView);
			TextureFiles[h].WH.x = width;
			TextureFiles[h].WH.y = height;

			// ファイル・ディレクトリの最終更新日時を取得
			//filesystem::file_time_type file_time = filesystem::last_write_time("regular.txt");

			//filesystem::file_time_type file_time = filesystem::last_write_time(path);
		}
	}
}

void TEXTURE_MANEGER::Add(const string& const file_name, const float width, const float height)
{
	ID3D11ShaderResourceView* ShaderResourceView;

	// char から wchar_t への変換
	wstring file = stringTowstring(file_name);

	// 置換
	replace(file.begin(), file.end(), '\\', '/');

	HRESULT hr;

	hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), file.c_str(), nullptr, &ShaderResourceView, nullptr, nullptr);
	if (FAILED(hr))
	{
		return;
	}

	hash<string> hasher;

	size_t hash = hasher(file_name);

	TextureFiles[hash].Resource.reset(ShaderResourceView);
	TextureFiles[hash].WH.x = width;
	TextureFiles[hash].WH.y = height;

	//TextureResource[file_name].reset(ShaderResourceView);
}

// テクスチャの解放
void TEXTURE_MANEGER::Unload(const string& const file_name)
{
}

XMINT2* const TEXTURE_MANEGER::Get_WH(const string& file_name)
{
	//for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	//{
	//	if (file_name == g_TextureFiles[i].Name)
	//	{
	//		return &g_TextureFiles[i].WH;
	//	}
	//}

	hash<string> hasher;

	size_t hash = hasher(file_name);

	if (TextureFiles.find(hash) != TextureFiles.end())
	{
		return &TextureFiles[hash].WH;
	}

	return nullptr;
}

ID3D11ShaderResourceView* const TEXTURE_MANEGER::GetShaderResourceView(const string& const file_name)
{
	//for (auto tex = TextureResource.begin(); tex != TextureResource.end(); tex++)
	//{
	//	if (file_name == tex->first)
	//	{
	//		return tex->second.get();
	//	}
	//}

	hash<string> hasher;

	size_t hash = hasher(file_name);

	if (TextureFiles.find(hash) != TextureFiles.end())
	{
		return TextureFiles[hash].Resource.get();
	}

	return nullptr;
}

const unordered_set<string>& TEXTURE_MANEGER::Get_TexturePath()
{
	return TexturePath;
}


TEXTURE::TEXTURE()
{
	FileName = "none";
}

TEXTURE::TEXTURE(const string& file_name)
{
	FileName = file_name;
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
	FileName = file_name;
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