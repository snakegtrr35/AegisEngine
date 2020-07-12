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
    float4 Direction;
    float4 Position;
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}



#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 12
//#define FXAA_DISCARD 1
#include "FXAA.hlsl"

#define    PIXEL_SIZE float2(1920.0, 1080.0)
#define    SUBPIX 0.75
#define    EDGE_THRESHOLD 0.166
#define    EDGE_THRESHOLD_MIN 0.0833


float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);
SamplerState g_LinearSampler : register(s1);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(PS_IN Input,

		  out float4 outDiffuse : SV_Target)
{
    if (g_Texture.Sample(g_SamplerState, Input.TexCoord).a <= 0.0)
        discard;
    
    float2 uv = Input.TexCoord.xy;
    
    //outDiffuse.rgb = ACESFilm(g_Texture.Sample(g_SamplerState, Input.TexCoord).rgb);
    //outDiffuse.a = g_Texture.Sample(g_SamplerState, Input.TexCoord).a;
    
    
    //FxaaTex InputFXAATex = { g_LinearSampler, g_Texture };
    //outDiffuse = FxaaPixelShader
    //(
    //    uv, // FxaaFloat2 pos,
    //    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsolePosPos,
    //    InputFXAATex, // FxaaTex tex,
    //    InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegOne,
    //    InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegTwo,
    //    PIXEL_SIZE, // FxaaFloat2 fxaaQualityRcpFrame,
    //    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt,
    //    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt2,
    //    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsole360RcpFrameOpt2,
    //    SUBPIX, // FxaaFloat fxaaQualitySubpix,
    //    EDGE_THRESHOLD, // FxaaFloat fxaaQualityEdgeThreshold,
    //    EDGE_THRESHOLD_MIN, // FxaaFloat fxaaQualityEdgeThresholdMin,
    //    0.0f, // FxaaFloat fxaaConsoleEdgeSharpness,
    //    0.0f, // FxaaFloat fxaaConsoleEdgeThreshold,
    //    0.0f, // FxaaFloat fxaaConsoleEdgeThresholdMin,
    //    FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f) // FxaaFloat fxaaConsole360ConstDir,
    //);

}