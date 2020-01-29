#ifndef LIGHT_HLSL_H
#define LIGHT_HLSL_H

// �}�g���N�X�o�b�t�@
struct Lights {
	// ���ʕ���
	float3 Position;
	float4 Color;
	float Radius;

	// �|�C���g���C�g
	float3 Attenuation;

	// �X�|�b�g���C�g
	//float3	Attenuation;

	// ���ʕ���
	bool Enable;
};

// �}�g���N�X�o�b�t�@
cbuffer LightsBuffer : register(b6)
{
	Lights LightsBuf;
}



#endif // !LIGHT_HLSL_H