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


cbuffer BoneBuffer : register(b6)
{
    matrix BoneMatrix[2] : packoffset(c0);
    matrix BoneMatrixDmmy[253] : packoffset(c8);
    matrix EndBoneMatrix : packoffset(c1020);
    
    // c0 → c1 4バイト
    // c0 → c4 16バイト
    // 4 * 256(ボーンの最大数)
}


void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float4 inDiffuse      : COLOR0,
		   in float2 inTexCoord     : TEXCOORD0,
           in uint4 inIneces        : BLENDINDICE,
           in float4 inWeight       : BLENDWEIGHT,

           out float4 outPosition   : SV_POSITION)
{
    matrix BoneTransform = BoneMatrix[inIneces[0]] * inWeight[0]
                         + BoneMatrix[inIneces[1]] * inWeight[1]
                         + BoneMatrix[inIneces[2]] * inWeight[2]
                         + BoneMatrix[inIneces[3]] * inWeight[3];
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    inPosition.w = 1;
    outPosition = mul(inPosition, BoneTransform);
    outPosition = mul(outPosition, wvp);
}