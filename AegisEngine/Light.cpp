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

bool LIGHTS::Init()
{
	return true;
}

void LIGHTS::Uninit()
{
	m_pPointLightBuffer.reset(nullptr);
	m_pSpotLightBuffer.reset(nullptr);
}