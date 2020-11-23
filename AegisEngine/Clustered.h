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
		XMFLOAT3 Scale;
		float dummy1;

		XMFLOAT3 Bias;
		float dummy2;
	};

	struct CONSTATNT_BUFFER {
		XMFLOAT3 Min;
		float dummy1;

		XMFLOAT3 Scale;
		float dummy2;

		XMFLOAT3 Bias;
		float dummy3;
	};

	using CLSTER = array<array<array<UINT, CLUSTERED_X>, CLUSTERED_Y>, CLUSTERED_Z>;
	using LIGHT_LIST = array<array<UINT, CLUSTERED_X* CLUSTERED_Y* CLUSTERED_Z>, 32>;

	unique_ptr <ID3D11Buffer, Release>					ConstatntBuffer;

	unique_ptr <ID3D11ComputeShader, Release>			ClusterCS;

	unique_ptr <ID3D11Texture3D, Release>				ClusterTexture;
	unique_ptr <ID3D11UnorderedAccessView, Release>		ClusterUAV;
	unique_ptr <ID3D11ShaderResourceView, Release>		ClusterSRV;

	unique_ptr <ID3D11Texture2D, Release>				Light_List_Texture;
	unique_ptr <ID3D11UnorderedAccessView, Release>		Light_List_UAV;
	unique_ptr <ID3D11ShaderResourceView, Release>		Light_List_SRV;

	unique_ptr<ID3D11Buffer, Release> ClusterBuffer;


	unique_ptr<CLSTER> Cluster;
	unique_ptr<LIGHT_LIST> LightList;

	BOUNDING_FRUSTUM m_Frustum;
	BOUNDING_AABB m_Aabb;

	array<array<array<bool, CLUSTERED_X>, CLUSTERED_Y>, CLUSTERED_Z> m_ClusterFlags = {};

	XMFLOAT3  m_Max = {}, m_Min = {};

	void Cale_Cluster(XMFLOAT3& max, XMFLOAT3& min);

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