#include    "Commom_Hlsl.hlsli"
#include    "Light_Hlsl.hlsli"

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

    //float3 light_color = (float3) 0.0;
    
    //for (int i = 0; i < MAX_NUM_LIGHTS; i++)
    //{
    //    if (LightsBuf[i].Enable)
    //    {
    //        switch (LightsBuf[i].Type)
    //        {
    //            case LIGHT_TYPE_POINT:
    //                if (Cul_Radius(LightsBuf[i], Input.WPos))
    //                {
    //                    float3 col = DoPointLight(LightsBuf[i], Input.WPos, CameraPos, Input.Normal);
    //                    light_color = saturate(col + light_color);
    //                }
    //            break;
                
    //            case LIGHT_TYPE_SPOT:
    //                break;
    //        }
    //    }

    //}
    
    {
        float3 light_color = 0.0;
        {
            float3 world_pos = Input.WPos.xyz;
            
            int3 coord = int3(floor(abs((Bias - world_pos) / Scale)));
            uint cluster_mask = g_Cluster.Load(float4(coord, 0));
       
            while (cluster_mask)
            {
                uint cluster_index = firstbitlow(cluster_mask);
                cluster_mask &= ~(1 << cluster_index);

                int3 coord_index = coord.xyz;
        
                // 3Dでの番号を1Dの番号に変換
                int index = (coord_index.z * CLUSTERED_Z + CLUSTERED_Y * coord_index.y + CLUSTERED_X * coord_index.x) /*+ 1*/;
        
                uint light_mask = g_LightList.Load(int3(index, cluster_index, 0));
        
                while (light_mask)
                {
                    uint light_index = firstbitlow(light_mask);
                    light_mask &= ~(1 << light_index);
            
                    uint light_idx = mad(cluster_index, 32, light_index);

                    Lights light = LightsBuf[light_idx];
            
                    if (light.Enable)
                    {
                        switch (light.Type)
                        {
                            case LIGHT_TYPE_POINT:
                                float3 col = DoPointLight(light, world_pos);
                                light_color = saturate(col + light_color);
                                break;
            
                            case LIGHT_TYPE_SPOT:
                                break;
                        }
                    }
                }
            }
            
        }
        color.rgb += light_color;
    }
    
    outDiffuse = color;
}