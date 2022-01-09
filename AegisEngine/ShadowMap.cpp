#include	"ShadowMap.h"
#include	"GameObject.h"

#include	"manager.h"
#include	"Scene.h"
#include	"Player.h"

using namespace Aegis;

UINT SHADOW_MAP::WIDTH = 2048;
UINT SHADOW_MAP::HEIGHT = 2048;

static ID3D11DepthStencilState* m_DepthStateEnable;


SHADOW_MAP::SHADOW_MAP()
{
	LightPos = Vector3(15.0f, 15.0f, -15.0f);

	Enable = false;

	{
		// ビューマトリックス(光源から見る)
		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&LightPos), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 1.0f, 0.f, 0.f));
	}

	{
		// プロジェクションマトリックス
		//PlojectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f), WIDTH / HEIGHT, 1.0f, 100.0f);

		PlojectionMatrix = XMMatrixOrthographicLH(60.0f, 60.0f, 0.1f, 100.0f);
	}

	{
		// ビューポートの設定設定
		Viewport.left = 0;
		Viewport.top = 0;
		Viewport.right = (LONG)WIDTH;
		Viewport.bottom = (LONG)HEIGHT;

		// ビューポート設定
		DxViewport.Width = (float)(Viewport.right - Viewport.left);
		DxViewport.Height = (float)(Viewport.bottom - Viewport.top);
		DxViewport.MinDepth = 0.0f;
		DxViewport.MaxDepth = 1.0f;
		DxViewport.TopLeftX = 0;
		DxViewport.TopLeftY = 0;
	}

	Target.reset();
}

bool SHADOW_MAP::Init()
{
	CRenderer* render = CRenderer::getInstance();
	HRESULT hr;

	// デプスステンシルビューの作成
	{

		ID3D11Texture2D* pTex = nullptr;

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		hr = render->GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.Format = DXGI_FORMAT_D32_FLOAT;
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = 0;

			hr = render->GetDevice()->CreateDepthStencilView(pTex, &desc, &DepthStencilView);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		// シェーダーリソースビュー設定
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			ZeroMemory(&desc, sizeof(desc));
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;

			hr = render->GetDevice()->CreateShaderResourceView(pTex, &desc, &ShaderResourceView);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
    }

	//// デプスバッファの作成
	//{
	//	ID3D11Texture2D* pTex = nullptr;

	//	// テクスチャの作成
	//	D3D11_TEXTURE2D_DESC td = {};
	//	td.Width = WIDTH;
	//	td.Height = HEIGHT;
	//	td.MipLevels = 1;
	//	td.ArraySize = 1;
	//	td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//	td.SampleDesc.Count = 1;
	//	td.SampleDesc.Quality = 0;
	//	td.Usage = D3D11_USAGE_DEFAULT;
	//	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//	td.CPUAccessFlags = 0;
	//	td.MiscFlags = 0;

	//	hr = render->GetDevice()->CreateTexture2D(&td, nullptr, &pTex);
	//	if (FAILED(hr))
	//	{
	//		FAILDE_ASSERT;
	//		return false;
	//	}

	//	// レンダーターゲットビュー設定
	//	{
	//		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
	//		desc.Format = td.Format;
	//		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//		desc.Texture2D.MipSlice = 0;

	//		hr = render->GetDevice()->CreateRenderTargetView(pTex, &desc, &RenderTargetView);
	//		if (FAILED(hr))
	//		{
	//			return false;
	//		}
	//	}

	//	// シェーダーリソースビュー設定
	//	{
	//		// シェーダーリソースビューの設定
	//		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	//		desc.Format = td.Format;
	//		desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	//		desc.Texture2D.MostDetailedMip = 0;
	//		desc.Texture2D.MipLevels = 1;

	//		hr = render->GetDevice()->CreateShaderResourceView(pTex, &desc, &ShaderResourceView);
	//		if (FAILED(hr))
	//		{
	//			FAILDE_ASSERT;
	//			return false;
	//		}
	//	}
	//}

	// ラスタライズステートの設定
	{
		// ラスタライザステート設定
		D3D11_RASTERIZER_DESC rd = {};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		//rd.CullMode = D3D11_CULL_FRONT;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;

		render->GetDevice()->CreateRasterizerState(&rd, &RasterizerState);
	}

	// サンプラーステートの生成
	{
		D3D11_SAMPLER_DESC desc = {};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.MaxAnisotropy = 16;
		desc.MipLODBias = 0;
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = 0.f;

		// サンプラーステートを生成
		HRESULT hr = render->GetDevice()->CreateSamplerState(&desc, &Sampler);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		render->GetDeviceContext()->PSSetSamplers(1, 1, Sampler.GetAddressOf());
	}

	// 定数バッファ作成
	{
		D3D11_BUFFER_DESC hBufferDesc = {};
		hBufferDesc.ByteWidth = sizeof(CONSTANT_SHADOW_MAP);
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		{
			hr = render->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &ShadowBuffer);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			render->GetDeviceContext()->VSSetConstantBuffers(1, 1, ShadowBuffer.GetAddressOf());
		}

		{
			hBufferDesc.ByteWidth = sizeof(CONSTANT_LIGHT);

			hr = render->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &LightBuffer);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			render->GetDeviceContext()->PSSetConstantBuffers(0, 1, LightBuffer.GetAddressOf());
		}
	}

	Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);
	Shadow.ProjectionMatrix = XMMatrixTranspose(PlojectionMatrix);

	// シャドウ生成パスの定数バッファを更新
	render->GetDeviceContext()->UpdateSubresource(ShadowBuffer.Get(), 0, nullptr, &Shadow, 0, 0);


	{
		// 深度ステンシルステート設定
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = FALSE;

		render->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);
	}

    return true;
}

void SHADOW_MAP::Begin()
{
	CRenderer* render = render->getInstance();
	ID3D11RenderTargetView* pRTV = RenderTargetView.Get();

    //render->GetDeviceContext()->OMSetRenderTargets(1, &pRTV, DepthStencilView.Get());
	render->GetDeviceContext()->OMSetRenderTargets(0, nullptr, DepthStencilView.Get());

	// バックバッファクリア
	float ClearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//render->GetDeviceContext()->ClearRenderTargetView(pRTV, ClearColor);
    render->GetDeviceContext()->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0 );


	render->GetDeviceContext()->RSSetViewports(1, &DxViewport);

	render->GetDeviceContext()->RSSetState(RasterizerState.Get());

	//render->GetDeviceContext()->OMSetDepthStencilState(m_DepthStateEnable, 0x00);


	Set_Light(LightPos);

	Enable = true;
}

void SHADOW_MAP::End()
{
	Enable = false;

	ID3D11RenderTargetView* pRTV = nullptr;

	CRenderer* render = render->getInstance();
	render->getInstance()->GetDeviceContext()->OMSetRenderTargets(1, &pRTV, nullptr);

	//render->GetDeviceContext()->PSSetShaderResources(1, 1, ShaderResourceView.GetAddressOf());
}

void SHADOW_MAP::Update()
{
	CRenderer* render = render->getInstance();
	LIGHT* light = render->Get_Light();

	LightPos.x = light->Position.x;
	LightPos.y = light->Position.y;
	LightPos.z = light->Position.z;

	if (Target.expired())
	{
		auto scene = CManager::Get_Instance()->Get_Scene();

		Target = scene->Get_Game_Object<PLAYER>("player");
	}

	if (!Target.expired())
	{
		Vector3* at = Target.lock()->Get_Transform().Get_Position();

		Vector3 pos;
		pos.x = at->x + LightPos.x;
		pos.y = at->y + LightPos.y;
		pos.z = at->z + LightPos.z;

		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMVectorSet(at->x, at->y, at->z, 0.f), XMVectorSet(0.f, 1.0f, 0.f, 0.f));

		Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);

		// シャドウ生成パスの定数バッファを更新
		render->GetDeviceContext()->UpdateSubresource(ShadowBuffer.Get(), 0, nullptr, &Shadow, 0, 0);
	}
}

void SHADOW_MAP::Uninit()
{
}

void SHADOW_MAP::Set_SamplerState()
{
	CRenderer::getInstance()->GetDeviceContext()->PSSetSamplers(1, 1, Sampler.GetAddressOf());
}

void SHADOW_MAP::Set_LightPos(const Vector3& pos)
{
	LightPos = pos;
}

void SHADOW_MAP::Set_Target(const weak_ptr<GameObject>& object)
{
	Target = object;
}

const bool SHADOW_MAP::Target_Enable()
{
	return !Target.expired();
}

void SHADOW_MAP::Set_Light(const Vector3& pos)
{
	Light.Direction.x = -pos.x;
	Light.Direction.y = -pos.y;
	Light.Direction.z = -pos.z;
	Light.Direction.w = 0.f;

	CRenderer::getInstance()->GetDeviceContext()->UpdateSubresource(LightBuffer.Get(), 0, nullptr, &Light, 0, 0);
}

void SHADOW_MAP::Set()
{
	CRenderer* render = render->getInstance();

	render->GetDeviceContext()->PSSetShaderResources(1, 1, ShaderResourceView.GetAddressOf());

	render->GetDeviceContext()->PSSetSamplers(1, 1, Sampler.GetAddressOf());
}