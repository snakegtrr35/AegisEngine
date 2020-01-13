#pragma once

#ifndef CLUSTERED_H
#define CLUSTERED_H

#define LIGHT_COUNT 9

static const constexpr UINT CLUSTER_X = 8;		// 32
static const constexpr UINT CLUSTER_Y = 2;		// 8
static const constexpr UINT CLUSTER_Z = 8;		// 32

struct Light
{
	XMFLOAT3 Position;
	float  Radius;
};

class CLUSTERED {
private:
	XMFLOAT3 AABB_min, AABB_max;
	unique_ptr<ID3D11Texture3D, Release>	Resource;
	unique_ptr<ID3D11ShaderResourceView, Release>	ShaderResourceView;

	vector<Light> m_Lights;

	struct CONSTANT
	{
		Light Lights[LIGHT_COUNT];
		XMFLOAT4 Scale;
		XMFLOAT4 Bias;
	};

	unique_ptr<ID3D11Buffer, Release>	ConstantBuffer;

	void clusteredLightAssignment();

public:

	CLUSTERED() {}
	~CLUSTERED() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();
};

#endif // !CLUSTERED_H