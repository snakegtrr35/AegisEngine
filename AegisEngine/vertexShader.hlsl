

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}
cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}
cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}


// ライトバッファ
struct LIGHT
{
	float4		Direction;
	float4		Diffuse;
	float4		Ambient;
    float4      Specular;
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}



//=============================================================================
// 頂点シェーダ
//=============================================================================
void main( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float3 posLocal 	    : TEXCOORD1,
						  out float4 outDiffuse		: COLOR0 )
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	outPosition = mul( inPosition, wvp);
	outNormal = inNormal;
	outTexCoord = inTexCoord;
	
	float4 worldNormal, normal;
	normal = float4(inNormal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

	outDiffuse = inDiffuse * Material.Diffuse * light * Light.Diffuse;
	outDiffuse += inDiffuse * Material.Ambient * Light.Ambient;
    //outDiffuse += inDiffuse * Material.Specular * Light.Specular;
	outDiffuse.a = inDiffuse.a * Material.Diffuse.a;


    //float3 n;
    //float3 v;
    //float3 l;
    //float3 r;
    //float d;
    //float a;
    //float3 iA;
    //float3 iD;
    //float3 iS;
 
    //n = normalize(inNormal.xyz);
    //v = normalize(eyePos.xyz - inPosition.xyz);
    //l = pntLight.pos.xyz - input.posw.xyz;
    //d = length(l); //光源距離
    //l = normalize(l); //正規化光源ベクトル
    //r = 2.0 * n * dot(n, l) - l; //正規化反射ベクトル
 
    //iS = pow(saturate(dot(r, v)), Material.Specular.w) * Material.Specular.xyz * Light.Specular.xyz;
 
}

