

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************


// �}�g���N�X�o�b�t�@
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

//cbuffer WorldBuffer : register(b0)
//{
//    matrix World;
//}


// �}�e���A���o�b�t�@
struct MATERIAL
{
    float4      Ambient;
    float4      Diffuse;
    float4      Specular;
    float4      Emission;
    float       Shininess;
    float3      Dummy; //16bit���E�p
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}


// ���C�g�o�b�t�@
struct LIGHT
{
    float4      Direction;
    float4      Diffuse;
    float4      Ambient;
    float4      Specular;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}


cbuffer CameraBuffer : register(b5)
{
    float4 CameraPos;
}


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D       g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


float3 Phong
(
    float3 N,
    float3 L,
    float3 V,
    float3 Kd,
    float3 Ks,
    float n )
{
    float3 R = normalize(reflect(N, V));
    float3 cosAr = saturate(dot(V, N));
    float3 cosAl = saturate(dot(N, L));
    float3 C = (Kd + Ks * (n + 2) * 0.5f * pow(cosAr, n)) * cosAl;

    return C;
}

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float3 posLocal		: TEXCOORD1,
						 in  float4 inDiffuse		: COLOR0,

						 out float4 outDiffuse		: SV_Target )
{
    //outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);

    //if (outDiffuse.a <= 0.0)
    //    discard;

    //outDiffuse *= inDiffuse;

    float4 worldNormal, normal;
    normal = float4(inNormal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);

    float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

    float4 Color;

    Color = inDiffuse * Material.Diffuse * light * Light.Diffuse;
    Color += inDiffuse * Material.Ambient * Light.Ambient;
    Color.a = inDiffuse.a * Material.Diffuse.a;

    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);

    if (outDiffuse.a <= 0.0)
        discard;

    outDiffuse *= Color;
}
