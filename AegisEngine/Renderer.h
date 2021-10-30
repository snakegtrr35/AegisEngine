#pragma once

#ifndef RENDER_H
#define RENDER_H

#include	"main.h"

//! 頂点構造体
struct VERTEX_3D
{
    Aegis::Vector3 Position;
	Aegis::Vector3 Normal;
	Aegis::Vector4 Diffuse;
	Aegis::Vector2 TexCoord;

	VERTEX_3D() : Position(Aegis::Vector3(0.f, 0.f, 0.f)), Normal(Aegis::Vector3(0.f, 0.f, 0.f)), Diffuse(Aegis::Vector4(0.f, 0.f, 0.f, 1.0f)), TexCoord(Aegis::Vector2(0.f, 0.f)) {}

	VERTEX_3D(XMFLOAT3 v, XMFLOAT3 n, XMFLOAT4 d, XMFLOAT2 t) : Position(v.x, v.y, v.z), Normal(n), Diffuse(d), TexCoord(t) {}

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
	Aegis::Vector3 Position;
	Aegis::Vector3 Normal;
	Aegis::Vector3 Binormal;
	Aegis::Vector3 Tangent;
	Aegis::Vector4 Diffuse;
	Aegis::Vector2 TexCoord;

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
	Aegis::COLOR Ambient;
	Aegis::COLOR Diffuse;
	Aegis::COLOR Specular;
	Aegis::COLOR Emission;
	float Shininess;
	float Dummy[3];//16bit境界用
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
	XMFLOAT4 Direction;
	XMFLOAT4 Position;
	Aegis::COLOR Diffuse;
	Aegis::COLOR Ambient;
	Aegis::COLOR Specular;
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

	static unique_ptr<CRenderer> m_Instance;

	//! DirectXのフューチャーレベル
	D3D_FEATURE_LEVEL											m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	//! DirectX11のデバイス
	ComPtr<ID3D11Device>										m_D3DDevice;
	//! DirectX11のデバイスコンテキスト
	ComPtr<ID3D11DeviceContext>									m_ImmediateContext;
	//! スワップチェイン
	ComPtr<IDXGISwapChain1>										m_SwapChain;
	//! レンダーターゲットビュー
	ComPtr<ID3D11RenderTargetView>								m_RenderTargetView;

	ComPtr<ID3D11RenderTargetView>								RenderTargetView_16bit;
	ComPtr<ID3D11ShaderResourceView>							ShaderResourceView_16bit;

	ComPtr<ID3D11RenderTargetView>								RenderTargetView[3];
	ComPtr<ID3D11ShaderResourceView>							ShaderResourceView[3];

	//! デプスステンシル
	ComPtr<ID3D11DepthStencilView>								m_DepthStencilView;
	//! Direct2Dのデバイス
	ComPtr<ID2D1Device>											m_D2DDevice;
	//! Direct2Dのデバイスコンテキスト
	ComPtr<ID2D1DeviceContext>									m_D2DDeviceContext;
	//! Direct2Dのターゲットビットマップ
	ComPtr<ID2D1Bitmap1>										m_D2DTargetBitmap;
	//! Dxgi
	ComPtr<IDXGIDevice1>										m_dxgiDev;

	//! テキストフォーマット
	ComPtr<IDWriteTextFormat>									m_DwriteTextFormat;
	//! テキストレイアウト
	ComPtr<IDWriteTextLayout>									m_TextLayout;
	//! Direct2Dのライトファクトリー
	ComPtr<IDWriteFactory>										m_DwriteFactory;

	unordered_map<SHADER_INDEX_V, ComPtr<ID3D11VertexShader>>	m_VertexShader;
	unordered_map<SHADER_INDEX_P, ComPtr<ID3D11PixelShader>>	m_PixelShader;

	ComPtr<ID3D11DepthStencilState>								m_DepthStateEnable;
	ComPtr<ID3D11DepthStencilState>								m_DepthStateDisable;
	ComPtr<ID3D11RasterizerState>								m_RasterizerState;

	ComPtr<ID3D11SamplerState>									m_SamplerState;

	//! 頂点レイアウト
	ComPtr<ID3D11InputLayout>									m_VertexLayout[3];
	//! マテリアルバッファ
	ComPtr<ID3D11Buffer>										m_MaterialBuffer;
	//! ライトバッファ
	ComPtr<ID3D11Buffer>										m_LightBuffer;
	//! ボーン情報バッファ
	ComPtr<ID3D11Buffer>										m_Bone_Matrix_Buffer;
	//! コンスタントバッファ
	ComPtr<ID3D11Buffer>										m_ConstantBuffer;
	
	ComPtr<ID3D11Buffer>										m_ConstantBuffer_02;

	//! スタンバイモードフラグ
	bool														Stand_By_Enable = false;
	// ライト
	LIGHT														m_Light;

	/**
	* @brief Direct3Dの初期化
	* @return bool 成功なら true 失敗なら false
	* @details Direct3Dの初期化する
	*/
	bool Init3D();

	/**
	* @brief Direct2Dの初期化
	* @return bool 成功なら true 失敗なら false
	* @details Direct2Dの初期化する
	*/
	bool Init2D();

public:

	CRenderer() {}
	~CRenderer() {}

	static CRenderer* getInstance();

	bool Init();
	void Uninit();
	void Begin();
	void End();

	// 自前
	void SetBlendState(D3D11_BLEND_DESC* blend_state = nullptr, bool flag = false);
	void GetAddBlendState(D3D11_BLEND_DESC& blend_state);
	void GetSubtractBlendState(D3D11_BLEND_DESC& blend_state);
	void Get2DBlendState(D3D11_BLEND_DESC& blend_state);
	void SetDepthEnable(bool Enable);
	void Change_Window_Mode();

	/**
	* @brief 2D用のワールドマトリックス設定
	* @param[in] scaling　拡大縮小のマトリックス
	* @param[in] rotation　回転のマトリックス
	* @details 2D用のワールドマトリックス設定を設定する
	*/
	void SetWorldViewProjection2D(const XMFLOAT3& scaling = XMFLOAT3(1.0f, 1.0f, 1.0f), const XMFLOAT3& rotation = XMFLOAT3(0.0f, 0.0f, 0.0f));

	/**
	* @brief コンスタントバッファ設定
	* @param[in] world　ワールドマトリックス
	* @param[in] view　ビューマトリックス
	* @param[in] projection　プロジェクションマトリックス
	* @details コンスタントバッファを設定する
	*/
	void Set_MatrixBuffer(const XMMATRIX world, const XMMATRIX view, const XMMATRIX projection);

	void Set_MatrixBuffer01(const XMVECTOR camera_pos)
	{
		CONSTANT_02 cons;

		XMFLOAT4 pos;

		XMStoreFloat4(&pos, camera_pos);

		cons.Camera_Pos.x = pos.x;
		cons.Camera_Pos.y = pos.y;
		cons.Camera_Pos.z = pos.z;
		cons.Camera_Pos.w = 0.0f;

		CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer_02.Get(), 0, NULL, &cons, 0, 0);

		m_ImmediateContext->VSSetConstantBuffers(5, 1, m_ConstantBuffer_02.GetAddressOf());
		m_ImmediateContext->PSSetConstantBuffers(5, 1, m_ConstantBuffer_02.GetAddressOf());
	}

	/**
	* @brief マテリアル設定
	* @param[in] Material　マテリアル
	* @details マテリアル設定を設定する
	*/
	void SetMaterial(MATERIAL Material);

	/**
	* @brief ディレクショナルライト設定
	* @param[in] Light　ディレクショナルライト
	* @details ディレクショナルライト設定を設定する
	*/
	void SetLight(LIGHT* Light);

	/**
	* @brief ディレクショナルライトの初期化
	* @details ディレクショナルライト設定を初期化する
	*/
	void Light_Identity();

	/**
	* @brief	ラスタライズステートの設定
	* @details 
	*/
	void Set_RasterizerState();

	/**
	* @brief 頂点バッファの設定
	* @details 頂点バッファ設定を設定する
	*/
	void SetVertexBuffers( ID3D11Buffer* VertexBuffer );

	/**
	* @brief 頂点バッファの設定(インスタンシング用)
	* @details インスタンシング用の頂点バッファ設定を設定する
	*/
	void SetVertexBuffers(ID3D11Buffer* IndexBuffer, ID3D11Buffer* InstancingBuffer, UINT size);

	/**
	* @brief インデックスバッファの設定
	* @details インデックスバッファ設定を設定する
	*/
	void SetIndexBuffer( ID3D11Buffer* IndexBuffer );

	void Draw(unsigned int IndexCount, unsigned int StartVertexLocation);
	void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );

	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology);

	/**
	* @brief デバイス3Dの取得
	* @return ID3D11Device* デバイスのポインタ
	* @details デバイス3Dの取得する
	*/
	ID3D11Device* GetDevice(){ return m_D3DDevice.Get(); }

	/**
	* @brief デバイスコンテキスト3Dの取得
	* @return ID3D11DeviceContext* デバイスコンテキストのポインタ
	* @details デバイスコンテキスト3Dの取得する
	*/
	ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext.Get(); }

	/**
	* @brief デバイス2Dの取得
	* @return ID2D1Device* デバイスのポインタ
	* @details デバイス2Dの取得する
	*/
	ID2D1Device* Get2DDevice() { return m_D2DDevice.Get(); }

	/**
	* @brief デバイスコンテキスト2Dの取得
	* @return ID2D1DeviceContext* デバイスコンテキストのポインタ
	* @details デバイスコンテキスト2Dの取得する
	*/
	ID2D1DeviceContext* Get2DDeviceContext(void) { return m_D2DDeviceContext.Get(); }

	/**
	* @brief テキストフォーマットの作成
	* @param[in] TEXT_FOMAT テキストフォーマット
	* @return HRESULT 戻り値
	* @details テキストフォーマットの作成する
	*/
	HRESULT Create_TextFormat(const TEXT_FOMAT& fomat);

	/**
	* @brief テキストフォーマットの取得
	* @return IDWriteTextFormat* テキストフォーマットのポインタ
	* @details テキストフォーマットの取得する
	*/
	IDWriteTextFormat* GetTextFormat() { return m_DwriteTextFormat.Get(); }

	/**
	* @brief テキストレイアウトを作成
	* @param TEXT_LAYOUT テキストレイアウト
	* @return HRESULT 戻り値
	* @details テキストレイアウトを作成する
	*/
	HRESULT Create_TextLayout(const TEXT_LAYOUT& layout);

	/**
	* @brief テキストレイアウトを取得
	* @return IDWriteTextLayout* テキストレイアウトのポインタ
	* @details テキストレイアウトを取得する
	*/
	IDWriteTextLayout* GetTextLayout() { return m_TextLayout.Get(); }

	/**
	* @brief ダイレクトファクトリを取得
	* @return IDWriteFactory* ダイレクトファクトリのポインタ
	* @details ダイレクトファクトリを取得する
	*/
	IDWriteFactory* GetFactory() { return m_DwriteFactory.Get(); }

	void Set_Shader(const SHADER_INDEX_V v_index = SHADER_INDEX_V::DEFAULT, const SHADER_INDEX_P p_index = SHADER_INDEX_P::DEFAULT);

	void Set_InputLayout(const INPUTLAYOUT v_index = INPUTLAYOUT::DEFAULT);

	// 自前
	void CreateRenderTexture();
	void SetPass_Rendring();

	void SetPass_Geometry();

	bool Create();

	ID3D11ShaderResourceView* Get() {
		return ShaderResourceView[0].Get();
	}

	ID3D11ShaderResourceView* Get2() {
		return ShaderResourceView[1].Get();
	}

	ID3D11ShaderResourceView* Get3() {
		return ShaderResourceView[2].Get();
	}

	ID3D11ShaderResourceView* Get_Texture() {
		return ShaderResourceView_16bit.Get();
	}

	void End_Draw();

	LIGHT* Get_Light()
	{
		return &m_Light;
	}
};

#endif // !RENDER_H