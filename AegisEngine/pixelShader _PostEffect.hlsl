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
#define FXAA_GREEN_AS_LUMA 0
#include "FXAA.hlsl"

#define    PIXEL_SIZE float2(1.0 / 1920.0, 1.0 / 1080.0)
#define    SUBPIX 0.75
#define    EDGE_THRESHOLD 0.166
#define    EDGE_THRESHOLD_MIN 0.0833


//float3 ACESFilm(float3 x)
//{
//    float a = 2.51f;
//    float b = 0.03f;
//    float c = 2.43f;
//    float d = 0.59f;
//    float e = 0.14f;
//    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
//}


static const float3x3 ACESInputMat =
{
    { 0.59719, 0.35458, 0.04823 },
    { 0.07600, 0.90834, 0.01566 },
    { 0.02840, 0.13383, 0.83777 }
};

// ODT_SAT => XYZ => D60_2_D65 => sRGB
static const float3x3 ACESOutputMat =
{
    { 1.60475, -0.53108, -0.07367 },
    { -0.10208, 1.10813, -0.00605 },
    { -0.00327, -0.07276, 1.07602 }
};

float3 RRTAndODTFit(float3 v)
{
    float3 a = v * (v + 0.0245786) - 0.000090537;
    float3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return a / b;
}

float3 ACESFilm(float3 color)
{
    color = mul(ACESInputMat, color);

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = mul(ACESOutputMat, color);

    // Clamp to [0, 1]
    color = saturate(color);

    return color;
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
    float2 uv = Input.TexCoord.xy;
    
    //outDiffuse.rgb = /*ACESFilm*/(g_Texture.Sample(g_SamplerState, Input.TexCoord).rgb);
    //outDiffuse.rgb = sqrt(outDiffuse.rgb); //ガンマ2.0カラー出力
    //outDiffuse.a = dot(outDiffuse.rgb, float3(0.298912, 0.586611, 0.114478));
    //outDiffuse.rgb = pow(outDiffuse.rgb, 1.0 / 1.5);
    //outDiffuse.a = 1.0;
    
    FxaaTex InputFXAATex = { g_LinearSampler, g_Texture };
    outDiffuse = FxaaPixelShader
    (
        uv, // FxaaFloat2 pos,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsolePosPos,
        InputFXAATex, // FxaaTex tex,
        InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegOne,
        InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegTwo,
        PIXEL_SIZE, // FxaaFloat2 fxaaQualityRcpFrame,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt2,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsole360RcpFrameOpt2,
        SUBPIX, // FxaaFloat fxaaQualitySubpix,
        EDGE_THRESHOLD, // FxaaFloat fxaaQualityEdgeThreshold,
        EDGE_THRESHOLD_MIN, // FxaaFloat fxaaQualityEdgeThresholdMin,
        0.0f, // FxaaFloat fxaaConsoleEdgeSharpness,
        0.0f, // FxaaFloat fxaaConsoleEdgeThreshold,
        0.0f, // FxaaFloat fxaaConsoleEdgeThresholdMin,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f) // FxaaFloat fxaaConsole360ConstDir,
    );
}