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

bool LIGHTS::Init()
{
	//Lights[0].Position = XMFLOAT3(0.0f, 2.0f, 0.0f);
	//Lights[0].Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	//Lights[0].Radius = 10.0f;
	//Lights[0].Enable = 1.0;
	//Lights[0].Type = (UINT)LIGHT_TYPE::POINT;

	Lights.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Lights.Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	Lights.Radius = 10.0f;
	Lights.Enable = 1;

	// 定数バッファ生成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);
		hBufferDesc.ByteWidth = sizeof(LIGHT_BUFFER);

		HRESULT hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, nullptr, &buffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		LightBuffer.reset(buffer);

		CRenderer::GetDeviceContext()->PSSetConstantBuffers(6, 1, &buffer);
	}

	return true;
}

void LIGHTS::Update()
{
	CRenderer::GetDeviceContext()->UpdateSubresource(LightBuffer.get(), 0, nullptr, &Lights, 0, 0);

	auto buffer = LightBuffer.get();
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(6, 1, &buffer);
}

void LIGHTS::Uninit()
{
	LightBuffer.reset(nullptr);
}