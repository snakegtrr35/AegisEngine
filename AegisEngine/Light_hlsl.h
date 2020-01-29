#ifndef LIGHT_HLSL_H
#define LIGHT_HLSL_H

// マトリクスバッファ
struct Lights {
	// 共通部分
	float3 Position;
	float4 Color;
	float Radius;

	// ポイントライト
	float3 Attenuation;

	// スポットライト
	//float3	Attenuation;

	// 共通部分
	bool Enable;
};

// マトリクスバッファ
cbuffer LightsBuffer : register(b6)
{
	Lights LightsBuf;
}



#endif // !LIGHT_HLSL_H