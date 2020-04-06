#include    "Commom_Hlsl.hlsli"

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マテリアルバッファ
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float3 Dummy; //16bit境界用
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}


// ライトバッファ
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
// グローバル変数
//*****************************************************************************
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( PS_IN Input,

		   out float4 outDiffuse     : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, Input.TexCoord);

    if (outDiffuse.a <= 0.0)
        discard;

    // グレースケール
    /*float Y = outDiffuse.r * 0.29891f + outDiffuse.g * 0.58661f + outDiffuse.b * 0.11448f;

    outDiffuse.r = Y;

    outDiffuse.g = Y;

    outDiffuse.b = Y;*/

    // ネガティブフィルタ
    /*float4 inverted_color = 1 - outDiffuse;
    inverted_color.a = outDiffuse.a;
    inverted_color.rgb *= inverted_color.a;
    outDiffuse = inverted_color;*/

    // セピア
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