
#include "main.h"
#include "Renderer.h"
#include "texture.h"
#include <io.h>

#include	"manager.h"

bool CRenderer::Stand_By_Enable = false;

D3D_FEATURE_LEVEL       CRenderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           CRenderer::m_D3DDevice = nullptr;
ID3D11DeviceContext*    CRenderer::m_ImmediateContext = nullptr;
IDXGIDevice1*			CRenderer::m_dxgiDev = nullptr;
IDXGISwapChain1*         CRenderer::m_SwapChain = nullptr;
ID3D11RenderTargetView* CRenderer::m_RenderTargetView = nullptr;
ID3D11DepthStencilView* CRenderer::m_DepthStencilView = nullptr;
ID2D1Device*			CRenderer::m_D2DDevice = nullptr;
ID2D1DeviceContext*		CRenderer::m_D2DDeviceContext = nullptr;
ID2D1Bitmap1*			CRenderer::m_D2DTargetBitmap = nullptr;

IDWriteTextFormat*		CRenderer::m_DwriteTextFormat = nullptr;
IDWriteTextLayout*		CRenderer::m_TextLayout = nullptr;

IDWriteFactory*			CRenderer::m_DwriteFactory = nullptr;//

ID3D11VertexShader*		CRenderer::m_VertexShader[3] = { nullptr };
ID3D11PixelShader*      CRenderer::m_PixelShader[2] = { nullptr };
ID3D11InputLayout*      CRenderer::m_VertexLayout = nullptr;
ID3D11Buffer*			CRenderer::m_WorldBuffer = nullptr;
ID3D11Buffer*			CRenderer::m_ViewBuffer = nullptr;
ID3D11Buffer*			CRenderer::m_ProjectionBuffer = nullptr;
ID3D11Buffer*			CRenderer::m_MaterialBuffer = nullptr;
ID3D11Buffer*			CRenderer::m_LightBuffer = nullptr;

ID3D11Buffer* CRenderer::m_Bone_Matrix_Buffer = nullptr;


ID3D11DepthStencilState* CRenderer::m_DepthStateEnable;
ID3D11DepthStencilState* CRenderer::m_DepthStateDisable;


// 自前
ID3D11RenderTargetView*			CRenderer::My_RenderTargetView = nullptr;
ID3D11ShaderResourceView*		CRenderer::My_ShaderResourceView = nullptr;

D3D11_INPUT_ELEMENT_DESC CRenderer::animation_layout[6];

bool CRenderer::Init()
{
	HRESULT hr = S_OK;

	// デバイス、スワップチェーン、コンテキスト生成

	DXGI_SWAP_CHAIN_DESC1 sc;
	ZeroMemory(&sc, sizeof(sc));

	sc.Width = SCREEN_WIDTH;
	sc.Height = SCREEN_HEIGHT;
	sc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	// スケール方法
	sc.Scaling = DXGI_SCALING_STRETCH;
	// 立体視
	sc.Stereo = 0;
	// 半透明モード
	sc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc.BufferCount = 1;
	// 描画後のバッファの扱い
	sc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	// MSAA
	sc.SampleDesc.Count = 1;
	sc.SampleDesc.Quality = 0;
	// フラグ
	sc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 解像度変更が有効

	// フラグ
	UINT d3dFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // BGRA テクスチャ有効
#ifndef NDEBUG
	//d3dFlags |= D3D11_CREATE_DEVICE_DEBUG; // Debug ビルドならエラー報告を有効
#endif

	// Direct3Dの作成
	hr = D3D11CreateDevice(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, d3dFlags,
		&m_FeatureLevel, 1, D3D11_SDK_VERSION,
		&m_D3DDevice, nullptr, &m_ImmediateContext);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// DXGIデバイスの作成
	hr = m_D3DDevice->QueryInterface<IDXGIDevice1>(&m_dxgiDev);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// キューに格納されていく描画コマンドをスワップ時に全てフラッシュする
	m_dxgiDev->SetMaximumFrameLatency(1);

	// Direct2Dのファクトリーの作成
	ID2D1Factory1* d2dFactory;
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1),
		nullptr,
		reinterpret_cast<void**>(&d2dFactory));
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	
	// Direct2Dデバイスの作成
	hr = d2dFactory->CreateDevice(m_dxgiDev, &m_D2DDevice);
	d2dFactory->Release();
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// Direct2Dデバイスコンテクストの作成
	hr = m_D2DDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&m_D2DDeviceContext);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	float dx, dy;
	m_D2DDeviceContext->GetDpi(&dx, &dy);

	// DXGIアダプタ（GPU）の取得
	IDXGIAdapter* adapter;
	hr = m_dxgiDev->GetAdapter(&adapter);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// DXGIのファクトリの作成
	IDXGIFactory2* factory;
	hr = adapter->GetParent(IID_PPV_ARGS(&factory));
	adapter->Release();
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// スワップチェインをHWNDから作成
	hr = factory->CreateSwapChainForHwnd(m_D3DDevice, GetWindow(),
		&sc, nullptr, nullptr, &m_SwapChain);
	factory->Release();
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// レンダーターゲットの取得（D3D11）
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// レンダーターゲットビューの作成
	hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_RenderTargetView);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// レンダーターゲットの取得（DXGI）
	IDXGISurface* surf;
	hr = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&surf));
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// Direct2Dの描画先となるビットマップを作成
	D2D1_BITMAP_PROPERTIES1 d2dProp =
	D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dx,
		dy);
	
	hr = m_D2DDeviceContext->CreateBitmapFromDxgiSurface(
		surf, &d2dProp, &m_D2DTargetBitmap);
	surf->Release();
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}
	
	// 描画するDirect2Dビットマップの設定
	m_D2DDeviceContext->SetTarget(m_D2DTargetBitmap);

	// DirectWriteのファクトリの作成
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_DwriteFactory),
		reinterpret_cast<IUnknown**>(&m_DwriteFactory));
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// テキストフォーマットの作成
	const float FONT_DEFAULT_SIZE = 32.0f;
	hr = m_DwriteFactory->CreateTextFormat(
		L"メイリオ", nullptr,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		FONT_DEFAULT_SIZE, L"", &m_DwriteTextFormat);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// テキストレイアウトを作成
	wstring drawText /*= L"Hello HELL World!!!\n地球の未来にご奉仕するにゃん！"*/;

	const string Text = "Hello HELL World!!!\n地球の未来にご奉仕するにゃん！";

	drawText = stringTowstring(Text);

	hr = m_DwriteFactory->CreateTextLayout(
		drawText.c_str(),
		drawText.size(),
		m_DwriteTextFormat,
		400,
		50,
		&m_TextLayout
	);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// 文字の位置の設定
	hr = m_DwriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// パラグラフの指定
	hr = m_DwriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}


	//ステンシル用テクスチャー作成
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sc.Width;
	td.Height = sc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sc.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	hr = m_D3DDevice->CreateTexture2D(&td, NULL, &depthTexture);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	hr = m_D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &m_DepthStencilView);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	//rd.FillMode = D3D11_FILL_WIREFRAME;
	//rd.CullMode = D3D11_CULL_NONE;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	m_D3DDevice->CreateRasterizerState(&rd, &rs);

	m_ImmediateContext->RSSetState(rs);



	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);



	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	hr = m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);



	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	m_ImmediateContext->PSSetSamplers(0, 1, &samplerState);

	// 頂点シェーダ生成
	{
		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);

		{
			FILE* file;
			long int fsize;

			file = fopen("vertexShader.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader[0]);

			m_D3DDevice->CreateInputLayout(layout,
				numElements,
				buffer,
				fsize,
				&m_VertexLayout);

			delete[] buffer;
		}

		{
			FILE* file;
			long int fsize;

			file = fopen("vertexShader_No_Light.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader[1]);

			m_D3DDevice->CreateInputLayout(layout,
				numElements,
				buffer,
				fsize,
				&m_VertexLayout);

			delete[] buffer;
		}
	}

	/*// 頂点シェーダ生成 アニメーション
	{
		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC animation_layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT numElements = ARRAYSIZE(animation_layout);

		{
			FILE* file;
			long int fsize;

			file = fopen("AnimationShader.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader[2]);

			m_D3DDevice->CreateInputLayout(animation_layout,
				numElements,
				buffer,
				fsize,
				&m_VertexLayout);

			delete[] buffer;
		}
	}*/

	// ピクセルシェーダ生成
	{
		{
			FILE* file;
			long int fsize;

			file = fopen("pixelShader.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			m_D3DDevice->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader[0]);

			delete[] buffer;
		}

		{
			FILE* file;
			long int fsize;

			file = fopen("pixelShader_No_Texture.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			m_D3DDevice->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader[1]);

			delete[] buffer;
		}
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_WorldBuffer);
	m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_ViewBuffer);
	m_ImmediateContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_ProjectionBuffer);
	m_ImmediateContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);

	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);
	m_ImmediateContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);

	hBufferDesc.ByteWidth = sizeof(LIGHT);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);
	m_ImmediateContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);

	/*{
		// 定数バッファ生成
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(XMMATRIX) * 128;
		hBufferDesc.StructureByteStride = sizeof(float);

		m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_Bone_Matrix_Buffer);
		m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_Bone_Matrix_Buffer);
	}*/

	// 入力レイアウト設定
	m_ImmediateContext->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	m_ImmediateContext->VSSetShader(m_VertexShader[0], NULL, 0);
	m_ImmediateContext->PSSetShader(m_PixelShader[0], NULL, 0);

	// ライト初期化
	LIGHT light;
	light.Direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetLight(&light);

	// マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = COLOR(0.3f, 0.3f, 0.3f, 1.0f);
	material.Specular = COLOR(1.0f, 1.0f, 1.0f, 0.5f);
	SetMaterial(material);

	return true;
}

void CRenderer::Uninit()
{
	{
		BOOL FullScreen = FALSE;
		if (nullptr != m_SwapChain)
		{
			m_SwapChain->GetFullscreenState(&FullScreen, NULL);

			// フルスクリーンのとき
			if (FullScreen == TRUE)
			{
				m_SwapChain->SetFullscreenState(FALSE, NULL);
			}
		}
	}

	// オブジェクト解放
	SAFE_RELEASE(m_WorldBuffer)
	SAFE_RELEASE(m_ViewBuffer)
	SAFE_RELEASE(m_ProjectionBuffer)

	SAFE_RELEASE(m_MaterialBuffer)
	SAFE_RELEASE(m_LightBuffer)
	SAFE_RELEASE(m_VertexLayout)

	SAFE_RELEASE(m_VertexShader[0])
	SAFE_RELEASE(m_PixelShader[0])

	SAFE_RELEASE(m_VertexShader[1])
	SAFE_RELEASE(m_PixelShader[1])

	SAFE_RELEASE(m_RenderTargetView)
	SAFE_RELEASE(m_SwapChain)
	SAFE_RELEASE(m_ImmediateContext)
	SAFE_RELEASE(m_D3DDevice)

	SAFE_RELEASE(m_DwriteFactory)
	SAFE_RELEASE(m_TextLayout)
	SAFE_RELEASE(m_DwriteTextFormat)
	SAFE_RELEASE(m_D2DDevice)
	SAFE_RELEASE(m_D2DDeviceContext)
	SAFE_RELEASE(m_dxgiDev)
}

void CRenderer::SetBlendState(D3D11_BLEND_DESC* blend_state, bool flag)
{
	if (flag)
	{
		// ブレンドステート設定
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		ID3D11BlendState* blendState = NULL;
		m_D3DDevice->CreateBlendState(blend_state, &blendState);
		m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
	}
	else
	{
		// ブレンドステート設定 ( 初期設定 )
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		ID3D11BlendState* blendState = NULL;
		m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
		m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
	}
}

// 加算合成用ブレンド ステートのためのを設定を取得する
void CRenderer::GetAddBlendState(D3D11_BLEND_DESC& blend_state)
{
	blend_state.AlphaToCoverageEnable = FALSE;
	blend_state.IndependentBlendEnable = FALSE;
	blend_state.RenderTarget[0].BlendEnable = TRUE;
	blend_state.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_state.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend_state.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_state.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_state.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_state.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_state.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

// 減算合成用ブレンド ステートのためのを設定を取得する
void CRenderer::GetSubtractBlendState(D3D11_BLEND_DESC& blend_state)
{
	blend_state.AlphaToCoverageEnable = FALSE;
	blend_state.IndependentBlendEnable = FALSE;
	blend_state.RenderTarget[0].BlendEnable = TRUE;
	blend_state.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_state.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend_state.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blend_state.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blend_state.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend_state.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_state.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

// 2D用のブレンド ステートのためのを設定を取得する
void CRenderer::Get2DBlendState(D3D11_BLEND_DESC& blend_state)
{
	blend_state.AlphaToCoverageEnable = FALSE;
	blend_state.IndependentBlendEnable = FALSE;
	blend_state.RenderTarget[0].BlendEnable = TRUE;
	blend_state.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_state.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend_state.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_state.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_state.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_state.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_state.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

void CRenderer::Change_Window_Mode()
{
	HRESULT hr;
	BOOL FullScreen;

	// GetFullscreenState
	hr = m_SwapChain->GetFullscreenState(&FullScreen, NULL);

	// SetFullscreenState
	hr = m_SwapChain->SetFullscreenState(!FullScreen, NULL);

	// 初期起動をフルスクリーンモードにした場合、ウィンドウモードに変更すると
	// ウィンドウがアクティブにならないので表示させる。
	ShowWindow(GetWindow(), SW_SHOW);
}

void CRenderer::Begin()
{
	// バックバッファクリア
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_ImmediateContext->ClearRenderTargetView( m_RenderTargetView, ClearColor );
	m_ImmediateContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderer::End()
{
	if (false == Stand_By_Enable)
	{
		// 描画する
	 	HRESULT hr = m_SwapChain->Present(0, 0);

		if (DXGI_STATUS_OCCLUDED == hr)
		{
			Stand_By_Enable = true;		// スタンバイモードに入る
			return;
		}

		// デバイスの消失
		{
			hr = m_D3DDevice->GetDeviceRemovedReason();

			switch (hr)
			{
				case S_OK:
					break;

					// リセット
				case DXGI_ERROR_DEVICE_HUNG:
				case DXGI_ERROR_DEVICE_RESET:
					CManager::GameEnd();
					break;

					// エラー 終了
				case DXGI_ERROR_DEVICE_REMOVED:
				case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
				case DXGI_ERROR_INVALID_CALL:
				default:
					CManager::GameEnd();
					break;
			}
		}
	}
	else
	{
		// 描画しない
		HRESULT hr = m_SwapChain->Present(0, DXGI_PRESENT_TEST);

		if (DXGI_STATUS_OCCLUDED != hr)
		{
			Stand_By_Enable = false;		// スタンバイモードを解除する
		}
	}
}

void CRenderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_ImmediateContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_ImmediateContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );
}

//==============================
// Scaling : 拡大縮小の値
//==============================
void CRenderer::SetWorldViewProjection2D(const XMFLOAT3& scaling)
{
	XMMATRIX world = XMMatrixIdentity();

	world = XMMatrixScaling(scaling.x, scaling.y, 1.0f);

	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &XMMatrixTranspose(world), 0, 0);

	XMMATRIX view;
	view = XMMatrixIdentity();
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &XMMatrixTranspose(view), 0, 0);

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	m_ImmediateContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &XMMatrixTranspose(projection), 0, 0);
}

void CRenderer::SetWorldMatrix( XMMATRIX *WorldMatrix )
{
	XMMATRIX world;
	world = *WorldMatrix;
	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &XMMatrixTranspose(world), 0, 0);
}

void CRenderer::SetViewMatrix( XMMATRIX *ViewMatrix )
{
	XMMATRIX view;
	view = *ViewMatrix;
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &XMMatrixTranspose(view), 0, 0);
}

void CRenderer::SetProjectionMatrix( XMMATRIX *ProjectionMatrix )
{
	XMMATRIX projection;
	projection = *ProjectionMatrix;
	m_ImmediateContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &XMMatrixTranspose(projection), 0, 0);
}

void CRenderer::SetMaterial( MATERIAL Material )
{
	m_ImmediateContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void CRenderer::SetLight(LIGHT* Light)
{
	m_ImmediateContext->UpdateSubresource(m_LightBuffer, 0, NULL, Light, 0, 0);
}

void CRenderer::Light_Identity()
{
	// ライト初期化
	LIGHT light;
	light.Direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = COLOR(0.2f, 0.2f, 0.2f, 1.0f);

	m_ImmediateContext->UpdateSubresource(m_LightBuffer, 0, NULL, &light, 0, 0);
}

void CRenderer::SetVertexBuffers( ID3D11Buffer* VertexBuffer )
{
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { VertexBuffer };
	m_ImmediateContext->IASetVertexBuffers( 0, 1, vb, &stride, &offset );
}

void CRenderer::SetIndexBuffer( ID3D11Buffer* IndexBuffer )
{
	m_ImmediateContext->IASetIndexBuffer( IndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
}

void CRenderer::DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation )
{
	m_ImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_ImmediateContext->DrawIndexed( IndexCount, StartIndexLocation, BaseVertexLocation );
}

// シェーダ設定
void CRenderer::Set_Shader(const SHADER_INDEX_V v_index, const SHADER_INDEX_P p_index)
{
	switch ((int)v_index)
	{
		case (int)SHADER_INDEX_V::DEFAULT:
			m_ImmediateContext->VSSetShader(m_VertexShader[0], NULL, 0);
			break;

		case (int)SHADER_INDEX_V::NO_LIGHT:
			m_ImmediateContext->VSSetShader(m_VertexShader[1], NULL, 0);
			break;

		default:
			break;
	}

	switch ((int)p_index)
	{
		case (int)SHADER_INDEX_P::DEFAULT:
			m_ImmediateContext->PSSetShader(m_PixelShader[0], NULL, 0);
			break;

		case (int)SHADER_INDEX_P::NO_TEXTURE:
			m_ImmediateContext->PSSetShader(m_PixelShader[1], NULL, 0);
			break;

		default:
			break;
	}
}

void CRenderer::CreateRenderTexture()
{
	HRESULT hr;

	// 2次元テクスチャの設定
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = SCREEN_WIDTH;
	texDesc.Height = SCREEN_HEIGHT;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	
	ID3D11Texture2D* pTex;

	// 2次元テクスチャの生成
	hr = m_D3DDevice->CreateTexture2D(&texDesc, NULL, &pTex);

	if (FAILED(hr))
	{
		FAILDE_ASSERT;
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	// レンダーターゲットビューの生成
	hr = m_D3DDevice->CreateRenderTargetView(pTex, &rtvDesc, &My_RenderTargetView);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
	}

	 // シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	
	// シェーダリソースビューの生成
	hr = m_D3DDevice->CreateShaderResourceView(pTex, &srvDesc, &My_ShaderResourceView);
	if(FAILED(hr))
	{
		FAILDE_ASSERT;
	}
}

void CRenderer::SetRenderTargetView(bool flag)
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	if (flag)
	{
		//XMFLOAT3 Position = XMFLOAT3(0.0f, 5.0f, 10.0f);
		//XMFLOAT3 Rotation = XMFLOAT3(0.5f, 0.0f, 0.0f);

		//RECT Viewport;
		//Viewport.left = 0;
		//Viewport.top = 0;
		//Viewport.right = SCREEN_WIDTH;
		//Viewport.bottom = SCREEN_HEIGHT;

		//XMMATRIX m_ViewMatrix;
		//XMMATRIX m_InvViewMatrix;
		//XMMATRIX m_ProjectionMatrix;

		//// ビューポート設定
		//D3D11_VIEWPORT dxViewport;
		//dxViewport.Width = (float)(Viewport.right - Viewport.left);
		//dxViewport.Height = (float)(Viewport.bottom - Viewport.top);
		//dxViewport.MinDepth = 0.0f;
		//dxViewport.MaxDepth = 1.0f;
		//dxViewport.TopLeftX = (float)Viewport.left;
		//dxViewport.TopLeftY = (float)Viewport.top;

		//CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

		//// ビューマトリクス設定
		//m_InvViewMatrix = XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
		//m_InvViewMatrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		//XMVECTOR det;
		//m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

		//CRenderer::SetViewMatrix(&m_ViewMatrix);

		//// プロジェクションマトリクス設定
		//m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

		//CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);

		// 自前のレンダーターゲットビューに切り替え
		m_ImmediateContext->OMSetRenderTargets(1, &My_RenderTargetView, m_DepthStencilView);
		m_ImmediateContext->ClearRenderTargetView(My_RenderTargetView, clearColor);
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	else
	{
		// デフォルトのレンダーターゲットビューに切り替え
		m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
		m_ImmediateContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

ID3D11ShaderResourceView* CRenderer::Get_SRV()
{
	return My_ShaderResourceView;
}