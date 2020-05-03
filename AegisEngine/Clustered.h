#pragma once

#ifndef FORWARDLUS_H
#define FORWARDLUS_H

static constexpr UINT CLUSTERED_X = 32;
static constexpr UINT CLUSTERED_Y = 32;
static constexpr UINT CLUSTERED_Z = 16;

class CLUSTERED {
private:

	struct CLSTER_BUFFER {
		XMFLOAT3 Scale;
		float dummy1;

		XMFLOAT3 Bias;
		float dummy2;

		CLSTER_BUFFER(const XMFLOAT3& s, const XMFLOAT3& b) : Scale(s), Bias(b), dummy1(0.f), dummy2(0.f) {}

	};

	using CLSTER = array<array<array<XMINT2, CLUSTERED_X>, CLUSTERED_Y>, CLUSTERED_Z>;

	//! compute shader for per - tile depth bounds
	unique_ptr <ID3D11ComputeShader, Release>			m_LightCullCS;

	unique_ptr <ID3D11Texture3D, Release>				ClusteredTexture;
	unique_ptr <ID3D11ShaderResourceView, Release>		ClusteredSRV;

	unique_ptr <ID3D11Texture2D, Release>				Light_List_Texture;
	unique_ptr <ID3D11ShaderResourceView, Release>		Light_List_SRV;

	unique_ptr<ID3D11Buffer, Release> ClusterBuffer;

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