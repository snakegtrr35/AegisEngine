#ifndef LIGHT_HLSL_H
#define LIGHT_HLSL_H

#define MAX_NUM_LIGHTS 512
#define LIGHT_TYPE_POINT 0          // ポイントライト
#define LIGHT_TYPE_SPOT 1           // スポットライト


static uint CLUSTERED_X = 32;
static uint CLUSTERED_Y = 32;
static uint CLUSTERED_Z = 16;


// ライトバッファ
struct Lights
{
    // 共通部分
    bool Enable;
    float3 Position;
    
    float4 Color;
    
    uint Type;
    float3 Dummy;
};

// マトリクスバッファ
cbuffer LightsBuffer : register(b6)
{
    Lights LightsBuf[MAX_NUM_LIGHTS];
}

// 定数バッファ
cbuffer ClusterBuffer : register(b7)
{
    float3 Scale;
    float dummy1;
    
    float3 Bias;
    float dummy2;
}

Texture3D<uint> g_Cluster : register(t10);
Texture2D<uint> g_LightList : register(t11);

float3 DoPointLight(Lights light, float3 Position)
{
    float3 result = 0.0;

    float3 Dir = light.Position - Position;
	//float3 Dir = Position - light.Position;
    float distance = length(Dir);

    //float colA = 1.0 / ((1.0 + distance) * (1.0 + distance));
    float colA = 1.0 / (distance * distance);
    
    result = light.Color.rgb * colA;
    
    return result;
}

#endif // !LIGHT_HLSL_H