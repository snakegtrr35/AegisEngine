

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
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float2 inTexCoord     : TEXCOORD0,
		   in float4 inDiffuse      : COLOR0,
		   in float4 outWPos        : TEXCOORD1,

           
		   out float4 outDiffuse	: SV_Target )
{
    //float4 worldNormal, normal;
    //normal = float4(inNormal.xyz, 0.0);
    //worldNormal = mul(normal, World);
    //worldNormal = normalize(worldNormal);

    //float light = 0.5 - 0.5 * dot(normalize(Light.Direction.xyz), inNormal.xyz);

    //float4 Color;

    //Color = inDiffuse * Material.Diffuse * light * Light.Diffuse;
    //Color += inDiffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0f);
    //float4 diffuse = inDiffuse * float4(Material.Diffuse.rgb * light * Light.Diffuse.rgb, 1.0f);
    //float4 ambient = inDiffuse * Material.Ambient * Light.Ambient;
    //Color.a = inDiffuse.a * Material.Diffuse.a;


    float3 L = normalize(-Light.Position.xyz - outWPos.xyz); // 物体表面上の点から光源への方向ベクトル
    float3 N = normalize(inNormal); // 表面上の点における法線
    float LN = 0.5 - 0.5 * dot(L, N);

    //float3 R = normalize(2 * LN * N - normalize(LightPos.xyz)); // 光線が反射される方向
    //float3 V = normalize(CameraPos.xyz - outWPos.xyz); // 視点に向かう方向
    //float RV = 0.5 - 0.5 * dot(R, V);
 
    // 環境反射光
    float4 ambient = inDiffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0f);
    // 拡散反射光
    float4 diffuse = inDiffuse * float4(Material.Diffuse.rgb * LN * Light.Diffuse.rgb, 1.0f);

    float4 color = (float4)0.0;

    //float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //計算のためにライトの方向を反転します。
    //float3 lightDir =L;

    //
    //float3 R = 2.0f * N * dot(N.xyz, L) - L;
    //float3 V = normalize(CameraPos.xyz - outWPos.xyz);

    //float specPower = pow(saturate(dot(R, V)), Light.Specular.w);
    
    //

    //
    // ワールド空間でのライトの位置
    float3 lightPosWorld = -Light.Position.xyz;

    // 現在参照している物体表面からライトへの方向を求める
    float3 toLightDir = normalize(lightPosWorld.xyz - outWPos.xyz);

    // 現在参照している物体表面から視線への方向を計算
    float3 toCameraDir = normalize(CameraPos.xyz - outWPos.xyz);

    // 反射ベクトルをハーフベクトルで近似
    float3 halfDir = normalize(toLightDir + toCameraDir);
    //float3 toCameraDir = normalize(CameraPos.xyz - outWPos.xyz);
    //float3 R = -toCameraDir + 2.0f * dot(inNormal.xyz, toCameraDir) * inNormal.xyz;
    
    // スペキュラ成分を求める
    //float specular = pow( saturate( dot(halfDir, inNormal.xyz) ), Light.Specular.w );
    float specular = pow(saturate(dot(halfDir, inNormal.xyz)), Light.Specular.w);

    //
 

    ////このピクセルの光量を計算します。
    //float lightIntensity = saturate(dot(inNormal.xyz, lightDir));
    ////lightIntensity = 1.0f;
    //float3 view = normalize(CameraPos.xyz - outWPos.xyz);
    //if (lightIntensity > 0.0f)
    //{
       
    //    color += (diffuse * lightIntensity);
    //    color = saturate(color);

    //    float3 reflection = normalize(50.0f * lightIntensity * inNormal.xyz - lightDir);
    //    //float i = pow(saturate(dot(h, v)), Light.Specular.w);
    //    specular = pow(saturate(dot(reflection, view)), Light.Specular.w);
    //    // 鏡面反射光
    //    //float4 specular = saturate(60.0 * pow(RV, 20.0) * (Light.Specular * Material.Specular) );
    //    //specular = i * (inDiffuse * float4(Light.Specular.rgb * Material.Specular.rgb, 1.0f));
    //}

    color = (diffuse * g_Texture.Sample(g_SamplerState, inTexCoord)) + specular
            + (ambient * g_Texture.Sample(g_SamplerState, inTexCoord));

    //color = saturate(color + specular);

    //outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);

    outDiffuse = color;

    if (outDiffuse.a <= 0.0)
        discard;


    //outDiffuse += specular;
}
