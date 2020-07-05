#define MAX_NUM_LIGHTS 512
#define LIGHT_TYPE_POINT 0          // �|�C���g���C�g
#define LIGHT_TYPE_SPOT 1           // �X�|�b�g���C�g

#define CLUSTERED_X 32
#define CLUSTERED_Y 32
#define CLUSTERED_Z 16

//static const uint MAX_NUM_LIGHTS = 512;

struct CS_INPUT{
    uint3 GTid : SV_GroupThreadID;
    uint3 Gid : SV_GroupID;
    uint3 DTid : SV_DispatchThreadID;
    uint Gix : SV_GroupIndex;
};

// ���C�g�o�b�t�@
struct Lights
{
	// ���ʕ���
    bool Enable /*: packoffset(c0)*/;
    float3 Position /*: packoffset(c0.x)*/;
    float4 Color /*: packoffset(c1)*/;
    float Radius /*: packoffset(c2.x)*/;
    float3 Attenuation /*: packoffset(c2.y)*/;

    uint Type;
    float3 Dummy;
};

// �}�g���N�X�o�b�t�@
cbuffer LightsBuffer : register(b10)
{
    Lights LightsBuf[MAX_NUM_LIGHTS];
}

groupshared bool g_BitBuffer[MAX_NUM_LIGHTS];

Texture3D<uint> g_In_Tex : register(t0);

RWTexture3D<uint> g_CluseredGrid : register(u0);
RWTexture2D<uint> g_LightList : register(u1);

[numthreads(MAX_NUM_LIGHTS, 1, 1)]
void main(CS_INPUT Input)
{
    // ���C�g�C���f�b�N�X
    uint light_index = Input.GTid.x;

    // �����蔻��
    g_BitBuffer[light_index] = false;
    g_BitBuffer[light_index] = true;
    
    GroupMemoryBarrierWithGroupSync();
    
    if (0 == Input.GTid.x)
    {
        const uint x = Input.Gid.z * CLUSTERED_X * CLUSTERED_Y + Input.Gid.y * CLUSTERED_X + Input.Gid.x;
        
        for (uint i = 0; i < MAX_NUM_LIGHTS; i++)
        {
            if (true == g_BitBuffer[i])
            {
                const uint quo = i / 32;
                g_CluseredGrid[Input.Gid] |= (1 << quo); // ���C�g�ԍ� / 32 �̏��̒l�̃r�b�g�𗧂Ă�
                
                const uint rem = i % 32;
                g_LightList[uint2(x, quo)] |= (1 << rem); // ���C�g�ԍ� / 32 �̗]��̒l�̃r�b�g�𗧂Ă�
            }
        }
    }
    
}