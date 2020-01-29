#ifndef LIGHT_HLSL_H
#define LIGHT_HLSL_H

// マトリクスバッファ
struct Lights
{
	// 共通部分
	float3   Position;
	float4   Color;
	float    Radius;

	// ポイントライト
	float3   Attenuation;

	// スポットライト
	//float3	Attenuation;

	// 共通部分
	//int     Enable;
};

// マトリクスバッファ
cbuffer LightsBuffer : register(b6)
{
	Lights LightsBuf;
}

float3 DoPointLight(Lights light, float4 Position, float4 CameraPos, float4 Normal)
{
	float3 result = (float) 1.0;

	float3 Dir = Position.xyz - light.Position;
	float distance = length(Dir);
	Dir = normalize(Dir);

	//拡散
	float colD = saturate(dot(Normal.xyz, Dir));

	float colA = saturate(1.0f / (light.Attenuation.x + light.Attenuation.y * distance + light.Attenuation.z * distance * distance));

	result = light.Color.rgb * (colD * colA);
	return result;
}

bool Cul_Radius(Lights light, float4 Position)
{
	float3 Dir = Position.xyz - light.Position;
	float distance = length(Dir);
	distance *= distance;

	return (distance) <= (light.Radius * light.Radius);

}

#endif // !LIGHT_HLSL_H