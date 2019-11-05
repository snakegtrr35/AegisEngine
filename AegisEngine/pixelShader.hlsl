

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

//// マテリアルバッファ
//cbuffer MaterialBuffer : register( b1 )
//{
//	float4		Ambient;
//	float4		Diffuse;
//	float4		Specular;
//	float4		Emission;
//	float		Shininess;
//	float3		Dummy;//16bit境界用
//}


// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}


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
    float4 Diffuse;
    float4 Ambient;
    float4      Specular;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D       g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
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
