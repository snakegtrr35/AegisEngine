#pragma once

#ifndef FORWARDLUS_H
#define FORWARDLUS_H

static constexpr UINT CLUSTERED_X = 32;
static constexpr UINT CLUSTERED_Y = 32;
static constexpr UINT CLUSTERED_Z = 16;

#include "Bounding_Frustum.h"
#include "Bounding_Aabb.h"

class CLUSTERED {
private:

	struct CLSTER_BUFFER {
		Aegis::Vector3 Scale;
		float dummy1;

		Aegis::Vector3 Bias;
		float dummy2;
	};

	struct CONSTATNT_BUFFER {
		Aegis::Vector3 Min;
		float dummy1;

		Aegis::Vector3 Scale;
		float dummy2;

		Aegis::Vector3 Bias;
		float dummy3;
	};

	using CLSTER = array<array<array<UINT, CLUSTERED_X>, CLUSTERED_Y>, CLUSTERED_Z>;
	using LIGHT_LIST = array<array<UINT, CLUSTERED_X* CLUSTERED_Y* CLUSTERED_Z>, 32>;

	ComPtr<ID3D11Buffer>				ConstatntBuffer;

	ComPtr<ID3D11ComputeShader>			ClusterCS;

	ComPtr<ID3D11Texture3D>				ClusterTexture;
	ComPtr<ID3D11UnorderedAccessView>	ClusterUAV;
	ComPtr<ID3D11ShaderResourceView>	ClusterSRV;

	ComPtr<ID3D11Texture2D>				Light_List_Texture;
	ComPtr<ID3D11UnorderedAccessView>	Light_List_UAV;
	ComPtr<ID3D11ShaderResourceView>	Light_List_SRV;

	ComPtr<ID3D11Buffer>				ClusterBuffer;


	unique_ptr<CLSTER>					Cluster;
	unique_ptr<LIGHT_LIST>				LightList;

	BOUNDING_FRUSTUM					m_Frustum;
	BOUNDING_AABB						m_Aabb;

	array<array<array<bool, CLUSTERED_X>, CLUSTERED_Y>, CLUSTERED_Z> m_ClusterFlags = {};

	Aegis::Vector3  m_Max = {}, m_Min = {};

	void Cale_Cluster(Aegis::Vector3& max, Aegis::Vector3& min);

public:

	CLUSTERED() : Cluster(make_unique<CLSTER>()), LightList(make_unique<LIGHT_LIST>()) {}
	~CLUSTERED() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();

	void Light_Culling();
};

#endif // !FORWARDLUS_H