#include "ShadowMap.h"

#include "manager.h"
#include "Scene.h"
#include "Player.h"
#include "Renderer.h"


using namespace aegis;

UINT SHADOW_MAP::WIDTH = 2048;
UINT SHADOW_MAP::HEIGHT = 2048;

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
		Texture2D* pTex = nullptr;

		// テクスチャの作成
		Texture2DDesc td{};
		td.Width = WIDTH;
		td.Height = HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = Format::R32Typeless;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = Usage::Default;
		td.BindFlags = (BindFlag)((uint32)BindFlag::DepthStencil | (uint32)BindFlag::ShaderResource);
		td.CPUAccessFlags = CpuAccessFlag::None;
		td.MiscFlags = 0;

		pTex = render->CreateTexture2D(td, nullptr);

		{
			DepthStencilViewDesc desc{};
			desc.Format = Format::D32Float;
			desc.ViewDimension = DsvDimension::Texture2D;
			desc.Flags = 0;

			DepthStencilView.reset(render->CreateDepthStencilView(pTex, desc));
		}

		// シェーダーリソースビュー設定
		{
			ShaderResourceViewDesc desc{};
			desc.Format = Format::R32Float;
			desc.ViewDimension = SrvDimension::Texture2D;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;

			ShaderResourceView.reset(render->CreateShaderResourceView(pTex, desc));
		}
    }

	// ラスタライザステート設定
	RasterizeState rd{};
	rd.FillMode = FillMode::Solid;
	rd.CullMode = CullMode::Back;
	rd.DepthClipEnable = true;
	rd.MultisampleEnable = false;

	RasterizerState.reset(render->CreateRasterizeState(rd));

	// サンプラーステートの生成
	{
		SamplerDesc desc{};
		desc.Address = AddressMode::Border;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.ComparisonFunc = ComparisonFunc::Always;
		desc.Filter = Filter::Anisotropic;
		desc.MaxAnisotropy = 16;
		desc.MaxLOD = Math::Float32Max;

		// サンプラーステートを生成
		Sampler.reset(render->CreateSampler(desc));
	}

	// 定数バッファ作成
	{
		BufferDesc  hBufferDesc{};
		hBufferDesc.Usage = Usage::Default;
		hBufferDesc.ByteWidth = sizeof(CONSTANT_SHADOW_MAP);
		hBufferDesc.BindFlags = BindFlag::Constantbuffer;
		hBufferDesc.CPUAccessFlags = CpuAccessFlag::None;
		hBufferDesc.StructureByteStride = sizeof(float);

		SubresourceData sd{};
		{
			ShadowBuffer.reset(render->CreateBuffer(hBufferDesc, sd));

			Buffer* buffers[] = { ShadowBuffer .get() };

			render->VSSetConstantBuffers(1, 1, buffers);
		}

		{
			hBufferDesc.ByteWidth = sizeof(CONSTANT_LIGHT);

			LightBuffer.reset(render->CreateBuffer(hBufferDesc, sd));

			Buffer* buffers[] = { LightBuffer.get() };

			render->PSSetConstantBuffers(0, 1, buffers);
		}
	}

	Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);
	Shadow.ProjectionMatrix = XMMatrixTranspose(PlojectionMatrix);

	// シャドウ生成パスの定数バッファを更新
	render->UpdateSubresource(ShadowBuffer.get(), &Shadow);

    return true;
}

void SHADOW_MAP::Begin()
{
	CRenderer* render = render->getInstance();
	render->SetRenderTargets(0, nullptr, DepthStencilView.get());

	// バックバッファクリア
    render->ClearDepthStencilView(DepthStencilView.get(), ClearFlag::Depth, 1.0f, 0 );


	aegis::ViewPort* viewPorts[] = { &DxViewport };
	render->SetViewports(1, viewPorts);

	render->SetRasterizeState(RasterizerState.get());

	Set_Light(LightPos);

	Enable = true;
}

void SHADOW_MAP::End()
{
	Enable = false;

	aegis::RenderTargetView* pRTV = nullptr;

	CRenderer::getInstance()->SetRenderTargets(1, &pRTV, nullptr);

	//render->PSSetShaderResources(1, 1, ShaderResourceView.GetAddressOf());
}

void SHADOW_MAP::Update()
{
	CRenderer* render = render->getInstance();
	LIGHT* light = render->GetLight();

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
		Vector3 at = Target.lock()->Get_Transform().Get_Position();

		Vector3 pos = at + LightPos;

		ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMVectorSet(at.x, at.y, at.z, 0.f), XMVectorSet(0.f, 1.0f, 0.f, 0.f));

		Shadow.ViewMatrix = XMMatrixTranspose(ViewMatrix);

		// シャドウ生成パスの定数バッファを更新
		render->UpdateSubresource(ShadowBuffer.get(),&Shadow);
	}
}

void SHADOW_MAP::Uninit()
{
}

void SHADOW_MAP::Set_SamplerState()
{
	SamplerState* samplerStates[] = { Sampler.get() };

	CRenderer::getInstance()->PSSetSamplers(1, 1, samplerStates);
}

void SHADOW_MAP::Set_LightPos(const Vector3& pos)
{
	LightPos = pos;
}

void SHADOW_MAP::Set_Target(const std::weak_ptr<GameObject>& object)
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

	CRenderer::getInstance()->UpdateSubresource(LightBuffer.get(), &Light);
}

void SHADOW_MAP::Set()
{
	CRenderer* render = render->getInstance();

	aegis::ShaderResourceView* shaderResourceViews[] = { ShaderResourceView.get() };

	render->PSSetShaderResources(1, 1, shaderResourceViews);

	SamplerState* samplerStates[] = { Sampler.get() };

	render->PSSetSamplers(1, 1, samplerStates);
}