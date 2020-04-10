#ifndef LIGHT_HLSL_H
#define LIGHT_HLSL_H

//#define MAX_NUM_LIGHTS 64
//
//// ���C�g�o�b�t�@
//struct Lights
//{
//	// ���ʕ���
//	bool Enable /*: packoffset(c0)*/;
//	float3 Position /*: packoffset(c0.x)*/;
//	float4 Color /*: packoffset(c1)*/;
//	float Radius /*: packoffset(c2.x)*/;
//	float3 Attenuation /*: packoffset(c2.y)*/;

//	uint Type;
//	float3 Dummy;
//};
//
//// �}�g���N�X�o�b�t�@
//cbuffer LightsBuffer : register(b10)
//{
//	Lights LightsBuf[MAX_NUM_LIGHTS];
//}
//
//float3 DoPointLight(Lights light, float4 Position, float4 CameraPos, float4 Normal)
//{
//	float3 result = (float) 0.0;
//
//	float3 Dir = light.Position - Position.xyz;
//	//float3 Dir = Position.xyz - light.Position;
//	float distance = length(Dir);
//	//Dir = normalize(Dir);
//	Dir = Dir / distance;
//
//	//�g�U
//	float colD = 0.5 - 0.5 * dot(Normal.xyz, Dir);
//	colD = 1;
//
//	float colA = saturate(1.0f / (light.Attenuation.x + light.Attenuation.y * distance + light.Attenuation.z * distance * distance));
//
//	result = light.Color.rgb * (colD * colA);
//	return result;
//}
//
//bool Cul_Radius(Lights light, float4 Position)
//{
//	float x = (Position.xyz.x - light.Position.x) * (Position.xyz.x - light.Position.x);
//	float y = (Position.xyz.y - light.Position.y) * (Position.xyz.y - light.Position.y);
//	float z = (Position.xyz.z - light.Position.z) * (Position.xyz.z - light.Position.z);
//
//	float Dir = x + y + z;
//
//	return (Dir <= (light.Radius * light.Radius));
//}

#endif // !LIGHT_HLSL_H