#pragma once

#ifndef FORWARDLUS_H
#define FORWARDLUS_H

#include	"common.h"

//#include <wrl/client.h>
//using namespace Microsoft::WRL;

class SCENE_MANAGER;

static const constexpr WORD TILE_RES = 16;
static const constexpr WORD MAX_NUM_LIGHTS_PER_TILE = 272;

struct DepthStencilBuffer
{
	ID3D11Texture2D*				m_pDepthStencilTexture;
	ID3D11DepthStencilView*			m_pDepthStencilView;
	ID3D11ShaderResourceView*		m_pDepthStencilSRV;

	~DepthStencilBuffer() {
		SAFE_RELEASE(m_pDepthStencilTexture);
		SAFE_RELEASE(m_pDepthStencilView);
		SAFE_RELEASE(m_pDepthStencilSRV);
	}
};

class FORWARDLUS {
private:
	//! compute shader for per - tile depth bounds
	unique_ptr <ID3D11ComputeShader, Release>			m_pDepthBoundsCS;

	//! compute shader for tiled culling
	unique_ptr <ID3D11ComputeShader, Release>			m_pLightCullCS;

	unique_ptr <ID3D11DepthStencilView, Release>		m_pDepthStencilView;

	unique_ptr <ID3D11DepthStencilState, Release>		m_DepthStateEnable;

	unique_ptr <ID3D11VertexShader, Release>			m_VertexShader;

	unique_ptr <ID3D11DepthStencilState, Release>		m_pDepthStencilState;

	DepthStencilBuffer									m_DepthStencilBuffer;

	// texture to hold per-tile depth bounds
	unique_ptr <ID3D11Texture2D, Release>				m_pDepthBoundsTexture;
	unique_ptr <ID3D11ShaderResourceView, Release>		m_pDepthBoundsSRV;
	unique_ptr <ID3D11UnorderedAccessView, Release>		m_pDepthBoundsUAV;

	// buffers for light culling
	unique_ptr <ID3D11Buffer, Release>					m_pLightIndexBuffer;
	unique_ptr <ID3D11ShaderResourceView, Release>		m_pLightIndexBufferSRV;
	unique_ptr <ID3D11UnorderedAccessView, Release>		m_pLightIndexBufferUAV;

	// buffers for spot light culling
	unique_ptr <ID3D11Buffer, Release>					m_pSpotIndexBuffer;
	unique_ptr <ID3D11ShaderResourceView, Release>		m_pSpotIndexBufferSRV;
	unique_ptr <ID3D11UnorderedAccessView, Release>		m_pSpotIndexBufferUAV;

	bool CreateDepthStencilSurface(ID3D11Texture2D** ppDepthStencilTexture, ID3D11ShaderResourceView** ppDepthStencilSRV,
		ID3D11DepthStencilView** ppDepthStencilView, DXGI_FORMAT DSFormat, DXGI_FORMAT SRVFormat,
		unsigned int uWidth, unsigned int uHeight, unsigned int uSampleCount);

	bool CreateSurface(ID3D11Texture2D** ppTexture, ID3D11ShaderResourceView** ppTextureSRV,
		ID3D11RenderTargetView** ppTextureRTV, ID3D11UnorderedAccessView** ppTextureUAV,
		DXGI_FORMAT Format, unsigned int uWidth, unsigned int uHeight, unsigned int uSampleCount);

	WORD GetNumTilesX();
	WORD GetNumTilesY();
	WORD GetMaxNumElementsPerTile();
	WORD GetMaxNumLightsPerTile();

public:

	FORWARDLUS() {}
	~FORWARDLUS() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();

	void Depth_Pre_Pass(SCENE_MANAGER* scene_manager);
	void Light_Culling();
};

#endif // !FORWARDLUS_H