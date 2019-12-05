#include	"ShadowMap.h"

float SHADOW_MAP::WIDTH = 2048.0f;
float SHADOW_MAP::HEIGHT = 2048.0f;

SHADOW_MAP::SHADOW_MAP()
{
	LightPos = XMFLOAT3(10.0f, 10.0f, -10.0f);

	Enable = false;

	{
		// ビューマトリックス(光源から見る)
		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&LightPos), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 1.0f, 0.f, 0.f));
	}

	{
		// プロジェクションマトリックス
		//PlojectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f), WIDTH / HEIGHT, 1.0f, 100.0f);

		PlojectionMatrix = XMMatrixOrthographicLH(40.0f, 40.0f, 1.0f, 100.0f);
	}

	{
		// ビューポートの設定設定
		Viewport.left = 0;
		Viewport.top = 0;
		Viewport.right = WIDTH;
		Viewport.bottom = HEIGHT;

		// ビューポート設定
		DxViewport.Width = (float)(Viewport.right - Viewport.left);
		DxViewport.Height = (float)(Viewport.bottom - Viewport.top);
		DxViewport.MinDepth = 0.0f;
		DxViewport.MaxDepth = 1.0f;
		DxViewport.TopLeftX = (float)Viewport.left;
		DxViewport.TopLeftY = (float)Viewport.top;
	}
}

bool SHADOW_MAP::Init()
{
	HRESULT hr;

	// デプスステンシルビューの作成
	{
		ID3D11DepthStencilView* dsv = nullptr;
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = CRenderer::GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = 0;
		desc.Texture2D.MipSlice = 0;

		hr = CRenderer::GetDevice()->CreateDepthStencilView(pTex, &desc, &dsv);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		DepthStencilView.reset(dsv);
    }

	// シェーダリソースビューの作成
	{
		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td;
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = CRenderer::GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
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

			hr = CRenderer::GetDevice()->CreateRenderTargetView(pTex, &desc, &pRtv);
			if (FAILED(hr))
			{
				return false;
			}

			RenderTargetView.reset(pRtv);
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

			hr = CRenderer::GetDevice()->CreateShaderResourceView(pTex, &desc, &srv);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
			ShaderResourceView.reset(srv);
		}
	}

	// ラスタライズステートの設定
	{
		ID3D11RasterizerState* rs = nullptr;

		// ラスタライザステート設定
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_FRONT;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;

		CRenderer::GetDevice()->CreateRasterizerState(&rd, &rs);

		RasterizerState.reset(rs);
	}

	// サンプラーステートの生成.
	{
		ID3D11SamplerState* sampler = nullptr;

		D3D11_SAMPLER_DESC desc;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		desc.MaxAnisotropy = 1;
		desc.MipLODBias = 0;
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = 0.f;

		// サンプラーステートを生成.
		HRESULT hr = CRenderer::GetDevice()->CreateSamplerState(&desc, &sampler);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		Sampler.reset(sampler);

		CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &sampler);
	}

	// 定数バッファ作成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(CONSTANT_SHADOW_MAP);
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &buffer);

		ShadowBuffer.reset(buffer);
	}

	Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);
	Shadow.ProjectionMatrix = XMMatrixTranspose(PlojectionMatrix);

	// シャドウ生成パスの定数バッファを更新
	CRenderer::GetDeviceContext()->UpdateSubresource(ShadowBuffer.get(), 0, nullptr, &Shadow, 0, 0);

    return true;
}

void SHADOW_MAP::Begin()
{
	ID3D11DepthStencilView* pDSV = DepthStencilView.get();
	ID3D11RenderTargetView* pRTV = RenderTargetView.get();

    CRenderer::GetDeviceContext()->OMSetRenderTargets( 1, &pRTV, pDSV );

	CRenderer::GetDeviceContext()->RSSetViewports(1, &DxViewport);

	// バックバッファクリア
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	CRenderer::GetDeviceContext()->ClearRenderTargetView( pRTV, ClearColor );
    CRenderer::GetDeviceContext()->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	Enable = true;

	CRenderer::GetDeviceContext()->RSSetState(RasterizerState.get());
}

void SHADOW_MAP::End()
{
	Enable = false;

	ID3D11ShaderResourceView* srv = ShaderResourceView.get();

	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &srv);

	ID3D11SamplerState* s = Sampler.get();

	CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &s);
}

void SHADOW_MAP::Update()
{
}

void SHADOW_MAP::Uninit()
{
	RenderTargetView.reset(nullptr);
	DepthStencilView.reset(nullptr);
	ShaderResourceView.reset(nullptr);
	RasterizerState.reset(nullptr);
	Sampler.reset(nullptr);
	ShadowBuffer.reset(nullptr);
}

void SHADOW_MAP::Set_SamplerState()
{
	ID3D11SamplerState* samp = Sampler.get();

	CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &samp);
}

void SHADOW_MAP::Set_LightPos(const XMFLOAT3& pos)
{
	LightPos = pos;
}