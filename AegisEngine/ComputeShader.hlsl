#define MAX_NUM_LIGHTS 64
#define LIGHT_TYPE_POINT 0          // ポイントライト
#define LIGHT_TYPE_SPOT 1           // スポットライト

//static const uint MAX_NUM_LIGHTS = 512;

struct CS_INPUT{
    uint3 GTid : SV_GroupThreadID;
    uint3 Gid : SV_GroupID;
    uint3 DTid : SV_DispatchThreadID;
    uint Gix : SV_GroupIndex;
};

// ライトバッファ
struct Lights
{
	// 共通部分
    bool Enable /*: packoffset(c0)*/;
    float3 Position /*: packoffset(c0.x)*/;
    float4 Color /*: packoffset(c1)*/;
    float Radius /*: packoffset(c2.x)*/;
    float3 Attenuation /*: packoffset(c2.y)*/;

    uint Type;
    float3 Dummy;
};

// マトリクスバッファ
cbuffer LightsBuffer : register(b10)
{
    Lights LightsBuf[MAX_NUM_LIGHTS];
}

Texture3D<uint> g_In_Tex : register(t0);

RWTexture3D<uint> g_CluseredGrid : register(t1);
RWTexture3D<uint> g_LightList : register(t2);

groupshared uint g_BitBuffer;

[numthreads(MAX_NUM_LIGHTS, 1, 1)]
void main(CS_INPUT Input)
{
    // ライトインデックス
    uint light_index = Input.GTid.x;

}