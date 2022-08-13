#include "Light.h"

using namespace aegis;

aegis::array<LIGHT_BUFFER, MAX_NUM_LIGHTS> LIGHTS::Lights;
aegis::uniquePtr<aegis::Buffer>	LIGHTS::LightBuffer;


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
		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(LIGHT_BUFFER) * MAX_NUM_LIGHTS;
		bd.BindFlags = BindFlag::Constantbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};
		sd.pSysMem = Lights.data();

		LightBuffer.reset(render->CreateBuffer(bd, sd));
	}

	return true;
}

void LIGHTS::Draw()
{
	Buffer* buffer[] = { LightBuffer.get() };

	CRenderer::getInstance()->PSSetConstantBuffers(6, 1, buffer);
}

void LIGHTS::Update()
{
	CRenderer::getInstance()->UpdateSubresource(LightBuffer.get(),Lights.data());
}

void LIGHTS::Uninit()
{
}