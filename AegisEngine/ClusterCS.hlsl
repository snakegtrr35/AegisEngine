#include "Commom_Hlsl.hlsli"

#define MAX_NUM_LIGHTS 512
#define LIGHT_TYPE_POINT 0          // �|�C���g���C�g
#define LIGHT_TYPE_SPOT 1           // �X�|�b�g���C�g

static uint CLUSTERED_X = 32;
static uint CLUSTERED_Y = 32;
static uint CLUSTERED_Z = 16;

// �萔�o�b�t�@
cbuffer ConstatntBuffer : register(b0)
{
    float3 Min;
    float dummy1;
    
    float3 Scale;
    float dummy2;
    
    float3 Bias;
    float dummy3;
}

// ���C�g�o�b�t�@
struct Lights
{
    // ���ʕ���
    bool Enable;
    float3 Position;
    
    float4 Color;
    
    uint Type;
    float3 Dummy;
};

// ���C�g�o�b�t�@
cbuffer LightsBuffer : register(b1)
{
    Lights LightsBuf[MAX_NUM_LIGHTS];
}


RWTexture3D<uint> outCluster : register(u0);
RWTexture2D<uint> outLightList : register(u1);

// �O���[�v���L������
groupshared uint TempBuffer[2];

bool isHit(in float3 max, in float3 min, in float3 position, in float radius);

float CaleRadius(in Lights light);

[numthreads(MAX_NUM_LIGHTS, 1, 1)]
void main(CS_IN Input)
{
    const uint3 Gid = Input.Gid;
 
    const uint GTid = Input.GTid.x;
    
    const float x0 = Min.x + Scale.x * (float) Gid.x;
    const float x1 = Min.x + Scale.x * float(Gid.x + 1);

    const float y0 = Min.y + Scale.y * (float) Gid.y;
    const float y1 = Min.y + Scale.y * float(Gid.y + 1);

    const float z0 = Min.z + Scale.z * (float) Gid.z;
    const float z1 = Min.z + Scale.z * float(Gid.z + 1);

    const float3 max = float3(x1, y1, z1) * Bias;
    const float3 min = float3(x0, y0, z0) * Bias;
    
    InterlockedAdd(TempBuffer[0], 0);
    InterlockedAdd(TempBuffer[1], 0);
    
    GroupMemoryBarrierWithGroupSync();
    
    int quo = 0;
    int rem = 0;
    
    {
        Lights light = LightsBuf[GTid];
        
        if (light.Enable)
        {
			// �����蔻��
            if (isHit(max, min, light.Position, CaleRadius(light)))
            {
                quo = GTid / 32;
                rem = GTid % 32;
                
                InterlockedOr(TempBuffer[0], uint(1 << quo));
                
                InterlockedOr(TempBuffer[1], uint(1 << rem));
            }
        }
    }
    
    GroupMemoryBarrierWithGroupSync();
    
    {
        InterlockedOr(outCluster[uint3(Input.Gid.x, Input.Gid.y, Input.Gid.z)], TempBuffer[0]);

        InterlockedOr(outLightList[uint2(Gid.z * CLUSTERED_Z + Gid.y * CLUSTERED_Y + Gid.x * CLUSTERED_X, quo)], TempBuffer[1]);
    }
}



bool isHit(in float3 max, in float3 min, in float3 position, in float radius)
{
    float3 BoxExtents = (max - min) * 0.5;
    float3 BoxCenter = min + BoxExtents;

    float SqLen = 0.0; // �����ׂ̂���̒l���i�[

	// �e���œ_���ŏ��l�ȉ��������͍ő�l�ȏ�Ȃ�΁A�����l��
    if (position.x < min.x)
    {
        SqLen += (position.x - min.x) * (position.x - min.x);
    }
    else if (position.x > max.x)
    {
        SqLen += (position.x - max.x) * (position.x - max.x);
    }

    if (position.y < min.y)
    {
        SqLen += (position.y - min.y) * (position.y - min.y);
    }
    else if (position.y > max.y)
    {
        SqLen += (position.y - max.y) * (position.y - max.y);
    }

    if (position.z < min.z)
    {
        SqLen += (position.z - min.z) * (position.z - min.z);
    }
    else if (position.z > max.z)
    {
        SqLen += (position.z - max.z) * (position.z - max.z);
    }

    return SqLen <= (radius * radius);
}

float CaleRadius(in Lights light)
{
    float luminance = dot(light.Color.rgb, float3(0.298912, 0.586611, 0.114478));

    float distance = 0.0;

    distance = sqrt(luminance * 500.0) - 1.0;

    return distance;
}