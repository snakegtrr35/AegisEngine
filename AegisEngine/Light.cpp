#include	"Light.h"
#include	"Renderer.h"

using namespace aegis;

aegis::array<LIGHT_BUFFER, MAX_NUM_LIGHTS> LIGHTS::Lights;
ComPtr<ID3D11Buffer>	LIGHTS::LightBuffer;


LIGHT_BUFFER::LIGHT_BUFFER() : Enable(0), Position(0.f, 0.f, 0.f), Color(0.f, 0.f, 0.f, 0.f), Type((UINT)LIGHT_TYPE::NONE) {}

using namespace aegis;

LIGHTS::LIGHTS()
{
	for (UINT i = 0; i < MAX_NUM_LIGHTS; i++)
	{
		Lights[i] = LIGHT_BUFFER();
	}

	Lights[0].Enable = 1;
	Lights[0].Position = Vector3(0.0f, 0.0f, 0.0f);
	Lights[0].Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	Lights[0].Type = (UINT)LIGHT_TYPE::POINT;

	Lights[1].Enable = 1;
	Lights[1].Position = Vector3(2.0f, 0.0f, 0.0f);
	Lights[1].Color = COLOR(0.0f, 0.f, 1.0f, 1.0f);
	Lights[1].Type = (UINT)LIGHT_TYPE::POINT;
}

bool LIGHTS::Init()
{
	CRenderer* render = CRenderer::getInstance();

	// 定数バッファ生成
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.ByteWidth = sizeof(LIGHT_BUFFER) * MAX_NUM_LIGHTS;

		D3D11_SUBRESOURCE_DATA date = {};
		date.pSysMem = Lights.data();

		HRESULT hr = render->GetDevice()->CreateBuffer(&hBufferDesc, &date, &LightBuffer);
		if (FAILED(hr))
		{
			FAILDE_ASSERT;
			return false;
		}

		//render->GetDeviceContext()->PSSetConstantBuffers(6, 1, LightBuffer.GetAddressOf());
	}

	return true;
}

void LIGHTS::Draw()
{
	CRenderer::getInstance()->GetDeviceContext()->PSSetConstantBuffers(6, 1, LightBuffer.GetAddressOf());
}

void LIGHTS::Update()
{
	CRenderer::getInstance()->GetDeviceContext()->UpdateSubresource(LightBuffer.Get(), 0, nullptr, Lights.data(), 0, 0);
}

void LIGHTS::Uninit()
{
}