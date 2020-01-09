#pragma once

#ifndef RENDER_H
#define RENDER_H

#include	"main.h"

//! ���_�\����
struct VERTEX_3D
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT4 Diffuse;
    XMFLOAT2 TexCoord;
};

//! ���_�\����
struct VERTEX_ANIME_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	WORD BoneIndex[4];
	float BoneWeight[4];

	VERTEX_ANIME_3D() : Normal(XMFLOAT3(0.f, 0.f, 0.f)), Position(XMFLOAT3(0.f, 0.f, 0.f)), Diffuse(XMFLOAT4(0.f, 0.f, 0.f, 0.f)), TexCoord(XMFLOAT2(0.f, 0.f)) {
		for (int i = 0; i < 4; i++)
		{
			BoneIndex[i] = 0;
			BoneWeight[i] = 0.f;
		}
	}
};


//! �F�\����
struct COLOR
{
	float r;
	float g;
	float b;
	float a;

	COLOR() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	COLOR( float _r, float _g, float _b, float _a )
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	// ������Z( = )
	COLOR& operator = (const COLOR& color)
	{
		r = color.r; g = color.g; b = color.b, a = color.a;
		return *this;
	}

	// �a( + )
	const COLOR operator + (const COLOR& color) const
	{
		return COLOR(r + color.r, g + color.g, b + color.b, a + color.a);
	}

	// ��( - )
	const COLOR operator - (const COLOR& color) const
	{
		return COLOR(r - color.r, g - color.g, b - color.b, a - color.a);
	}

	// ��( * )
	const COLOR operator * (const COLOR& color) const
	{
		return COLOR(r * color.r, g * color.g, b * color.b, a * color.a);
	}

	// ��( / )
	const COLOR operator / (const COLOR& color) const
	{
		return COLOR(r / color.r, g / color.g, b / color.b, a / color.a);
	}

	// ���g�ɑ΂���a( += ) 
	COLOR& operator += (const COLOR& color)
	{
		r += color.r; g += color.g;	b += color.b; a += color.a;
		return *this;
	}

	// ���g�ɑ΂��鍷( -= ) 
	COLOR& operator -= (const COLOR& color)
	{
		r -= color.r; g -= color.g;	b -= color.b; a -= color.a;
		return *this;
	}

	// ���g�ɑ΂����( *= ) 
	COLOR& operator *= (const COLOR& color)
	{
		r *= color.r; g *= color.g;	b *= color.b; a *= color.a;
		return *this;
	}

	// ���g�ɑ΂��鏤( /= ) 
	COLOR& operator /= (const COLOR& color)
	{
		r /= color.r; g /= color.g;	b /= color.b; a /= color.a;
		return *this;
	}

	bool operator !=(const COLOR& color)
	{
		if (r != color.r) return true;
		if (g != color.g) return true;
		if (b != color.b) return true;
		if (a != color.a) return true;
		return false;
	}

	bool operator ==(const COLOR& color)
	{
		if (r != color.r) return false;
		if (g != color.g) return false;
		if (b != color.b) return false;
		if (a != color.a) return false;
		return true;
	}
};

template<typename Archive>
void serialize(Archive& ar, COLOR& color)
{
	ar(color.r, color.g, color.b, color.a);
}

//! �}�e���A���\����
struct MATERIAL
{
	COLOR		Ambient;
	COLOR		Diffuse;
	COLOR		Specular;
	COLOR		Emission;
	float		Shininess;
	float		Dummy[3];//16bit���E�p
};




//! �}�e���A���\����
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	//class CTexture*	Texture;
};


//! �`��T�u�Z�b�g�\����
struct DX11_SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	XMFLOAT4	Direction;
	XMFLOAT4	Position;
	COLOR		Diffuse;
	COLOR		Ambient;
	COLOR		Specular;
};

typedef struct {
	string FontName;
	float FontSize = 32.0f;
	DWRITE_FONT_WEIGHT Weight = DWRITE_FONT_WEIGHT_NORMAL;
	DWRITE_FONT_STYLE Style = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH Stretch = DWRITE_FONT_STRETCH_NORMAL;
}TEXT_FOMAT;

typedef struct {
	string Text;
	float Width;
	float Height;
}TEXT_LAYOUT;

enum class SHADER_INDEX_P {
	DEFAULT,
	NO_TEXTURE,
	NO_LIGHT,
	SHADOW_MAP,
	GEOMETRY,
	LIGHT,
};

enum class SHADER_INDEX_V {
	DEFAULT,
	SHADOW_MAP,
	ANIMATION,
	GEOMETRY,
};

class CVertexBuffer;
class CIndexBuffer;

struct CONSTANT
{
	XMMATRIX WorldMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjectionMatrix;
};

struct CONSTANT_02
{
	XMFLOAT4 Camera_Pos;	// �J�����̍��W
};

enum RENDERING_PASS
{
	GEOMETRY = 0,
	LIGHTING,
	REDRING,
	MAX_PASS
};

/**
* @brief Directx�̃f�o�C�X�N���X
* @details Directx11�̃f�o�C�X�N���X
*/
class CRenderer {
private:

	//! DirectX�̃t���[�`���[���x��
	static D3D_FEATURE_LEVEL		m_FeatureLevel;
	//! DirectX11�̃f�o�C�X
	static ID3D11Device*			m_D3DDevice;
	//! DirectX11�̃f�o�C�X�R���e�L�X�g
	static ID3D11DeviceContext*		m_ImmediateContext;
	//! �X���b�v�`�F�C��
	static IDXGISwapChain1*			m_SwapChain;
	//! �����_�[�^�[�Q�b�g�r���[
	static ID3D11RenderTargetView*	m_RenderTargetView;

	static unique_ptr<ID3D11RenderTargetView, Release>			RenderTargetView[3];
	static unique_ptr<ID3D11ShaderResourceView, Release>		ShaderResourceView[3];

	//! �f�v�X�X�e���V��
	static ID3D11DepthStencilView*	m_DepthStencilView;
	//! Direct2D�̃f�o�C�X
	static ID2D1Device*				m_D2DDevice;
	//! Direct2D�̃f�o�C�X�R���e�L�X�g
	static ID2D1DeviceContext*		m_D2DDeviceContext;
	//! Direct2D�̃^�[�Q�b�g�r�b�g�}�b�v
	static ID2D1Bitmap1*			m_D2DTargetBitmap;
	//! Dxgi
	static IDXGIDevice1*			m_dxgiDev;

	//! �e�L�X�g�t�H�[�}�b�g
	static IDWriteTextFormat*		m_DwriteTextFormat;
	//! �e�L�X�g���C�A�E�g
	static IDWriteTextLayout*		m_TextLayout;
	//! Direct2D�̃��C�g�t�@�N�g���[
	static IDWriteFactory*			m_DwriteFactory;

	static ID3D11VertexShader*		m_VertexShader[3];
	static ID3D11PixelShader*		m_PixelShader[6];

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static ID3D11RasterizerState*	m_RasterizerState;

	//! ���_���C�A�E�g
	static ID3D11InputLayout*		m_VertexLayout;
	//! �}�e���A���o�b�t�@
	static ID3D11Buffer*			m_MaterialBuffer;
	//! ���C�g�o�b�t�@
	static ID3D11Buffer*			m_LightBuffer;
	//! �{�[�����o�b�t�@
	static ID3D11Buffer*			m_Bone_Matrix_Buffer;
	//! �R���X�^���g�o�b�t�@
	static ID3D11Buffer*			m_ConstantBuffer;
	
	static ID3D11Buffer*			m_ConstantBuffer_02;

	//! �X�^���o�C���[�h�t���O
	static bool						Stand_By_Enable;

	// ���C�g
	static LIGHT m_Light;//

	static RENDERING_PASS Pass;

	/**
	* @brief Direct3D�̏�����
	* @return bool �����Ȃ� true ���s�Ȃ� false
	* @details Direct3D�̏���������
	*/
	static bool Init3D();

	/**
	* @brief Direct2D�̏�����
	* @return bool �����Ȃ� true ���s�Ȃ� false
	* @details Direct2D�̏���������
	*/
	static bool Init2D();

public:
	static bool Init();
	static void Uninit();
	static void Begin();
	static void End();

	// ���O
	static void SetBlendState(D3D11_BLEND_DESC* blend_state = nullptr, bool flag = false);
	static void GetAddBlendState(D3D11_BLEND_DESC& blend_state);
	static void GetSubtractBlendState(D3D11_BLEND_DESC& blend_state);
	static void Get2DBlendState(D3D11_BLEND_DESC& blend_state);
	static void SetDepthEnable(bool Enable);
	static void Change_Window_Mode();

	/**
	* @brief 2D�p�̃��[���h�}�g���b�N�X�ݒ�
	* @param[in] scaling�@�g��k���̃}�g���b�N�X
	* @param[in] rotation�@��]�̃}�g���b�N�X
	* @details 2D�p�̃��[���h�}�g���b�N�X�ݒ��ݒ肷��
	*/
	static void SetWorldViewProjection2D(const XMFLOAT3& scaling = XMFLOAT3(1.0f, 1.0f, 1.0f), const XMFLOAT3& rotation = XMFLOAT3(0.0f, 0.0f, 0.0f));

	/**
	* @brief �R���X�^���g�o�b�t�@�ݒ�
	* @param[in] world�@���[���h�}�g���b�N�X
	* @param[in] view�@�r���[�}�g���b�N�X
	* @param[in] projection�@�v���W�F�N�V�����}�g���b�N�X
	* @details �R���X�^���g�o�b�t�@��ݒ肷��
	*/
	static void Set_MatrixBuffer(const XMMATRIX world, const XMMATRIX view, const XMMATRIX projection);

	static void Set_MatrixBuffer01(const XMVECTOR camera_pos)
	{
		CONSTANT_02 cons;

		XMFLOAT4 pos;

		XMStoreFloat4(&pos, camera_pos);

		cons.Camera_Pos.x = pos.x;
		cons.Camera_Pos.y = pos.y;
		cons.Camera_Pos.z = pos.z;
		cons.Camera_Pos.w = 0.0f;

		CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer_02, 0, NULL, &cons, 0, 0);
	}

	/**
	* @brief �}�e���A���ݒ�
	* @param[in] Material�@�}�e���A��
	* @details �}�e���A���ݒ��ݒ肷��
	*/
	static void SetMaterial(MATERIAL Material);

	/**
	* @brief �f�B���N�V���i�����C�g�ݒ�
	* @param[in] Light�@�f�B���N�V���i�����C�g
	* @details �f�B���N�V���i�����C�g�ݒ��ݒ肷��
	*/
	static void SetLight(LIGHT* Light);

	/**
	* @brief �f�B���N�V���i�����C�g�̏�����
	* @details �f�B���N�V���i�����C�g�ݒ������������
	*/
	static void Light_Identity();

	/**
	* @brief	���X�^���C�Y�X�e�[�g�̐ݒ�
	* @details 
	*/
	static void Set_RasterizerState();

	/**
	* @brief ���_�o�b�t�@�̐ݒ�
	* @details ���_�o�b�t�@�ݒ��ݒ肷��
	*/
	static void SetVertexBuffers( ID3D11Buffer* VertexBuffer );

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̐ݒ�
	* @details �C���f�b�N�X�o�b�t�@�ݒ��ݒ肷��
	*/
	static void SetIndexBuffer( ID3D11Buffer* IndexBuffer );

	static void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );

	/**
	* @brief �f�o�C�X3D�̎擾
	* @return ID3D11Device* �f�o�C�X�̃|�C���^
	* @details �f�o�C�X3D�̎擾����
	*/
	static ID3D11Device* GetDevice(){ return m_D3DDevice; }

	/**
	* @brief �f�o�C�X�R���e�L�X�g3D�̎擾
	* @return ID3D11DeviceContext* �f�o�C�X�R���e�L�X�g�̃|�C���^
	* @details �f�o�C�X�R���e�L�X�g3D�̎擾����
	*/
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }

	/**
	* @brief �f�o�C�X2D�̎擾
	* @return ID2D1Device* �f�o�C�X�̃|�C���^
	* @details �f�o�C�X2D�̎擾����
	*/
	static ID2D1Device* Get2DDevice() { return m_D2DDevice; }

	/**
	* @brief �f�o�C�X�R���e�L�X�g2D�̎擾
	* @return ID2D1DeviceContext* �f�o�C�X�R���e�L�X�g�̃|�C���^
	* @details �f�o�C�X�R���e�L�X�g2D�̎擾����
	*/
	static ID2D1DeviceContext* Get2DDeviceContext(void) { return m_D2DDeviceContext; }

	/**
	* @brief �e�L�X�g�t�H�[�}�b�g�̍쐬
	* @param[in] TEXT_FOMAT �e�L�X�g�t�H�[�}�b�g
	* @return HRESULT �߂�l
	* @details �e�L�X�g�t�H�[�}�b�g�̍쐬����
	*/
	static HRESULT Create_TextFormat(const TEXT_FOMAT& fomat);

	/**
	* @brief �e�L�X�g�t�H�[�}�b�g�̎擾
	* @return IDWriteTextFormat* �e�L�X�g�t�H�[�}�b�g�̃|�C���^
	* @details �e�L�X�g�t�H�[�}�b�g�̎擾����
	*/
	static IDWriteTextFormat* GetTextFormat() { return m_DwriteTextFormat; }

	/**
	* @brief �e�L�X�g���C�A�E�g���쐬
	* @param TEXT_LAYOUT �e�L�X�g���C�A�E�g
	* @return HRESULT �߂�l
	* @details �e�L�X�g���C�A�E�g���쐬����
	*/
	static HRESULT Create_TextLayout(const TEXT_LAYOUT& layout);

	/**
	* @brief �e�L�X�g���C�A�E�g���擾
	* @return IDWriteTextLayout* �e�L�X�g���C�A�E�g�̃|�C���^
	* @details �e�L�X�g���C�A�E�g���擾����
	*/
	static IDWriteTextLayout* GetTextLayout() { return m_TextLayout; }

	/**
	* @brief �_�C���N�g�t�@�N�g�����擾
	* @return IDWriteFactory* �_�C���N�g�t�@�N�g���̃|�C���^
	* @details �_�C���N�g�t�@�N�g�����擾����
	*/
	static IDWriteFactory* GetFactory() { return m_DwriteFactory; }

	static void Set_Shader(const SHADER_INDEX_V v_index = SHADER_INDEX_V::DEFAULT, const SHADER_INDEX_P p_index = SHADER_INDEX_P::DEFAULT);

	// ���O
	static void CreateRenderTexture();
	static void SetPass_Rendring();

	static void SetPass_Geometry();

	static bool Create();

	static RENDERING_PASS Get_Rendering_Pass();

	static ID3D11ShaderResourceView* Get() {
		return ShaderResourceView[0].get();
	}

	static ID3D11ShaderResourceView* Get2() {
		return ShaderResourceView[1].get();
	}

	static ID3D11ShaderResourceView* Get3() {
		return ShaderResourceView[2].get();
	}

	static LIGHT* Get_Light()
	{
		return &m_Light;
	}
};

#endif // !RENDER_H