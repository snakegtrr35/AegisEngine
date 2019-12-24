

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
           out float4 outColor      : COLOR0)
{
    matrix wvp;

    inPosition.w = 1.0f;
    
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);
    
    inNormal.w = 0.0;
    outNormal = mul(inNormal, World);
    
    outColor = outPosition;
}