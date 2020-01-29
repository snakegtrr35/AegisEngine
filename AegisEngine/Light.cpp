#include	"Light.h"
#include	"Renderer.h"

//bool LIGHTS::Init()
//{
//	return true;
//}
//
//void LIGHTS::Uninit()
//{
//	m_pPointLightBufferCenterAndRadius.reset(nullptr);
//	m_pPointLightBufferCenterAndRadiusSRV.reset(nullptr);
//	m_pPointLightBufferColor.reset(nullptr);
//	m_pPointLightBufferCenterAndRadius.reset(nullptr);
//
//	m_pSpotLightBufferCenterAndRadius.reset(nullptr);
//	m_pSpotLightBufferCenterAndRadiusSRV.reset(nullptr);
//	m_pSpotLightBufferColor.reset(nullptr);
//	m_pSpotLightBufferColorSRV.reset(nullptr);
//	m_pSpotLightBufferSpotParams.reset(nullptr);
//	m_pSpotLightBufferSpotParamsSRV.reset(nullptr);
//}

//array<LIGHT_BUFFER, MAX_NUM_LIGHTS> LIGHTS::Lights;
LIGHT_BUFFER LIGHTS::Lights;

LIGHT_BUFFER Light;

bool LIGHTS::Init()
{
	//Lights[0].Enable = 1.0;
	//Lights[0].Position = XMFLOAT3(0.0f, 2.0f, 0.0f);
	//Lights[0].Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	//Lights[0].Radius = 10.0f;
	//Lights[0].Type = (UINT)LIGHT_TYPE::POINT;

	Lights.Enable = 1;
	Lights.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Lights.Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	Lights.Radius = 5.0f;
	Lights.Attenuation = XMFLOAT3(1.0f, 0.f, 0.f);

	Light = Lights;

	// 定数バッファ生成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.ByteWidth = sizeof(LIGHT_BUFFER);

		D3D11_SUBRESOURCE_DATA date = {};
		date.pSysMem = &Lights;

		HRESULT hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, &date, &buffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		LightBuffer.reset(buffer);

		CRenderer::GetDeviceContext()->PSSetConstantBuffers(10, 1, &buffer);
	}

	return true;
}

void LIGHTS::Draw()
{
	auto buffer = LightBuffer.get();
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(10, 1, &buffer);
}

void LIGHTS::Update()
{
	/*LIGHT_BUFFER light;

	light.Enable = 1;
	light.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	light.Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	light.Radius = 3.0f;
	light.Attenuation = attenuation;*/

	CRenderer::GetDeviceContext()->UpdateSubresource(LightBuffer.get(), 0, nullptr, &Light, 0, 0);

	//// 頂点バッファの書き換え
	//{
	//	D3D11_MAPPED_SUBRESOURCE msr;
	//	CRenderer::GetDeviceContext()->Map(LightBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	//	LIGHT_BUFFER light;

	//	light.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	light.Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	//	light.Radius = 10.0f;
	//	light.Attenuation = XMFLOAT3(1.0f, 0.f, 0.2f);
	//	light.Enable = 1;

	//	memcpy(msr.pData, &light, sizeof(LIGHT_BUFFER));

	//	CRenderer::GetDeviceContext()->Unmap(LightBuffer.get(), 0);
	//}
}

void LIGHTS::Uninit()
{
	LightBuffer.reset(nullptr);
}