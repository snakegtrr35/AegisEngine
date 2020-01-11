//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer ConstantBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}


// シャドウマップ用のマトリックス
cbuffer ShadowBuffer : register( b1 )
{
    matrix ShadowView;
    matrix ShadowProjection;
}


cbuffer BoneBuffer : register( b5 )
{
    matrix BoneMatrix[2]          : packoffset(c0);
    matrix BoneMatrixDmmy[253]    : packoffset(c8);
    matrix EndBoneMatrix          : packoffset(c1020);
    
    // c0 → c1 4バイト
    // c0 → c4 16バイト
    // 4 * 256(ボーンの最大数)
}



//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(  in float4 inPosition         : POSITION0,
		    in float4 inNormal           : NORMAL0,
		    in float4 inDiffuse          : COLOR0,
		    in float2 inTexCoord         : TEXCOORD0,
            in uint4  inIneces           : BLENDINDICE,
            in float4 inWeight           : BLENDWEIGHT,

            out float4 outPosition       : SV_POSITION,
		    out float4 outNormal         : NORMAL0,
		    out float2 outTexCoord       : TEXCOORD0,
		    out float4 outDiffuse        : COLOR0,
		    out float4 outWPos           : POSITION1,
            out float4 outShadowMapPos   : POSITION_SHADOWMAP )
{
    matrix BoneTransform = BoneMatrix[inIneces[0]] * inWeight[0]
                         + BoneMatrix[inIneces[1]] * inWeight[1]
                         + BoneMatrix[inIneces[2]] * inWeight[2]
                         + BoneMatrix[inIneces[3]] * inWeight[3];

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    outPosition = mul(inPosition, BoneTransform);
    outPosition = mul(outPosition, wvp);

    inNormal.w = 0.0;
    outNormal = mul(inNormal, BoneTransform);
    outNormal = mul(outNormal, World);

    outTexCoord = inTexCoord;
	
    float4 worldNormal, normal;
    normal = float4(outNormal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);

    outDiffuse = inDiffuse;
    
    outWPos = mul(inPosition, BoneTransform);
    outWPos = mul(outPosition, World);
    
    wvp = mul(World, ShadowView);
    wvp = mul(wvp, ShadowProjection);
    outShadowMapPos = mul(inPosition, BoneTransform);
    outShadowMapPos = mul(inPosition, wvp);
}