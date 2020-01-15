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
           out float4 outPos        : POSITION1)
{
    matrix wvp;

    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    inPosition.w = 1.0;
    
    outPosition = mul(inPosition, wvp);
    
    outPos = outPosition;
}