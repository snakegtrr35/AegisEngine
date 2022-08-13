﻿#pragma once

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
		aegis::Vector3 Scale;
		float dummy1;

		aegis::Vector3 Bias;
		float dummy2;
	};

	struct CONSTATNT_BUFFER {
		aegis::Vector3 Min;
		float dummy1;

		aegis::Vector3 Scale;
		float dummy2;

		aegis::Vector3 Bias;
		float dummy3;
	};

	using CLSTER = aegis::array<aegis::array<aegis::array<UINT, CLUSTERED_X>, CLUSTERED_Y>, CLUSTERED_Z>;
	using LIGHT_LIST = aegis::array<aegis::array<UINT, CLUSTERED_X* CLUSTERED_Y* CLUSTERED_Z>, 32>;

	aegis::uniquePtr<aegis::Buffer>					ConstatntBuffer;

	aegis::uniquePtr<aegis::ComputeShader>			ClusterCS;

	aegis::uniquePtr<aegis::Texture3D>				ClusterTexture;
	aegis::uniquePtr<aegis::UnorderedAccessView>		ClusterUAV;
	aegis::uniquePtr<aegis::ShaderResourceView>		ClusterSRV;

	aegis::uniquePtr<aegis::Texture2D>				LightListTexture;
	aegis::uniquePtr<aegis::UnorderedAccessView>		LightListUAV;
	aegis::uniquePtr<aegis::ShaderResourceView>	LightListSRV;

	aegis::uniquePtr<aegis::Buffer>					ClusterBuffer;


	std::unique_ptr<CLSTER>					Cluster;
	std::unique_ptr<LIGHT_LIST>				LightList;

	BOUNDING_FRUSTUM					m_Frustum;
	BOUNDING_AABB						m_Aabb;

	aegis::array<aegis::array<aegis::array<bool, CLUSTERED_X>, CLUSTERED_Y>, CLUSTERED_Z> m_ClusterFlags = {};

	aegis::Vector3  m_Max = {}, m_Min = {};

	void Cale_Cluster(aegis::Vector3& max, aegis::Vector3& min);

public:

	CLUSTERED() : Cluster(std::make_unique<CLSTER>()), LightList(std::make_unique<LIGHT_LIST>()) {}
	~CLUSTERED() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();

	void Light_Culling();
};

#endif // !FORWARDLUS_H