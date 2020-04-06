#include    "Commom_Hlsl.hlsli"
#include    "Light_Hlsl.hlsli"

//*****************************************************************************
// 定数バッファ
//*****************************************************************************



#define MAX_NUM_LIGHTS 64
#define LIGHT_TYPE_POINT 0          // ポイントライト
#define LIGHT_TYPE_SPOT 1           // スポットライト

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
cbuffer LightsBuffer : register(b6)
{
    Lights LightsBuf[MAX_NUM_LIGHTS];
}

float3 DoPointLight(Lights light, float4 Position, float4 CameraPos, float4 Normal)
{
    float3 result = (float) 0.0;

    float3 Dir = light.Position - Position.xyz;
    //float3 Dir = Position.xyz - light.Position;
    float distance = length(Dir);
    //Dir = normalize(Dir);
    Dir = Dir / distance;

    //拡散
    float colD = 0.5 - 0.5 * dot(Normal.xyz, Dir);
    //colD = 1;

    float colA = saturate(1.0f / (light.Attenuation.x + light.Attenuation.y * distance + light.Attenuation.z * distance * distance));

    result = light.Color.rgb * (colD * colA);
    return result;
}

bool Cul_Radius(Lights light, float4 Position)
{
    float x = (Position.xyz.x - light.Position.x) * (Position.xyz.x - light.Position.x);
    float y = (Position.xyz.y - light.Position.y) * (Position.xyz.y - light.Position.y);
    float z = (Position.xyz.z - light.Position.z) * (Position.xyz.z - light.Position.z);

    float Dir = x + y + z;

    return (Dir <= (light.Radius * light.Radius));
}




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
    //float shadow = 1.0;
    
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

    float3 light_color = (float3) 0.0;
    
    for (int i = 0; i < MAX_NUM_LIGHTS; i++)
    {
        if (LightsBuf[i].Enable)
        {
            switch (LightsBuf[i].Type)
            {
                case LIGHT_TYPE_POINT:
                    if (Cul_Radius(LightsBuf[i], Input.WPos))
                    {
                        float3 col = DoPointLight(LightsBuf[i], Input.WPos, CameraPos, Input.Normal);
                        light_color = saturate(col + light_color);
                    }
                break;
                
                case LIGHT_TYPE_SPOT:
                    break;
            }
        }

    }
       
    color.rgb += light_color;
    
    outDiffuse = color;
}