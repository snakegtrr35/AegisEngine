
#include "main.h"
#include "Renderer.h"
#include "texture.h"
#include <io.h>

#include	"manager.h"

D3D_FEATURE_LEVEL											CRenderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*												CRenderer::m_D3DDevice = nullptr;
ID3D11DeviceContext*										CRenderer::m_ImmediateContext = nullptr;
IDXGIDevice1*												CRenderer::m_dxgiDev = nullptr;
IDXGISwapChain1*											CRenderer::m_SwapChain = nullptr;//
ID3D11RenderTargetView*										CRenderer::m_RenderTargetView = nullptr;

unique_ptr < ID3D11RenderTargetView, Release>				CRenderer::RenderTargetView_16bit;
unique_ptr<ID3D11ShaderResourceView, Release>				CRenderer::ShaderResourceView_16bit;

ID3D11DepthStencilView*										CRenderer::m_DepthStencilView = nullptr;
ID2D1Device*												CRenderer::m_D2DDevice = nullptr;
ID2D1DeviceContext*											CRenderer::m_D2DDeviceContext = nullptr;
ID2D1Bitmap1*												CRenderer::m_D2DTargetBitmap = nullptr;

IDWriteTextFormat*											CRenderer::m_DwriteTextFormat = nullptr;
IDWriteTextLayout*											CRenderer::m_TextLayout = nullptr;

IDWriteFactory*												CRenderer::m_DwriteFactory = nullptr;

unordered_map<SHADER_INDEX_V, ID3D11VertexShader*>			CRenderer::m_VertexShader;
unordered_map<SHADER_INDEX_P, ID3D11PixelShader*>			CRenderer::m_PixelShader;
ID3D11InputLayout*											CRenderer::m_VertexLayout[3] = { nullptr };
ID3D11Buffer*												CRenderer::m_MaterialBuffer = nullptr;
ID3D11Buffer*												CRenderer::m_LightBuffer = nullptr;

unique_ptr<ID3D11SamplerState, Release>						CRenderer::m_SamplerState;

ID3D11Buffer*												CRenderer::m_Bone_Matrix_Buffer = nullptr;

ID3D11DepthStencilState*									CRenderer::m_DepthStateEnable = nullptr;
ID3D11DepthStencilState*									CRenderer::m_DepthStateDisable = nullptr;
ID3D11RasterizerState*										CRenderer::m_RasterizerState = nullptr;

bool CRenderer::Stand_By_Enable = false;


ID3D11Buffer*												CRenderer::m_ConstantBuffer;//
ID3D11Buffer*												CRenderer::m_ConstantBuffer_02;

unique_ptr<ID3D11RenderTargetView, Release>					CRenderer::RenderTargetView[3];
unique_ptr<ID3D11ShaderResourceView, Release>				CRenderer::ShaderResourceView[3];

// ライト
LIGHT CRenderer::m_Light;//

IDXGIAdapter3* pAdapter = nullptr;
DXGI_QUERY_VIDEO_MEMORY_INFO info;

bool CRenderer::Init()
{
	HRESULT hr = S_OK;

	if (false == Init3D())
	{
		return false;
	}

	if (false == Init2D())
	{
		return false;
	}

	//if (false == Create())
	//{
	//	return false;
	//}

	// 頂点シェーダ生成
	{
		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);


		// 頂点シェーダ生成 デフォルト
		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::DEFAULT]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			hr = m_D3DDevice->CreateInputLayout(layout, numElements, buffer, fsize, &m_VertexLayout[INPUTLAYOUT::DEFAULT]);
			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		// 頂点シェーダ生成 スカイボックス
		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader_Skybox.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::SKYBOX]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}

		// 頂点シェーダ生成 シャドウマップ
		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader_ShadowMap.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::SHADOW_MAP]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}

		// 頂点シェーダ生成 (Depth Pre Pass)
		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader_Depth_Pre_Pass.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::DEPTH_PRE]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}
	}

	// 頂点シェーダ生成 アニメーション
	{
		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC animation_layout[] =
		{
			{ "POSITION",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICE", 0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT numElements = ARRAYSIZE(animation_layout);

		// 頂点シェーダ生成 アニメーション
		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader_Animation.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::ANIMATION]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			hr = m_D3DDevice->CreateInputLayout(animation_layout, numElements, buffer, fsize, &m_VertexLayout[INPUTLAYOUT::ANIMATION]);
			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		// 頂点シェーダ生成 アニメーション(シャドウマップ付き)
		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader_ShadowMap_Anime.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::SHADOW_MAP_ANIMATION]);
			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		// 頂点シェーダ生成 アニメーション(Depth Pre Pass)
		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader_Animation_Depth_Pre_Pass.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::DEPTH_PRE_ANIME]);
			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
	}

	// 頂点シェーダ生成 インスタンシング
	{
		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC animation_layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },

			{ "MATRIX",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
			{ "MATRIX",		1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
			{ "MATRIX",		2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
			{ "MATRIX",		3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
		};

		UINT numElements = ARRAYSIZE(animation_layout);

		{
			FILE* file;
			long int fsize;

			file = fopen("VertexShader_Instancing.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreateVertexShader(buffer, fsize, nullptr, &m_VertexShader[SHADER_INDEX_V::INSTANCING]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			hr = m_D3DDevice->CreateInputLayout(animation_layout, numElements, buffer, fsize, &m_VertexLayout[INPUTLAYOUT::INSTANCING]);
			delete[] buffer;

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
	}

	// ピクセルシェーダ生成
	{
		// 標準
		{
			FILE* file;
			long int fsize;

			file = fopen("PixelShader.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreatePixelShader(buffer, fsize, nullptr, &m_PixelShader[SHADER_INDEX_P::DEFAULT]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}

		// テクスチャなし
		{
			FILE* file;
			long int fsize;

			file = fopen("PixelShader_No_Texture.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreatePixelShader(buffer, fsize, nullptr, &m_PixelShader[SHADER_INDEX_P::NO_TEXTURE]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}

		// ライティングなし
		{
			FILE* file;
			long int fsize;

			file = fopen("PixelShader_No_Light.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreatePixelShader(buffer, fsize, nullptr, &m_PixelShader[SHADER_INDEX_P::NO_LIGHT]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}

		// スカイボックス
		{
			FILE* file;
			long int fsize;

			file = fopen("PixelShader_Skybox.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreatePixelShader(buffer, fsize, nullptr, &m_PixelShader[SHADER_INDEX_P::SKYBOX]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}

		// シャドウマップ
		{
			FILE* file;
			long int fsize;

			file = fopen("PixelShader_ShadowMap.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreatePixelShader(buffer, fsize, nullptr, &m_PixelShader[SHADER_INDEX_P::SHADOW_MAP]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}

		// ポストエフェクト
		{
			FILE* file;
			long int fsize;

			file = fopen("pixelShader _PostEffect.cso", "rb");
			fsize = _filelength(_fileno(file));
			unsigned char* buffer = new unsigned char[fsize];
			fread(buffer, fsize, 1, file);
			fclose(file);

			hr = m_D3DDevice->CreatePixelShader(buffer, fsize, nullptr, &m_PixelShader[SHADER_INDEX_P::POST_EFFECT]);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			delete[] buffer;
		}
	}

	{
		m_VertexShader[SHADER_INDEX_V::MAX] = nullptr;
		m_PixelShader[SHADER_INDEX_P::MAX] = nullptr;
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	hr = m_D3DDevice->CreateBuffer(&hBufferDesc, nullptr, &m_MaterialBuffer);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	m_ImmediateContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
	m_ImmediateContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);

	hBufferDesc.ByteWidth = sizeof(LIGHT);

	hr = m_D3DDevice->CreateBuffer(&hBufferDesc, nullptr, &m_LightBuffer);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	m_ImmediateContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_ImmediateContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);

	hBufferDesc.ByteWidth = sizeof(CONSTANT);
	hr = m_D3DDevice->CreateBuffer(&hBufferDesc, nullptr, &m_ConstantBuffer);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_ImmediateContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	hBufferDesc.ByteWidth = sizeof(CONSTANT_02);
	hr = m_D3DDevice->CreateBuffer(&hBufferDesc, nullptr, &m_ConstantBuffer_02);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	m_ImmediateContext->VSSetConstantBuffers(5, 1, &m_ConstantBuffer_02);
	m_ImmediateContext->PSSetConstantBuffers(5, 1, &m_ConstantBuffer_02);

	// 入力レイアウト設定
	m_ImmediateContext->IASetInputLayout(m_VertexLayout[0]);

	// シェーダ設定
	m_ImmediateContext->VSSetShader(m_VertexShader[SHADER_INDEX_V::DEFAULT], nullptr, 0);
	m_ImmediateContext->PSSetShader(m_PixelShader[SHADER_INDEX_P::DEFAULT], nullptr, 0);

	// ライト初期化
	ZeroMemory(&m_Light, sizeof(m_Light));
	m_Light.Position = XMFLOAT4(20.0f, 20.0f, -20.0f, 0.0f);
	m_Light.Direction = m_Light.Position * -1.0f;
	m_Light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light.Specular = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetLight(&m_Light);

	// マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = COLOR(0.5f, 0.5f, 0.5f, 1.0f);
	material.Specular = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	return true;
}

void CRenderer::Uninit()
{
	{
		BOOL FullScreen = FALSE;
		if (nullptr != m_SwapChain)
		{
			m_SwapChain->GetFullscreenState(&FullScreen, nullptr);

			// フルスクリーンのとき
			if (FullScreen == TRUE)
			{
				m_SwapChain->SetFullscreenState(FALSE, nullptr);
			}
		}
	}

	// オブジェクト解放
	SAFE_RELEASE(m_MaterialBuffer);
	SAFE_RELEASE(m_LightBuffer);

	SAFE_RELEASE(m_VertexLayout[0]);
	SAFE_RELEASE(m_VertexLayout[1]);
	SAFE_RELEASE(m_VertexLayout[2]);

	for (auto s : m_VertexShader)
	{
		if(nullptr != s.second) s.second->Release();
	}

	for (auto s : m_PixelShader)
	{
		if (nullptr != s.second) s.second->Release();
	}

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

	SAFE_RELEASE(m_ConstantBuffer);

	SAFE_RELEASE(m_ConstantBuffer_02);
}

bool CRenderer::Init3D()
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
	// 使用方法
	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc.BufferCount = 1;
	// 描画後のバッファの扱い
	sc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	// MSAA
	sc.SampleDesc.Count = 1;	// MSAA用 2 4 8 が使用可能(多分これだけ)
	sc.SampleDesc.Quality = 0;
	// フラグ
	sc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 解像度変更が有効

	// フラグ
	UINT d3dFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // BGRA テクスチャ有効(Direct2Dには必ず必要)

	// Direct3Dの作成
	{
		// アダプターの列挙
		vector<IDXGIAdapter*> Adapters;
		{
			IDXGIFactory1* pDXGIFactory = nullptr;
			IDXGIAdapter* pAdapter = nullptr;

			DXGI_ADAPTER_DESC desc;
			wstring str;

			//ファクトリの作成
			hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDXGIFactory));
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			for (UINT index = 0; ; index++)
			{
				hr = pDXGIFactory->EnumAdapters(index, &pAdapter);
				if (FAILED(hr))
					break;

				if (SUCCEEDED(pAdapter->GetDesc(&desc)))
				{
					str = wstring(desc.Description);

					if (wstring::npos == str.find(L"Microsoft"))
					{
						Adapters.emplace_back(pAdapter);
					}
					else
					{
						SAFE_RELEASE(pAdapter);
					}
				}
			}

			SAFE_RELEASE(pDXGIFactory);
		}

		// 内蔵GPUじゃないGPUがある
		if (false == Adapters.empty())
		{
			// Direct3Dの作成
			hr = D3D11CreateDevice(Adapters.front(), D3D_DRIVER_TYPE_UNKNOWN, 0, d3dFlags, &m_FeatureLevel, 1, D3D11_SDK_VERSION, &m_D3DDevice, nullptr, &m_ImmediateContext);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
		else// 内蔵GPUしかない
		{
			// Direct3Dの作成
			hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, d3dFlags, &m_FeatureLevel, 1, D3D11_SDK_VERSION, &m_D3DDevice, nullptr, &m_ImmediateContext);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		for (auto& adap : Adapters)
		{
			adap->Release();
		}
		Adapters.clear();
	}

	//// MSAA用
	//for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
	//{
	//	UINT Quality;
	//	if (SUCCEEDED(m_D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)))
	//	{
	//		if (0 < Quality)
	//		{
	//			sc.SampleDesc.Count = i;
	//			sc.SampleDesc.Quality = Quality - 1;
	//		}
	//	}
	//}

	// DXGIデバイスの作成
	hr = m_D3DDevice->QueryInterface<IDXGIDevice1>(&m_dxgiDev);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// キューに格納されていく描画コマンドをスワップ時に全てフラッシュする
	m_dxgiDev->SetMaximumFrameLatency(1);

	// DXGIアダプタ（GPU）の取得
	IDXGIAdapter* adapter = nullptr;
	hr = m_dxgiDev->GetAdapter(&adapter);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	{
		// DXGIアダプタ（GPU）の取得
		hr = m_dxgiDev->GetAdapter((IDXGIAdapter**)&pAdapter);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		pAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);
	}

	// DXGIのファクトリの作成
	IDXGIFactory2* factory = nullptr;
	hr = adapter->GetParent(IID_PPV_ARGS(&factory));
	adapter->Release();
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// スワップチェインをHWNDから作成
	hr = factory->CreateSwapChainForHwnd(m_D3DDevice, GetWindow(), &sc, nullptr, nullptr, &m_SwapChain);
	factory->Release();
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// レンダーターゲットの取得（D3D11）
	ID3D11Texture2D* pBackBuffer = nullptr;
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

	{
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = SCREEN_WIDTH;
		td.Height = SCREEN_HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = m_D3DDevice->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// レンダーターゲットビュー設定
		{
			ID3D11RenderTargetView* pRtv = nullptr;

			hr = m_D3DDevice->CreateRenderTargetView(pTex, nullptr, &pRtv);
			if (FAILED(hr))
			{
				return false;
			}

			RenderTargetView[0].reset(pRtv);
		}

		// シェーダーリソースビュー設定
		{
			ID3D11ShaderResourceView* srv = nullptr;

			hr = m_D3DDevice->CreateShaderResourceView(pTex, nullptr, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
			ShaderResourceView[0].reset(srv);
		}
	}

	{
		// アルベドテクスチャの作成
		{
			ID3D11Texture2D* pTex = nullptr;

			// テクスチャの作成
			D3D11_TEXTURE2D_DESC td;
			td.Width = SCREEN_WIDTH;
			td.Height = SCREEN_HEIGHT;
			td.MipLevels = 1;
			td.ArraySize = 1;
			td.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			td.SampleDesc.Count = 1;
			td.SampleDesc.Quality = 0;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			td.CPUAccessFlags = 0;
			td.MiscFlags = 0;

			hr = m_D3DDevice->CreateTexture2D(&td, nullptr, &pTex);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			// レンダーターゲットビュー設定
			{
				ID3D11RenderTargetView* pRtv = nullptr;

				hr = m_D3DDevice->CreateRenderTargetView(pTex, nullptr, &pRtv);
				if (FAILED(hr))
				{
					return false;
				}

				RenderTargetView_16bit.reset(pRtv);
			}

			// シェーダーリソースビュー設定
			{
				ID3D11ShaderResourceView* srv = nullptr;

				hr = m_D3DDevice->CreateShaderResourceView(pTex, nullptr, &srv);
				if (FAILED(hr))
				{
					FAILDE_ASSERT;
					return false;
				}
				ShaderResourceView_16bit.reset(srv);
			}
		}
	}

	//ステンシル用テクスチャー作成
	ID3D11Texture2D* depthTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sc.Width;
	td.Height = sc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R32_TYPELESS;
	td.SampleDesc = sc.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	hr = m_D3DDevice->CreateTexture2D(&td, nullptr, &depthTexture);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;		// MSAA用
	dsvd.Texture2D.MipSlice = 0;
	dsvd.Flags = 0;
	hr = m_D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &m_DepthStencilView);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	//// シェーダーリソースビュー設定
	//{
	//	ID3D11ShaderResourceView* srv = nullptr;

	//	// シェーダーリソースビューの設定
	//	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	//	desc.Format = DXGI_FORMAT_R32_FLOAT;
	//	desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	//	desc.Texture2D.MostDetailedMip = 0;
	//	desc.Texture2D.MipLevels = 1;

	//	hr = m_D3DDevice->CreateShaderResourceView(depthTexture, &desc, &srv);
	//	if (FAILED(hr))
	//	{
	//		FAILDE_ASSERT;
	//		return false;
	//	}
	//	ShaderResourceView[0].reset(srv);
	//}


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
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	m_D3DDevice->CreateRasterizerState(&rd, &m_RasterizerState);

	m_ImmediateContext->RSSetState(m_RasterizerState);



	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	for (char i = 0; i < 3; i++)
	{
		blendDesc.RenderTarget[i].BlendEnable = TRUE;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = nullptr;
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

	ID3D11SamplerState* samplerState = nullptr;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	m_ImmediateContext->PSSetSamplers(0, 1, &samplerState);

	{
		// サンプラーステート設定
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		ID3D11SamplerState* samplerState = nullptr;
		m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

		m_ImmediateContext->PSSetSamplers(1, 1, &samplerState);
	}

	m_SamplerState.reset(samplerState);

	return true;
}

bool CRenderer::Init2D()
{
	HRESULT hr = S_OK;

	// Direct2Dのファクトリーの作成
	ID2D1Factory1* d2dFactory = nullptr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), nullptr, reinterpret_cast<void**>(&d2dFactory));
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
	hr = m_D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_D2DDeviceContext);
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// DPIの取得
	float dx, dy;
	m_D2DDeviceContext->GetDpi(&dx, &dy);

	// レンダーターゲットの取得（DXGI）
	IDXGISurface* surf = nullptr;
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

	hr = m_D2DDeviceContext->CreateBitmapFromDxgiSurface(surf, &d2dProp, &m_D2DTargetBitmap);
	surf->Release();
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// 描画するDirect2Dビットマップの設定
	m_D2DDeviceContext->SetTarget(m_D2DTargetBitmap);

	// DirectWriteのファクトリの作成
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_DwriteFactory), reinterpret_cast<IUnknown**>(&m_DwriteFactory));
	if (FAILED(hr))
	{
		FAILDE_ASSERT;
		return false;
	}

	// テキストフォーマットの作成
	{
		TEXT_FOMAT fomat;
		fomat.FontName = "メイリオ";

		hr = Create_TextFormat(fomat);

		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
	}

	// テキストレイアウトを作成
	{
		TEXT_LAYOUT layout;
		layout.Text = "Hello HELL World!!!\n地球の未来にご奉仕するにゃん！";
		layout.Width = 400;
		layout.Height = 50;

		hr = Create_TextLayout(layout);

		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
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

	return true;
}

void CRenderer::SetBlendState(D3D11_BLEND_DESC* blend_state, bool flag)
{
	if (flag)
	{
		// ブレンドステート設定
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		ID3D11BlendState* blendState = nullptr;
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
		for (char i = 0; i < 3; i++)
		{
			blendDesc.RenderTarget[i].BlendEnable = TRUE;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		ID3D11BlendState* blendState = nullptr;
		m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
		m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
	}
}

// 加算合成用ブレンド ステートのためのを設定を取得する
void CRenderer::GetAddBlendState(D3D11_BLEND_DESC& blend_state)
{
	blend_state.AlphaToCoverageEnable = FALSE;
	blend_state.IndependentBlendEnable = FALSE;
	for (char i = 0; i < 3; i++)
	{
		blend_state.RenderTarget[i].BlendEnable = TRUE;
		blend_state.RenderTarget[i].SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_state.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		blend_state.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blend_state.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_state.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_state.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_state.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}

// 減算合成用ブレンド ステートのためのを設定を取得する
void CRenderer::GetSubtractBlendState(D3D11_BLEND_DESC& blend_state)
{
	blend_state.AlphaToCoverageEnable = FALSE;
	blend_state.IndependentBlendEnable = FALSE;
	for (char i = 0; i < 3; i++)
	{
		blend_state.RenderTarget[i].BlendEnable = TRUE;
		blend_state.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_state.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		blend_state.RenderTarget[i].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blend_state.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blend_state.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blend_state.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_state.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}

// 2D用のブレンド ステートのためのを設定を取得する
void CRenderer::Get2DBlendState(D3D11_BLEND_DESC& blend_state)
{
	blend_state.AlphaToCoverageEnable = FALSE;
	blend_state.IndependentBlendEnable = FALSE;
	for (char i = 0; i < 3; i++)
	{
		blend_state.RenderTarget[i].BlendEnable = TRUE;
		blend_state.RenderTarget[i].SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_state.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		blend_state.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blend_state.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_state.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_state.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_state.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}

void CRenderer::Change_Window_Mode()
{
	BOOL FullScreen;

	// GetFullscreenState
	m_SwapChain->GetFullscreenState(&FullScreen, nullptr);

	// SetFullscreenState
	m_SwapChain->SetFullscreenState(!FullScreen, nullptr);

	// 初期起動をフルスクリーンモードにした場合、ウィンドウモードに変更すると
	// ウィンドウがアクティブにならないので表示させる。
	ShowWindow(GetWindow(), SW_SHOW);
}

void CRenderer::Begin()
{
	//auto render_target = RenderTargetView_16bit.get();

	// バックバッファクリア
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	//m_ImmediateContext->OMSetRenderTargets(1, &render_target, m_DepthStencilView);//
	m_ImmediateContext->ClearRenderTargetView(m_RenderTargetView, ClearColor);
	//m_ImmediateContext->ClearRenderTargetView(RenderTargetView_16bit.get(), ClearColor);//
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


#include	"Sprite.h"
static unique_ptr<SPRITE> sprite;
static bool flag = true;

void CRenderer::End_Draw()
{
	if (flag)
	{
		flag = false;

		sprite = make_unique<SPRITE>();

		sprite.get()->SetPosition(XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
		sprite.get()->SetSize(XMFLOAT4(SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.5f));

		sprite.get()->Set(ShaderResourceView[0].get());
		//te.get()->Set(ShaderResourceView_16bit.get());

		sprite.get()->flag = false;

		sprite.get()->Init();
	}

	{
		float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
		m_ImmediateContext->ClearRenderTargetView(m_RenderTargetView, ClearColor);//
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		{
			// サンプラーステート設定
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.MaxAnisotropy = 4;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.MaxLOD = 0.0f;
			samplerDesc.MinLOD = 0.0f;

			ID3D11SamplerState* samplerState = nullptr;
			m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

			m_ImmediateContext->PSSetSamplers(1, 1, &samplerState);
		}
	}

		sprite.get()->Draw();
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
					CManager::Get_Instance()->GameEnd();
					break;

				// エラー 終了
				case DXGI_ERROR_DEVICE_REMOVED:
				case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
				case DXGI_ERROR_INVALID_CALL:
				default:
					CManager::Get_Instance()->GameEnd();
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

	pAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);
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
void CRenderer::SetWorldViewProjection2D(const XMFLOAT3& scaling, const XMFLOAT3& rotation)
{
	XMMATRIX world = XMMatrixIdentity();

	world = XMMatrixScaling(scaling.x, scaling.y, 1.0f);
	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));

	XMMATRIX view;
	view = XMMatrixIdentity();

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

	CONSTANT constant;
	constant.WorldMatrix = XMMatrixTranspose(world);
	constant.ViewMatrix = XMMatrixTranspose(view);
	constant.ProjectionMatrix = XMMatrixTranspose(projection);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &constant, 0, 0);
}

void CRenderer::Set_MatrixBuffer(const XMMATRIX world, const XMMATRIX view, const XMMATRIX projection)
{
	CONSTANT constant;
	constant.WorldMatrix = XMMatrixTranspose(world);
	constant.ViewMatrix = XMMatrixTranspose(view);
	constant.ProjectionMatrix = XMMatrixTranspose(projection);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &constant, 0, 0);
}

void CRenderer::SetMaterial( MATERIAL Material )
{
	m_ImmediateContext->UpdateSubresource( m_MaterialBuffer, 0, nullptr, &Material, 0, 0 );
}

void CRenderer::SetLight(LIGHT* Light)
{
	XMVECTOR vec = XMLoadFloat4(&Light->Direction);

	vec = XMVector4Normalize(vec);

	XMStoreFloat4(&Light->Direction, vec);

	m_ImmediateContext->UpdateSubresource(m_LightBuffer, 0, nullptr, Light, 0, 0);
}

void CRenderer::Light_Identity()
{
	// ライト初期化
	LIGHT light;
	light.Direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = COLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.Specular = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_ImmediateContext->UpdateSubresource(m_LightBuffer, 0, nullptr, &light, 0, 0);
}

void CRenderer::Set_RasterizerState()
{
	m_ImmediateContext->RSSetState(m_RasterizerState);
}

HRESULT CRenderer::Create_TextFormat(const TEXT_FOMAT& fomat)
{
	SAFE_RELEASE(m_DwriteTextFormat)

	wstring font_name = stringTowstring(fomat.FontName);

	// テキストフォーマットの作成
	HRESULT hr = m_DwriteFactory->CreateTextFormat(
		font_name.c_str(), nullptr,
		fomat.Weight, fomat.Style, fomat.Stretch,
		fomat.FontSize, L"", &m_DwriteTextFormat);

	return hr;
}

HRESULT CRenderer::Create_TextLayout(const TEXT_LAYOUT& layout)
{
	SAFE_RELEASE(m_TextLayout)

	// テキストレイアウトを作成
	wstring drawText;

	drawText = stringTowstring(layout.Text);

	HRESULT hr = m_DwriteFactory->CreateTextLayout(
		drawText.c_str(),
		drawText.size(),
		m_DwriteTextFormat,
		layout.Width,
		layout.Height,
		&m_TextLayout
	);

	return hr;
}

void CRenderer::SetVertexBuffers( ID3D11Buffer* VertexBuffer )
{
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { VertexBuffer };
	m_ImmediateContext->IASetVertexBuffers( 0, 1, vb, &stride, &offset );
}

void CRenderer::SetVertexBuffers(ID3D11Buffer* VertexBuffer, ID3D11Buffer* InstancingBuffer, UINT size)
{
	UINT stride[2] = { size, sizeof(XMMATRIX) };
	UINT offset[2] = { 0, 0 };
	ID3D11Buffer* vb[2] = { VertexBuffer, InstancingBuffer };
	m_ImmediateContext->IASetVertexBuffers(0, 2, vb, stride, offset);
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
	m_ImmediateContext->VSSetShader(m_VertexShader[v_index], nullptr, 0);

	m_ImmediateContext->PSSetShader(m_PixelShader[p_index], nullptr, 0);
}

void CRenderer::Set_InputLayout(const INPUTLAYOUT InputLayout)
{
	// 入力レイアウト設定
	switch ((int)InputLayout)
	{
		case (int)INPUTLAYOUT::DEFAULT:
			m_ImmediateContext->IASetInputLayout(m_VertexLayout[(int)INPUTLAYOUT::DEFAULT]);
			break;

		case (int)INPUTLAYOUT::ANIMATION:
			m_ImmediateContext->IASetInputLayout(m_VertexLayout[(int)INPUTLAYOUT::ANIMATION]);
			break;

		case (int)INPUTLAYOUT::INSTANCING:
			m_ImmediateContext->IASetInputLayout(m_VertexLayout[(int)INPUTLAYOUT::INSTANCING]);
			break;

		default:
			break;
	}
}

void CRenderer::CreateRenderTexture() {}

void CRenderer::SetPass_Rendring()
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	{
		// デフォルトのレンダーターゲットビューに切り替え
		auto render_target = RenderTargetView[0].get();
		m_ImmediateContext->OMSetRenderTargets(1, &render_target, m_DepthStencilView);
		m_ImmediateContext->ClearRenderTargetView(render_target, clearColor);

		//m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
		//m_ImmediateContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		{
			//auto render_target = RenderTargetView_16bit.get();

			//m_ImmediateContext->OMSetRenderTargets(1, &render_target, m_DepthStencilView);//
			//m_ImmediateContext->ClearRenderTargetView(RenderTargetView_16bit.get(), clearColor);//
			//m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		{
			// ビューポート設定
			D3D11_VIEWPORT dxViewport;
			dxViewport.Width = (float)SCREEN_WIDTH;
			dxViewport.Height = (float)SCREEN_HEIGHT;
			dxViewport.MinDepth = 0.0f;
			dxViewport.MaxDepth = 1.0f;
			dxViewport.TopLeftX = 0.0f;
			dxViewport.TopLeftY = 0.0f;

			m_ImmediateContext->RSSetViewports(1, &dxViewport);
		}

		Set_Shader();
		Set_RasterizerState();

		ID3D11SamplerState* pSS = m_SamplerState.get();
		m_ImmediateContext->PSSetSamplers(0, 1, &pSS);

		SetDepthEnable(true);
	}
}

void CRenderer::SetPass_Geometry()
{
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	{
		ID3D11RenderTargetView* rtv[3] = { RenderTargetView[0].get(), RenderTargetView[1].get(), RenderTargetView[2].get() };

		// デフォルトのレンダーターゲットビューに切り替え
		m_ImmediateContext->OMSetRenderTargets(3, rtv, m_DepthStencilView);
		m_ImmediateContext->ClearRenderTargetView(rtv[0], clearColor);
		m_ImmediateContext->ClearRenderTargetView(rtv[1], clearColor);
		m_ImmediateContext->ClearRenderTargetView(rtv[2], clearColor);
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		{
			// ビューポート設定
			D3D11_VIEWPORT dxViewport;
			dxViewport.Width = (float)SCREEN_WIDTH;
			dxViewport.Height = (float)SCREEN_HEIGHT;
			dxViewport.MinDepth = 0.0f;
			dxViewport.MaxDepth = 1.0f;
			dxViewport.TopLeftX = 0.0f;
			dxViewport.TopLeftY = 0.0f;

			D3D11_VIEWPORT viewport[3] = { dxViewport, dxViewport, dxViewport };

			m_ImmediateContext->RSSetViewports(3, viewport);
		}

		Set_Shader();
		Set_RasterizerState();
	}
}

bool CRenderer::Create()
{
	HRESULT hr;

	// デプスバッファの作成
	{
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = SCREEN_WIDTH;
		td.Height = SCREEN_HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = m_D3DDevice->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		// レンダーターゲットビュー設定
		{
			ID3D11RenderTargetView* pRtv;

			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = td.Format;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			hr = m_D3DDevice->CreateRenderTargetView(pTex, &desc, &pRtv);
			if (FAILED(hr))
			{
				return false;
			}

			RenderTargetView[0].reset(pRtv);
		}

		// シェーダーリソースビュー設定
		{
			ID3D11ShaderResourceView* srv = nullptr;

			// シェーダーリソースビューの設定
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = td.Format;
			desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.MipLevels = 1;

			hr = m_D3DDevice->CreateShaderResourceView(pTex, &desc, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
			ShaderResourceView[0].reset(srv);
		}
	}

	//// アルベドバッファの作成
	//{
	//	ID3D11Texture2D* pTex = nullptr;

	//	// テクスチャの作成
	//	D3D11_TEXTURE2D_DESC td;
	//	td.Width = SCREEN_WIDTH;
	//	td.Height = SCREEN_HEIGHT;
	//	td.MipLevels = 1;
	//	td.ArraySize = 1;
	//	td.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	//	td.SampleDesc.Count = 1;
	//	td.SampleDesc.Quality = 0;
	//	td.Usage = D3D11_USAGE_DEFAULT;
	//	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//	td.CPUAccessFlags = 0;
	//	td.MiscFlags = 0;

	//	hr = m_D3DDevice->CreateTexture2D(&td, nullptr, &pTex);
	//	if (FAILED(hr))
	//	{
	//		FAILDE_ASSERT;
	//		return false;
	//	}

	//	// レンダーターゲットビュー設定
	//	{
	//		ID3D11RenderTargetView* pRtv;

	//		hr = m_D3DDevice->CreateRenderTargetView(pTex, nullptr, &pRtv);
	//		if (FAILED(hr))
	//		{
	//			return false;
	//		}

	//		RenderTargetView[1].reset(pRtv);
	//	}

	//	// シェーダーリソースビュー設定
	//	{
	//		ID3D11ShaderResourceView* srv = nullptr;

	//		hr = m_D3DDevice->CreateShaderResourceView(pTex, nullptr, &srv);
	//		if (FAILED(hr))
	//		{
	//			FAILDE_ASSERT;
	//			return false;
	//		}
	//		ShaderResourceView[1].reset(srv);
	//	}
	//}

	//// 法線バッファの作成
	//{
	//	ID3D11Texture2D* pTex = nullptr;

	//	// テクスチャの作成
	//	D3D11_TEXTURE2D_DESC td;
	//	td.Width = SCREEN_WIDTH;
	//	td.Height = SCREEN_HEIGHT;
	//	td.MipLevels = 1;
	//	td.ArraySize = 1;
	//	td.Format = DXGI_FORMAT_R11G11B10_FLOAT;
	//	td.SampleDesc.Count = 1;
	//	td.SampleDesc.Quality = 0;
	//	td.Usage = D3D11_USAGE_DEFAULT;
	//	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//	td.CPUAccessFlags = 0;
	//	td.MiscFlags = 0;

	//	hr = m_D3DDevice->CreateTexture2D(&td, nullptr, &pTex);
	//	if (FAILED(hr))
	//	{
	//		FAILDE_ASSERT;
	//		return false;
	//	}

	//	// レンダーターゲットビュー設定
	//	{
	//		ID3D11RenderTargetView* pRtv;

	//		hr = m_D3DDevice->CreateRenderTargetView(pTex, nullptr, &pRtv);
	//		if (FAILED(hr))
	//		{
	//			return false;
	//		}

	//		RenderTargetView[2].reset(pRtv);
	//	}

	//	// シェーダーリソースビュー設定
	//	{
	//		ID3D11ShaderResourceView* srv = nullptr;

	//		hr = m_D3DDevice->CreateShaderResourceView(pTex, nullptr, &srv);
	//		if (FAILED(hr))
	//		{
	//			FAILDE_ASSERT;
	//			return false;
	//		}
	//		ShaderResourceView[2].reset(srv);
	//	}
	//}
}