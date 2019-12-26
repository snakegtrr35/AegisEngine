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


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float4 inDiffuse      : COLOR0,
		   in float2 inTexCoord     : TEXCOORD0,
           
		   out float4 outPosition   : SV_POSITION,
		   out float4 outNormal     : NORMAL0,
		   out float2 outTexCoord   : TEXCOORD0,
		   out float4 outDiffuse    : COLOR0,
           out float4 outPos        : POSITIONT1)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    outPosition = mul(inPosition, wvp);

    inNormal.w = 0.0;
    outNormal = mul(inNormal, World);

    outTexCoord = inTexCoord;
    
    outDiffuse = inDiffuse;
    
    outPos = outPosition;
}