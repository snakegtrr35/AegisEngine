#pragma once

#ifndef FORWARDLUS_H
#define FORWARDLUS_H

static constexpr UINT CLUSTERED_X = 32;
static constexpr UINT CLUSTERED_Y = 32;
static constexpr UINT CLUSTERED_Z = 16;

class CLUSTERED {
private:
	//! compute shader for per - tile depth bounds
	unique_ptr <ID3D11ComputeShader, Release>			m_LightCullCS;

	unique_ptr <ID3D11Texture3D, Release>				ClusteredTexture;
	unique_ptr <ID3D11ShaderResourceView, Release>		ClusteredSRV;

public:

	CLUSTERED() {}
	~CLUSTERED() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();

	void Light_Culling();
};

#endif // !FORWARDLUS_H