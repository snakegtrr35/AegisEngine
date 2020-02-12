#include	"Light.h"
#include	"Renderer.h"

array<LIGHT_BUFFER, MAX_NUM_LIGHTS> LIGHTS::Lights;


LIGHT_BUFFER::LIGHT_BUFFER() : Enable(0), Position(0.f, 0.f, 0.f), Color(0.f, 0.f, 0.f, 0.f), Radius(0.f), Attenuation(1.0f, 0.1f, 0.2f), Type((UINT)LIGHT_TYPE::NONE) {}

LIGHTS::LIGHTS()
{
	for (UINT i = 0; i < MAX_NUM_LIGHTS; i++)
	{
		Lights[i] = LIGHT_BUFFER();
	}

	Lights[0].Enable = 1;
	Lights[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Lights[0].Color = COLOR(1.0f, 0.f, 0.f, 1.0f);
	Lights[0].Radius = 3.0f;
	Lights[0].Type = (UINT)LIGHT_TYPE::POINT;

	Lights[1].Enable = 1;
	Lights[1].Position = XMFLOAT3(2.0f, 0.0f, 0.0f);
	Lights[1].Color = COLOR(0.0f, 0.f, 1.0f, 1.0f);
	Lights[1].Radius = 4.0f;
	Lights[1].Type = (UINT)LIGHT_TYPE::POINT;
}

bool LIGHTS::Init()
{
	// 定数バッファ生成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.ByteWidth = sizeof(LIGHT_BUFFER) * MAX_NUM_LIGHTS;

		D3D11_SUBRESOURCE_DATA date = {};
		date.pSysMem = Lights.data();

		HRESULT hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, &date, &buffer);
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

void LIGHTS::Draw()
{
	auto buffer = LightBuffer.get();
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(6, 1, &buffer);
}

void LIGHTS::Update()
{
	CRenderer::GetDeviceContext()->UpdateSubresource(LightBuffer.get(), 0, nullptr, Lights.data(), 0, 0);
}

void LIGHTS::Uninit()
{
	LightBuffer.reset(nullptr);
}