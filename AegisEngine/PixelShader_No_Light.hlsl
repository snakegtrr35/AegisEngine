#include    "Commom_Hlsl.hlsli"

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�e���A���o�b�t�@
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float3 Dummy; //16bit���E�p
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}


// ���C�g�o�b�t�@
struct LIGHT
{
    float4      Direction;
    float4      Position;
    float4      Diffuse;
    float4      Ambient;
    float4      Specular;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main( PS_IN Input,

		   out float4 outDiffuse     : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, Input.TexCoord);

    if (outDiffuse.a <= 0.0)
        discard;

    // �O���[�X�P�[��
    /*float Y = outDiffuse.r * 0.29891f + outDiffuse.g * 0.58661f + outDiffuse.b * 0.11448f;

    outDiffuse.r = Y;

    outDiffuse.g = Y;

    outDiffuse.b = Y;*/

    // �l�K�e�B�u�t�B���^
    /*float4 inverted_color = 1 - outDiffuse;
    inverted_color.a = outDiffuse.a;
    inverted_color.rgb *= inverted_color.a;
    outDiffuse = inverted_color;*/

    // �Z�s�A
    /*outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);

    if (outDiffuse.a <= 0.0)
        discard;

    float Y = outDiffuse.r * 0.29891f + outDiffuse.g * 0.58661f + outDiffuse.b * 0.11448f;

    outDiffuse.r = Y;

    outDiffuse.g = Y;

    outDiffuse.b = Y;

    outDiffuse.r *= 1.07f;

    outDiffuse.g *= 0.74f;

    outDiffuse.b *= 0.43f;*/

    outDiffuse *= Input.Diffuse;

}