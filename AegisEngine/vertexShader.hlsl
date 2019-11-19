

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

//cbuffer WorldBuffer : register(b0)
//{
//    matrix World;
//}
//cbuffer ViewBuffer : register(b1)
//{
//    matrix View;
//}
//cbuffer ProjectionBuffer : register(b2)
//{
//    matrix Projection;
//}

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
    outDiffuse = inDiffuse;//

    posLocal = inPosition.xyz;
	
	//float4 worldNormal, normal;
	//normal = float4(inNormal.xyz, 0.0);
	//worldNormal = mul(normal, World);
	//worldNormal = normalize(worldNormal);

	//float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

	//outDiffuse = inDiffuse * Material.Diffuse * light * Light.Diffuse;
	//outDiffuse += inDiffuse * Material.Ambient * Light.Ambient;
	//outDiffuse.a = inDiffuse.a * Material.Diffuse.a;
}

