#pragma once

#ifndef LIGHT_H
#define LIGHT_H

//static const constexpr UINT MAX_NUM_LIGHTS = 1024;
////static const constexpr UINT MAX_NUM_GRID_OBJECTS = 280;
//
//class LIGHTS {
//private:
//
//	// point lights
//	unique_ptr <ID3D11Buffer, Release>					m_pPointLightBufferCenterAndRadius;
//	unique_ptr <ID3D11ShaderResourceView, Release>		m_pPointLightBufferCenterAndRadiusSRV;
//	unique_ptr <ID3D11Buffer, Release>					m_pPointLightBufferColor;
//	unique_ptr <ID3D11ShaderResourceView, Release>		m_pPointLightBufferColorSRV;
//
//	// spot lights
//	unique_ptr <ID3D11Buffer, Release>					m_pSpotLightBufferCenterAndRadius;
//	unique_ptr <ID3D11ShaderResourceView, Release>		m_pSpotLightBufferCenterAndRadiusSRV;
//	unique_ptr <ID3D11Buffer, Release>					m_pSpotLightBufferColor;
//	unique_ptr <ID3D11ShaderResourceView, Release>		m_pSpotLightBufferColorSRV;
//	unique_ptr <ID3D11Buffer, Release>					m_pSpotLightBufferSpotParams;
//	unique_ptr <ID3D11ShaderResourceView, Release>		m_pSpotLightBufferSpotParamsSRV;
//
//public:
//	LIGHTS() {}
//	~LIGHTS() { Uninit(); }
//
//	bool Init();
//	//void Update();
//	void Uninit();
//
//	ID3D11ShaderResourceView* const GetPointLightBufferCenterAndRadiusSRVParam() const { return m_pPointLightBufferCenterAndRadiusSRV.get(); }
//	ID3D11ShaderResourceView* const GetPointLightBufferColorSRVParam() const { return m_pPointLightBufferColorSRV.get(); }
//
//	ID3D11ShaderResourceView* const GetSpotLightBufferCenterAndRadiusSRVParam() const { return m_pSpotLightBufferCenterAndRadiusSRV.get(); }
//	ID3D11ShaderResourceView* const GetSpotLightBufferColorSRVParam() const { return m_pSpotLightBufferColorSRV.get(); }
//	ID3D11ShaderResourceView* const GetSpotLightBufferSpotParamsSRVParam() const { return m_pSpotLightBufferSpotParamsSRV.get(); }
//	//ID3D11ShaderResourceView* const GetSpotLightBufferSpotMatricesSRVParam() const { return m_pSpotLightBufferSpotMatricesSRV.get(); }
//};


static const constexpr UINT MAX_NUM_LIGHTS = 64;

class LIGHTS {
private:

	// point lights
	unique_ptr <ID3D11Buffer, Release>			m_pPointLightBuffer;

	// spot lights
	unique_ptr <ID3D11Buffer, Release>			m_pSpotLightBuffer;

public:
	LIGHTS() {}
	~LIGHTS() { Uninit(); }

	bool Init();
	void Update();
	void Uninit();

};

#endif // !LIGHT_H