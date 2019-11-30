

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

// シャドウマップ用のマトリックス
cbuffer ShadowBuffer : register(b1)
{
    matrix ShadowWorld;
    matrix ShadowView;
    matrix ShadowProjection;
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
    float4      Position;
	float4		Diffuse;
	float4		Ambient;
    float4      Specular;
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

cbuffer CameraBuffer : register(b5)
{
    float4 CameraPos;
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main( in float4 inPosition		: POSITION0,
		   in float4 inNormal		: NORMAL0,
		   in float4 inDiffuse		: COLOR0,
		   in float2 inTexCoord		: TEXCOORD0,
		   in uint   inInstansID    : SV_InstanceID,
           
		   out float4 outPosition	: SV_POSITION,
		   out float4 outNormal		: NORMAL0,
		   out float2 outTexCoord	: TEXCOORD0,
		   out float4 outDiffuse	: COLOR0,
		   out float4 outWPos       : TEXCOORD1,
           out float4 outShadowMapPos : POSITION_SHADOWMAP)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

    //inPosition.x += inInstansID % 100;
    //inPosition.z += inInstansID / 100;

	outPosition = mul( inPosition, wvp);

    //outNormal = float4(inNormal.xyz, 0.0);
    outNormal = mul(inNormal, World);
    outNormal = normalize(outNormal);

	outTexCoord = inTexCoord;
    outDiffuse = inDiffuse;
    outWPos = mul(inPosition, World);
    
    wvp = mul(ShadowWorld, ShadowView);
    wvp = mul(wvp, ShadowProjection);
    outShadowMapPos = mul(inPosition, wvp);
}

