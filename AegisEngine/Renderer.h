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

	VERTEX_3D() : Position(XMFLOAT3(0.f, 0.f, 0.f)), Normal(XMFLOAT3(0.f, 0.f, 0.f)), Diffuse(XMFLOAT4(0.f, 0.f, 0.f, 1.0f)), TexCoord(XMFLOAT2(0.f, 0.f)) {}

	VERTEX_3D(XMFLOAT3 v, XMFLOAT3 n, XMFLOAT4 d, XMFLOAT2 t) : Position(v), Normal(n), Diffuse(d), TexCoord(t) {}

	template<class Archive>
	void serialize(Archive& ar) {
		ar(Position);
		ar(Normal);
		ar(Diffuse);
		ar(TexCoord);
	}
};

//! 頂点構造体
struct VERTEX_3D_NORMAL
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT3 Binormal;
	XMFLOAT3 Tangent;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;

	template<class Archive>
	void serialize(Archive& ar) {
		ar(Position);
		ar(Normal);
		ar(Binormal);
		ar(Tangent);
		ar(Diffuse);
		ar(TexCoord);
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

enum class SHADER_INDEX_V {
	DEFAULT,
	ANIMATION,
	SKYBOX,
	SHADOW_MAP,
	SHADOW_MAP_ANIMATION,
	INSTANCING,
	DEPTH_PRE,
	DEPTH_PRE_ANIME,
	MAX,
};

enum class SHADER_INDEX_P {
	DEFAULT,
	NO_TEXTURE,
	NO_LIGHT,
	SKYBOX,
	SHADOW_MAP,
	POST_EFFECT,
	MAX
};

class CVertexBuffer;
class CIndexBuffer;

struct CONSTANT {
	XMMATRIX WorldMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjectionMatrix;
};

struct CONSTANT_02 {
	XMFLOAT4 Camera_Pos;	// カメラの座標
};

enum INPUTLAYOUT {
	DEFAULT = 0,
	ANIMATION,
	INSTANCING,
	MAX
};

/**
* @brief Directxのデバイスクラス
* @details Directx11のデバイスクラス
*/
class CRenderer {
private:

	//! DirectXのフューチャーレベル
	static D3D_FEATURE_LEVEL									m_FeatureLevel;
	//! DirectX11のデバイス
	static ID3D11Device*										m_D3DDevice;
	//! DirectX11のデバイスコンテキスト
	static ID3D11DeviceContext*									m_ImmediateContext;
	//! スワップチェイン
	static IDXGISwapChain1*										m_SwapChain;
	//! レンダーターゲットビュー
	static ID3D11RenderTargetView*								m_RenderTargetView;

	static unique_ptr < ID3D11RenderTargetView, Release>		RenderTargetView_16bit;
	static unique_ptr<ID3D11ShaderResourceView, Release>		ShaderResourceView_16bit;

	static unique_ptr<ID3D11RenderTargetView, Release>			RenderTargetView[3];
	static unique_ptr<ID3D11ShaderResourceView, Release>		ShaderResourceView[3];

	//! デプスステンシル
	static ID3D11DepthStencilView*								m_DepthStencilView;
	//! Direct2Dのデバイス
	static ID2D1Device*											m_D2DDevice;
	//! Direct2Dのデバイスコンテキスト
	static ID2D1DeviceContext*									m_D2DDeviceContext;
	//! Direct2Dのターゲットビットマップ
	static ID2D1Bitmap1*										m_D2DTargetBitmap;
	//! Dxgi
	static IDXGIDevice1*										m_dxgiDev;

	//! テキストフォーマット
	static IDWriteTextFormat*									m_DwriteTextFormat;
	//! テキストレイアウト
	static IDWriteTextLayout*									m_TextLayout;
	//! Direct2Dのライトファクトリー
	static IDWriteFactory*										m_DwriteFactory;

	static unordered_map<SHADER_INDEX_V, ID3D11VertexShader*>	m_VertexShader;
	static unordered_map<SHADER_INDEX_P, ID3D11PixelShader*>	m_PixelShader;

	static ID3D11DepthStencilState*								m_DepthStateEnable;
	static ID3D11DepthStencilState*								m_DepthStateDisable;
	static ID3D11RasterizerState*								m_RasterizerState;

	static unique_ptr<ID3D11SamplerState, Release>				m_SamplerState;

	//! 頂点レイアウト
	static ID3D11InputLayout*									m_VertexLayout[3];
	//! マテリアルバッファ
	static ID3D11Buffer*										m_MaterialBuffer;
	//! ライトバッファ
	static ID3D11Buffer*										m_LightBuffer;
	//! ボーン情報バッファ
	static ID3D11Buffer*										m_Bone_Matrix_Buffer;
	//! コンスタントバッファ
	static ID3D11Buffer*										m_ConstantBuffer;
	
	static ID3D11Buffer*										m_ConstantBuffer_02;

	//! スタンバイモードフラグ
	static bool													Stand_By_Enable;

	// ライト
	static LIGHT m_Light;//

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

		m_ImmediateContext->VSSetConstantBuffers(5, 1, &m_ConstantBuffer_02);
		m_ImmediateContext->PSSetConstantBuffers(5, 1, &m_ConstantBuffer_02);
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
	* @brief 頂点バッファの設定(インスタンシング用)
	* @details インスタンシング用の頂点バッファ設定を設定する
	*/
	static void SetVertexBuffers(ID3D11Buffer* IndexBuffer, ID3D11Buffer* InstancingBuffer, UINT size);

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

	static void Set_InputLayout(const INPUTLAYOUT v_index = INPUTLAYOUT::DEFAULT);

	// 自前
	static void CreateRenderTexture();
	static void SetPass_Rendring();

	static void SetPass_Geometry();

	static bool Create();

	static ID3D11ShaderResourceView* Get() {
		return ShaderResourceView[0].get();
	}

	static ID3D11ShaderResourceView* Get2() {
		return ShaderResourceView[1].get();
	}

	static ID3D11ShaderResourceView* Get3() {
		return ShaderResourceView[2].get();
	}

	static ID3D11ShaderResourceView* Get_Texture() {
		return ShaderResourceView_16bit.get();
	}

	static void End_Draw();

	static LIGHT* Get_Light()
	{
		return &m_Light;
	}
};

#endif // !RENDER_H