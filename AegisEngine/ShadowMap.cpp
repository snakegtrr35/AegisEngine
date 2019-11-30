#include	"ShadowMap.h"


SHADOW_MAP::SHADOW_MAP()
{
	LightPos = XMFLOAT3(10.0f, 10.0f, -10.0f);

	Enable = true;

	{
		// ビューマトリックス(光源から見る)
		XMVECTOR at = { 0.0f, 0.0f,  0.0f };  // 注視点
		XMVECTOR up = { 0.0f, 1.0f,  0.0f };  // カメラの上方向

		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&LightPos), at, up);
	}

	{
		// プロジェクションマトリックス
		PlojectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	}
}

bool SHADOW_MAP::Init(const float width, const float height )
{
	HRESULT hr;

	// テクスチャの作成
	ID3D11Texture2D* pTex = nullptr;
	{
		D3D11_TEXTURE2D_DESC td;
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = CRenderer::GetDevice()->CreateTexture2D(&td, NULL, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}
	}

	// デプスステンシルビューの作成
    {
		ID3D11DepthStencilView* dsv = nullptr;

		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_D32_FLOAT;
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
		ID3D11ShaderResourceView* srv = nullptr;

		// レンダーターゲットビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = -1;

		hr = CRenderer::GetDevice()->CreateShaderResourceView(pTex, &desc, &srv);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		ShaderResourceView.reset(srv);
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
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.MaxAnisotropy = 1;
		desc.MipLODBias = 0;
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = -FLT_MAX;

		// サンプラーステートを生成.
		HRESULT hr = CRenderer::GetDevice()->CreateSamplerState(&desc, &sampler);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		Sampler.reset(sampler);
	}

	// 定数バッファ作成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(CONSTANT);
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

    return true;
}

void SHADOW_MAP::Begin()
{
	ID3D11DepthStencilView* pDSV = DepthStencilView.get();
    CRenderer::GetDeviceContext()->OMSetRenderTargets( 0, nullptr, pDSV );
    CRenderer::GetDeviceContext()->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );



	Enable = true;
}

void SHADOW_MAP::End()
{
	Enable = false;
}

void SHADOW_MAP::Update()
{
	{
		XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&LightPos), at, up);
	}

	Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);
	Shadow.ProjectionMatrix = XMMatrixTranspose(PlojectionMatrix);
}

void SHADOW_MAP::Uninit()
{
	DepthStencilView.reset(nullptr);
	ShaderResourceView.reset(nullptr);
	Sampler.reset(nullptr);

	SAFE_RELEASE(ShadowBuffer);
}

void SHADOW_MAP::Set_ShaderResourceView()
{
	ID3D11ShaderResourceView* srv = ShaderResourceView.get();

	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &srv);
}

void SHADOW_MAP::Set_SamplerState()
{
	ID3D11SamplerState* samp = Sampler.get();

	CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &samp);
}

void SHADOW_MAP::Set_Buffer(const XMMATRIX& world)
{
	Shadow.WorldMatrix = XMMatrixTranspose(world);

	// シャドウ生成パスの定数バッファを更新
	CRenderer::GetDeviceContext()->UpdateSubresource(ShadowBuffer.get(), 0, nullptr, &Shadow, 0, 0);
}

void SHADOW_MAP::Set_LightPos(const XMFLOAT3& pos)
{
	LightPos = pos;
}