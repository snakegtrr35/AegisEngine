#include    "Commom_Hlsl.h"

//*****************************************************************************
// 定数バッファ
//*****************************************************************************


// マトリクスバッファ
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}


// マテリアルバッファ
struct MATERIAL
{
    float4      Ambient;
    float4      Diffuse;
    float4      Specular;
    float4      Emission;
    float       Shininess;
    float3      Dummy; //16bit境界用
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


cbuffer CameraBuffer : register(b5)
{
    float4 CameraPos;
}


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D       g_Texture : register( t0 );
Texture2D       g_ShadowMap : register(t1);     // シャドウマップ

SamplerState	g_SamplerState : register( s0 );
SamplerState    g_ShadowSamplerState : register(s1);        // シャドウマップ用のサンプラー

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( PS_IN Input,
           
		   out float4 outDiffuse	: SV_Target )
{
    float4 TexColor = g_Texture.Sample(g_SamplerState, Input.TexCoord);
    
    if (TexColor.a <= 0.0)
        discard;
    
    Input.Normal = normalize(Input.Normal);
    float light = 0.5 - 0.5 * dot(Light.Direction.xyz, Input.Normal.xyz);
    
    
    float vec = dot(Input.Normal.xyz, Light.Direction.xyz);
    //vec = normalize(vec);
   
    
    // 環境反射光
    float4 ambient = Input.Diffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0);
    // 拡散反射光
    float4 diffuse = Input.Diffuse * float4(Material.Diffuse.rgb * (light * Light.Diffuse.rgb), 1.0);

    float4 color = (float4)0.0;

    //
    float3 refrect = reflect(Light.Direction.xyz, Input.Normal.xyz);
    refrect = normalize(refrect);
    // 視線ベクトル
    float3 eye_vec = Input.WPos.xyz - CameraPos.xyz;
    eye_vec = normalize(eye_vec);
    // スペキュラ成分を求める
    float speculer = -dot(eye_vec, refrect);
    speculer = saturate(speculer);
    speculer = pow(speculer, 60);
    //
    
    // シャドウマップ
    float shadow = 0.5;
    
    float2 ShadowTexCoord;
    ShadowTexCoord.x = Input.ShadowMapPos.x / Input.ShadowMapPos.w / 2.0 + 0.5;
    ShadowTexCoord.y = -Input.ShadowMapPos.y / Input.ShadowMapPos.w / 2.0 + 0.5;
    
    {
        float depthValue = g_ShadowMap.Sample(g_ShadowSamplerState, ShadowTexCoord).r;

        float lightDepthValue = Input.ShadowMapPos.z / Input.ShadowMapPos.w;

        lightDepthValue = lightDepthValue - 0.005;

        if (lightDepthValue <= depthValue || vec >= 0.0)
        {
            shadow = 1.0;
        }
    }
    
    color = (diffuse * TexColor) + speculer;

    color.rgb *= shadow;
    
    color.rgb += (ambient.rgb * TexColor.rgb);

    outDiffuse = color;
}




//// マトリクスバッファ
//struct Lights {
//    // 共通部分
//    float3 Position;
//    float4 Color;
//    float Radius;

//	// ポイントライト
//    float3 Attenuation;

//	// スポットライト
//	//float3	Attenuation;

//	// 共通部分
//    bool Enable;
//};

//// マトリクスバッファ
//cbuffer LightsBuffer  : register(b6)
//{
//    Lights LightsBuf;
//}

//float4 DoPointLight(Lights light, float4 Position, float4 CameraPos, float4 Normal)
//{
//    float4 result = (float4)1.0;

//    float3 Dir = light.Position - Position.xyz;
//    float distance = length(Dir);
//    Dir = normalize(Dir);

//    //拡散
//    float colD = saturate(dot(Normal.xyz, Dir));
    
//    float colA = saturate(1.0f / (light.Attenuation.x + light.Attenuation.y * distance + light.Attenuation.z * distance * distance));
    
//    result.rgb = light.Color.rgb * (colD * colA);
//    return result;

//}