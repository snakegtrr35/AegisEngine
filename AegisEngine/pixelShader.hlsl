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
Texture2D       g_Texture   : register( t0 );
Texture2D       g_ShadowMap : register( t1 );     // シャドウマップ
Texture2D       g_Depth     : register( t2 );     // デプス
Texture2D       g_Albedo    : register( t3 );     // アルベド
Texture2D       g_Normal    : register( t4 );     // 法線

SamplerState	g_SamplerState : register( s0 );
SamplerState    g_ShadowSamplerState : register(s1);        // シャドウマップ用のサンプラー


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float2 inTexCoord     : TEXCOORD0,
		   in float4 inDiffuse      : COLOR0,
		   in float4 inWPos         : POSITION1,
           in float4 inShadowMapPos : POSITION_SHADOWMAP,
           
		   out float4 outDiffuse	: SV_Target )
{
    float4 TexColor = g_Texture.Sample(g_SamplerState, inTexCoord);
    
    if (TexColor.a <= 0.0)
        discard;
    
    inNormal = normalize(inNormal);
    float light = 0.5 - 0.5 * dot(Light.Direction.xyz, inNormal.xyz);
    
    
    float vec = dot(inNormal.xyz, Light.Direction.xyz);
    //vec = normalize(vec);
   
    
    // 環境反射光
    float4 ambient = inDiffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0);
    // 拡散反射光
    float4 diffuse = inDiffuse * float4(Material.Diffuse.rgb * (light * Light.Diffuse.rgb), 1.0);

    float4 color = (float4)0.0;

    //
    float3 refrect = reflect(Light.Direction.xyz, inNormal.xyz);
    refrect = normalize(refrect);
    // 視線ベクトル
    float3 eye_vec = inWPos - CameraPos;
    eye_vec = normalize(eye_vec);
    // スペキュラ成分を求める
    float speculer = -dot(eye_vec, refrect);
    speculer = saturate(speculer);
    speculer = pow(speculer, 60);
    //
    
    // シャドウマップ
    float shadow;
    
    float2 ShadowTexCoord;
    ShadowTexCoord.x = inShadowMapPos.x / inShadowMapPos.w / 2.0 + 0.5;
    ShadowTexCoord.y = -inShadowMapPos.y / inShadowMapPos.w / 2.0 + 0.5;
    
    //float w = 1.0f / inShadowMapPos.w;
    //ShadowTexCoord.x = (1.0 + inShadowMapPos.x * w) * 0.5;
    //ShadowTexCoord.y = (1.0 - inShadowMapPos.y * w) * 0.5;
    
    {
        float depthValue = g_ShadowMap.Sample(g_ShadowSamplerState, ShadowTexCoord).r;

        float lightDepthValue = inShadowMapPos.z / inShadowMapPos.w;

        lightDepthValue = lightDepthValue - 0.003;

        if (lightDepthValue <= depthValue || vec >= 0.0)
        {
            shadow = 1.0;
        }
        else
        {
            shadow = 0.5;
        }
    }
    
    color = (diffuse * TexColor) + speculer;

    color.rgb *= shadow;
    
    color.rgb += (ambient * TexColor);
    

    outDiffuse = color;
}