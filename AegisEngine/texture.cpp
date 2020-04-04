#include	"texture.h"
#include	"Texture_Manager.h"
#include	"Renderer.h"


TEXTURE::TEXTURE()
{
	FileName = "none";
	File = hash<string>()(FileName);//
}

TEXTURE::TEXTURE(const string& file_name)
{
	FileName = file_name;
	File = hash<string>()(file_name);//
	TEXTURE_MANEGER::Get_Instance()->Add_ReferenceCnt(File);
}

//========================================
// �e�N�X�`����ݒ�(Directx11)
//========================================
void TEXTURE::Set_Texture(void)
{
	ID3D11ShaderResourceView* shader_resouce_view = TEXTURE_MANEGER::Get_Instance()->GetShaderResourceView(File);

	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &shader_resouce_view);
}

//========================================
// �e�N�X�`�����̐ݒ�
//========================================
void TEXTURE::Set_Texture_Name(const string& file_name)
{
	if (file_name != FileName)
	{
		TEXTURE_MANEGER::Get_Instance()->Sub_ReferenceCnt(File);
		FileName = file_name;
		File = hash<string>()(file_name);
		TEXTURE_MANEGER::Get_Instance()->Add_ReferenceCnt(File);
	}
}

//========================================
// �e�N�X�`�����̎擾
//========================================
const string& TEXTURE::Get_Texture_Name(void)
{
	return FileName;
}

XMINT2* const TEXTURE::Get_WH()
{
	return TEXTURE_MANEGER::Get_Instance()->Get_WH(File);
}


map<wstring,unique_ptr<ID3D11ShaderResourceView, Release>> FONT::FontResource;
ID3D11SamplerState* FONT::SamplerState = nullptr;


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
	// �t�H���g�f�[�^
	wstring Font;
	{
		string Font01("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 \+-*=/^.,;'\"!?()[]{}");

		string Font02("�������������������������������������ĂƂȂɂʂ˂̂͂Ђӂւق܂݂ނ߂�������������񂪂����������������������Âłǂ΂тԂׂڂς҂Ղ؂ۂ����");

		string Font03("�A�C�E�G�I�J�L�N�P�R�T�V�X�Z�\�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}�~�����������������������������K�M�O�Q�S�U�W�Y�[�]�_�a�d�f�h�o�r�u�x�{�p�s�v�y�|�������b");

		Font01 = Font01 + Font02;

		Font01 = Font01 + Font03;

		Font = stringTowstring(Font01);
	}

	// �t�H���g�n���h���̐���
	int fontSize = 128;
	int fontWeight = 1000;
	LOGFONTW lf =
	{
		fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
		//(WCHAR)"�l�r �o����"
		//(WCHAR)"�l�r ����"
		//(WCHAR)"�l�r �S�V�b�N"
		(WCHAR)"���C���I"
	};
	HFONT hFont = CreateFontIndirectW(&lf);

	// ���݂̃E�B���h�E�ɓK�p
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	const int gradFlag = GGO_GRAY4_BITMAP;
	// �K���̍ő�l
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

	//�t�H���g���������ރe�N�X�`���쐬
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

	// �V�F�[�_�p�ɃT���v�����쐬����
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

	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = fontTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

	ID3D11ShaderResourceView* ShaderResourceView;

	// �f�o�C�X
	auto device = CRenderer::GetDevice();

	// �f�o�C�X�R���e�L�X�g
	auto deviceContext = CRenderer::GetDeviceContext();

	for (auto font : Font)
	{
		// �t�H���g�r�b�g�}�b�v�擾

		UINT code = (UINT)font;

		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		GLYPHMETRICS gm;
		CONST MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };

		DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, nullptr, &mat);
		BYTE* pMono = new BYTE[size];

		GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

		//================================================================================

		//�t�H���g�̕��ƍ���
		int fontWidth = gm.gmCellIncX;
		int fontHeight = tm.tmHeight;

		//�t�H���g���������ރe�N�X�`���쐬
		fontTextureDesc.Width = fontWidth;
		fontTextureDesc.Height = fontHeight;

		ID3D11Texture2D* font_texture;

		HRESULT hr = device->CreateTexture2D(&fontTextureDesc, nullptr, &font_texture);

		// �t�H���g�����e�N�X�`���ɏ������ޕ���
		D3D11_MAPPED_SUBRESOURCE hMappedResource;
		hr = deviceContext->Map(
			font_texture,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&hMappedResource);
		// �����ŏ�������
		BYTE* pBits = (BYTE*)hMappedResource.pData;
		// �t�H���g���̏�������
		// iOfs_x, iOfs_y : �����o���ʒu(����)
		// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
		// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
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
		//�s�v�Ȃ̂ō폜
		SAFE_DELETE_ARRAY(pMono);

		// �V�F�[�_�[���\�[�X�̍쐬
		device->CreateShaderResourceView(font_texture, &srvDesc, &ShaderResourceView);

		wstring f;
		f.push_back(font);

		FontResource[f].reset(ShaderResourceView);

		f.clear();
	}
}

void FONT::Load_Font(const wstring& one_character)
{
	// �t�H���g�n���h���̐���
	int fontSize = 128;
	int fontWeight = 1000;
	LOGFONTW lf =
	{
		fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
		//(WCHAR)"�l�r �o����"
		//(WCHAR)"�l�r ����"
		//(WCHAR)"�l�r �S�V�b�N"
		(WCHAR)"���C���I"
	};
	HFONT hFont = CreateFontIndirectW(&lf);

	// ���݂̃E�B���h�E�ɓK�p
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(NULL);
	//HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// �t�H���g�r�b�g�}�b�v�擾
	wchar_t font = one_character.front();

	UINT code = (UINT)font;

	const int gradFlag = GGO_GRAY4_BITMAP;
	// �K���̍ő�l
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

	// �f�o�C�X
	auto device = CRenderer::GetDevice();

	// �f�o�C�X�R���e�L�X�g
	auto deviceContext = CRenderer::GetDeviceContext();

	//================================================================================

	//�t�H���g�̕��ƍ���
	int fontWidth = gm.gmCellIncX;
	int fontHeight = tm.tmHeight;

	//�t�H���g���������ރe�N�X�`���쐬
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

	HRESULT hr = device->CreateTexture2D(&fontTextureDesc, nullptr, &font_texture);

	// �t�H���g�����e�N�X�`���ɏ������ޕ���
	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = deviceContext->Map(
		font_texture,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource);
	// �����ŏ�������
	BYTE* pBits = (BYTE*)hMappedResource.pData;
	// �t�H���g���̏�������
	// iOfs_x, iOfs_y : �����o���ʒu(����)
	// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
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
	//�s�v�Ȃ̂ō폜
	SAFE_DELETE_ARRAY(pMono);

	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = fontTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

	ID3D11ShaderResourceView* ShaderResourceView;

	device->CreateShaderResourceView(font_texture, &srvDesc, &ShaderResourceView);

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
	if (FontResource.end() != FontResource.find(one_character))
	{
		return FontResource[one_character].get();
	}
	else
	{
		return nullptr;
	}
}

ID3D11SamplerState* FONT::Get_SamplerState()
{
	return SamplerState;
}