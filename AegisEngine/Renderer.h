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

	VERTEX_ANIME_3D() : Normal(XMFLOAT3(0.f, 0.f, 0.f)), Position(XMFLOAT3(0.f, 0.f, 0.f)), Diffuse(XMFLOAT4(0.f, 0.f, 0.f, 0.f)), TexCoord(XMFLOAT2(0.f, 0.f)) {
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
	XMFLOAT4 Camera_Pos;	// カメラの座標
};

enum RENDERING_PASS
{
	GEOMETRY = 0,
	LIGHTING,
	REDRING,
	MAX_PASS
};

/**
* @brief Directxのデバイスクラス
* @details Directx11のデバイスクラス
*/
class CRenderer {
private:

	//! DirectXのフューチャーレベル
	static D3D_FEATURE_LEVEL		m_FeatureLevel;
	//! DirectX11のデバイス
	static ID3D11Device*			m_D3DDevice;
	//! DirectX11のデバイスコンテキスト
	static ID3D11DeviceContext*		m_ImmediateContext;
	//! スワップチェイン
	static IDXGISwapChain1*			m_SwapChain;
	//! レンダーターゲットビュー
	static ID3D11RenderTargetView*	m_RenderTargetView;

	static unique_ptr<ID3D11RenderTargetView, Release>			RenderTargetView[3];
	static unique_ptr<ID3D11ShaderResourceView, Release>		ShaderResourceView[3];

	//! デプスステンシル
	static ID3D11DepthStencilView*	m_DepthStencilView;
	//! Direct2Dのデバイス
	static ID2D1Device*				m_D2DDevice;
	//! Direct2Dのデバイスコンテキスト
	static ID2D1DeviceContext*		m_D2DDeviceContext;
	//! Direct2Dのターゲットビットマップ
	static ID2D1Bitmap1*			m_D2DTargetBitmap;
	//! Dxgi
	static IDXGIDevice1*			m_dxgiDev;

	//! テキストフォーマット
	static IDWriteTextFormat*		m_DwriteTextFormat;
	//! テキストレイアウト
	static IDWriteTextLayout*		m_TextLayout;
	//! Direct2Dのライトファクトリー
	static IDWriteFactory*			m_DwriteFactory;

	static ID3D11VertexShader*		m_VertexShader[3];
	static ID3D11PixelShader*		m_PixelShader[6];

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static ID3D11RasterizerState*	m_RasterizerState;

	//! 頂点レイアウト
	static ID3D11InputLayout*		m_VertexLayout;
	//! マテリアルバッファ
	static ID3D11Buffer*			m_MaterialBuffer;
	//! ライトバッファ
	static ID3D11Buffer*			m_LightBuffer;
	//! ボーン情報バッファ
	static ID3D11Buffer*			m_Bone_Matrix_Buffer;
	//! コンスタントバッファ
	static ID3D11Buffer*			m_ConstantBuffer;
	
	static ID3D11Buffer*			m_ConstantBuffer_02;

	//! スタンバイモードフラグ
	static bool						Stand_By_Enable;

	// ライト
	static LIGHT m_Light;//

	static RENDERING_PASS Pass;

	/**
	* @brief Direct3Dの初期化
	* @return bool 成功なら true 失敗なら false
	* @details Direct3Dの初期化する
	*/
	static bool Init3D();

	/**
	* @brief Direct2Dの初期化
	* @return bool 成功なら true 失敗なら false
	* @details Direct2Dの初期化する
	*/
	static bool Init2D();

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

	/**
	* @brief 2D用のワールドマトリックス設定
	* @param[in] scaling　拡大縮小のマトリックス
	* @param[in] rotation　回転のマトリックス
	* @details 2D用のワールドマトリックス設定を設定する
	*/
	static void SetWorldViewProjection2D(const XMFLOAT3& scaling = XMFLOAT3(1.0f, 1.0f, 1.0f), const XMFLOAT3& rotation = XMFLOAT3(0.0f, 0.0f, 0.0f));

	/**
	* @brief コンスタントバッファ設定
	* @param[in] world　ワールドマトリックス
	* @param[in] view　ビューマトリックス
	* @param[in] projection　プロジェクションマトリックス
	* @details コンスタントバッファを設定する
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
	* @brief マテリアル設定
	* @param[in] Material　マテリアル
	* @details マテリアル設定を設定する
	*/
	static void SetMaterial(MATERIAL Material);

	/**
	* @brief ディレクショナルライト設定
	* @param[in] Light　ディレクショナルライト
	* @details ディレクショナルライト設定を設定する
	*/
	static void SetLight(LIGHT* Light);

	/**
	* @brief ディレクショナルライトの初期化
	* @details ディレクショナルライト設定を初期化する
	*/
	static void Light_Identity();

	/**
	* @brief	ラスタライズステートの設定
	* @details 
	*/
	static void Set_RasterizerState();

	/**
	* @brief 頂点バッファの設定
	* @details 頂点バッファ設定を設定する
	*/
	static void SetVertexBuffers( ID3D11Buffer* VertexBuffer );

	/**
	* @brief インデックスバッファの設定
	* @details インデックスバッファ設定を設定する
	*/
	static void SetIndexBuffer( ID3D11Buffer* IndexBuffer );

	static void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );

	/**
	* @brief デバイス3Dの取得
	* @return ID3D11Device* デバイスのポインタ
	* @details デバイス3Dの取得する
	*/
	static ID3D11Device* GetDevice(){ return m_D3DDevice; }

	/**
	* @brief デバイスコンテキスト3Dの取得
	* @return ID3D11DeviceContext* デバイスコンテキストのポインタ
	* @details デバイスコンテキスト3Dの取得する
	*/
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }

	/**
	* @brief デバイス2Dの取得
	* @return ID2D1Device* デバイスのポインタ
	* @details デバイス2Dの取得する
	*/
	static ID2D1Device* Get2DDevice() { return m_D2DDevice; }

	/**
	* @brief デバイスコンテキスト2Dの取得
	* @return ID2D1DeviceContext* デバイスコンテキストのポインタ
	* @details デバイスコンテキスト2Dの取得する
	*/
	static ID2D1DeviceContext* Get2DDeviceContext(void) { return m_D2DDeviceContext; }

	/**
	* @brief テキストフォーマットの作成
	* @param[in] TEXT_FOMAT テキストフォーマット
	* @return HRESULT 戻り値
	* @details テキストフォーマットの作成する
	*/
	static HRESULT Create_TextFormat(const TEXT_FOMAT& fomat);

	/**
	* @brief テキストフォーマットの取得
	* @return IDWriteTextFormat* テキストフォーマットのポインタ
	* @details テキストフォーマットの取得する
	*/
	static IDWriteTextFormat* GetTextFormat() { return m_DwriteTextFormat; }

	/**
	* @brief テキストレイアウトを作成
	* @param TEXT_LAYOUT テキストレイアウト
	* @return HRESULT 戻り値
	* @details テキストレイアウトを作成する
	*/
	static HRESULT Create_TextLayout(const TEXT_LAYOUT& layout);

	/**
	* @brief テキストレイアウトを取得
	* @return IDWriteTextLayout* テキストレイアウトのポインタ
	* @details テキストレイアウトを取得する
	*/
	static IDWriteTextLayout* GetTextLayout() { return m_TextLayout; }

	/**
	* @brief ダイレクトファクトリを取得
	* @return IDWriteFactory* ダイレクトファクトリのポインタ
	* @details ダイレクトファクトリを取得する
	*/
	static IDWriteFactory* GetFactory() { return m_DwriteFactory; }

	static void Set_Shader(const SHADER_INDEX_V v_index = SHADER_INDEX_V::DEFAULT, const SHADER_INDEX_P p_index = SHADER_INDEX_P::DEFAULT);

	// 自前
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