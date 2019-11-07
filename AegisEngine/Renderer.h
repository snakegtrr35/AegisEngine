#pragma once

#ifndef RENDER_H
#define RENDER_H

#include	"main.h"

//! 頂点構造体
struct VERTEX_3D
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT4 Diffuse;
    XMFLOAT2 TexCoord;
};

//! 頂点構造体
struct VERTEX_ANIME_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	WORD BoneIndex[4];
	float BoneWeight[4];

	VERTEX_ANIME_3D() {
		Normal = Position = XMFLOAT3(0.f, 0.f, 0.f);
		Diffuse = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		TexCoord = XMFLOAT2(0.f, 0.f);
		for (int i = 0; i < 4; i++)
		{
			BoneIndex[i] = 0;
			BoneWeight[i] = 0.f;
		}
	}
};


//! 色構造体
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

	// 代入演算( = )
	COLOR& operator = (const COLOR& color)
	{
		r = color.r; g = color.g; b = color.b, a = color.a;
		return *this;
	}

	// 和( + )
	const COLOR operator + (const COLOR& color) const
	{
		return COLOR(r + color.r, g + color.g, b + color.b, a + color.a);
	}

	// 差( - )
	const COLOR operator - (const COLOR& color) const
	{
		return COLOR(r - color.r, g - color.g, b - color.b, a - color.a);
	}

	// 積( * )
	const COLOR operator * (const COLOR& color) const
	{
		return COLOR(r * color.r, g * color.g, b * color.b, a * color.a);
	}

	// 商( / )
	const COLOR operator / (const COLOR& color) const
	{
		return COLOR(r / color.r, g / color.g, b / color.b, a / color.a);
	}

	// 自身に対する和( += ) 
	COLOR& operator += (const COLOR& color)
	{
		r += color.r; g += color.g;	b += color.b; a += color.a;
		return *this;
	}

	// 自身に対する差( -= ) 
	COLOR& operator -= (const COLOR& color)
	{
		r -= color.r; g -= color.g;	b -= color.b; a -= color.a;
		return *this;
	}

	// 自身に対する積( *= ) 
	COLOR& operator *= (const COLOR& color)
	{
		r *= color.r; g *= color.g;	b *= color.b; a *= color.a;
		return *this;
	}

	// 自身に対する商( /= ) 
	COLOR& operator /= (const COLOR& color)
	{
		r /= color.r; g /= color.g;	b /= color.b; a /= color.a;
		return *this;
	}
};

//! マテリアル構造体
struct MATERIAL
{
	COLOR		Ambient;
	COLOR		Diffuse;
	COLOR		Specular;
	COLOR		Emission;
	float		Shininess;
	float		Dummy[3];//16bit境界用
};




//! マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	//class CTexture*	Texture;
};


//! 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	XMFLOAT4	Direction;
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
};

enum class SHADER_INDEX_V {
	DEFAULT,
	ANIMATION,
};

class CVertexBuffer;
class CIndexBuffer;

/**
* @brief Directxのデバイスクラス
* @details Directx11のデバイスクラス
*/
class CRenderer {
private:

	//! Directx11
	static D3D_FEATURE_LEVEL       m_FeatureLevel;
	//! Directx11のデバイス
	static ID3D11Device*           m_D3DDevice;
	//! Directx11のデバイスコンテキスト
	static ID3D11DeviceContext*    m_ImmediateContext;
	//! スワップチェイン
	static IDXGISwapChain1*		   m_SwapChain;
	//! レンダーターゲットビュー
	static ID3D11RenderTargetView* m_RenderTargetView;
	//! デプスステンシル
	static ID3D11DepthStencilView* m_DepthStencilView;
	//! Directx2Dのデバイス
	static ID2D1Device* m_D2DDevice;
	//! Directx2Dのデバイスコンテキスト
	static ID2D1DeviceContext* m_D2DDeviceContext;
	//! Directx2Dのターゲットビットマップ
	static ID2D1Bitmap1* m_D2DTargetBitmap;
	//! Dxgi
	static IDXGIDevice1* m_dxgiDev;

	//! テキストフォーマット
	static IDWriteTextFormat* m_DwriteTextFormat;
	//! テキストレイアウト
	static IDWriteTextLayout* m_TextLayout;
	//! Directx2Dのライトファクトリー
	static IDWriteFactory* m_DwriteFactory;

	static ID3D11VertexShader*		m_VertexShader[2];
	static ID3D11PixelShader*		m_PixelShader[3];

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	//! 頂点レイアウト
	static ID3D11InputLayout*		m_VertexLayout;
	//! ワールドバッファ
	static ID3D11Buffer*			m_WorldBuffer;
	//! ビューバッファ
	static ID3D11Buffer*			m_ViewBuffer;
	//! プロジェクションバッファ
	static ID3D11Buffer*			m_ProjectionBuffer;
	//! マテリアルバッファ
	static ID3D11Buffer*			m_MaterialBuffer;
	//! ライトバッファ
	static ID3D11Buffer*			m_LightBuffer;
	//! カメラバッファ
	static ID3D11Buffer* m_CameraBuffer;//
	//! ボーン情報バッファ
	static ID3D11Buffer* m_Bone_Matrix_Buffer;

public:
	static bool Init();
	static void Uninit();
	static void Begin();
	static void End();

	// 自前
	static void SetBlendState(D3D11_BLEND_DESC* blend_state = nullptr, bool flag = false);
	static void GetAddBlendState(D3D11_BLEND_DESC& blend_state);
	static void GetSubtractBlendState(D3D11_BLEND_DESC& blend_state);
	static void Get2DBlendState(D3D11_BLEND_DESC& blend_state);
	static void SetDepthEnable(bool Enable);
	static void Change_Window_Mode();

	//==============================
	// Scaling : 拡大縮小の値
	//==============================
	static void SetWorldViewProjection2D(const XMFLOAT3& scaling = XMFLOAT3(1.0f, 1.0f, 1.0f));
	static void SetWorldMatrix(XMMATRIX * WorldMatrix);
	static void SetViewMatrix(XMMATRIX * ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX * ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT* Light);
	static void Light_Identity();

	static void SetCamera(XMFLOAT4* position);//

	static void SetVertexBuffers( ID3D11Buffer* VertexBuffer );
	static void SetIndexBuffer( ID3D11Buffer* IndexBuffer );
	static void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );

	static ID3D11Device* GetDevice(){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }


	static ID2D1Device* Get2DDevice() { return m_D2DDevice; }
	static ID2D1DeviceContext* Get2DDeviceContext(void) { return m_D2DDeviceContext; }

	/**
	* @brief テキストフォーマットの作成
	* @param[in] TEXT_FOMAT テキストフォーマット
	* @return HRESULT 戻り値
	* @details テキストフォーマットの作成
	*/
	static HRESULT Create_TextFormat(const TEXT_FOMAT& fomat);
	static IDWriteTextFormat* GetTextFormat() { return m_DwriteTextFormat; }

	/**
	* @brief テキストレイアウトを作成
	* @param TEXT_LAYOUT テキストレイアウト
	* @return HRESULT 戻り値
	* @details テキストレイアウトを作成
	*/
	static HRESULT Create_TextLayout(const TEXT_LAYOUT& layout);
	static IDWriteTextLayout* GetTextLayout() { return m_TextLayout; }

	static IDWriteFactory* GetFactory() { return m_DwriteFactory; }

	static void Set_Shader(const SHADER_INDEX_V v_index = SHADER_INDEX_V::DEFAULT, const SHADER_INDEX_P p_index = SHADER_INDEX_P::DEFAULT);

	// 自前
	static void CreateRenderTexture();
	static void SetRenderTargetView(bool flag);
	static ID3D11ShaderResourceView*	Get_SRV();
	static ID3D11RenderTargetView*		My_RenderTargetView;
	static ID3D11ShaderResourceView*	My_ShaderResourceView;

	static D3D11_INPUT_ELEMENT_DESC animation_layout[6];

	static bool Stand_By_Enable;
};

#endif // !RENDER_H