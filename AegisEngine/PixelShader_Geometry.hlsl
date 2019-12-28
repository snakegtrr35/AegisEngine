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


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float2 inTexCoord     : TEXCOORD0,
		   in float4 inDiffuse      : COLOR0,
           in float4 inPos          : POSITION1,
           
		   out float4 outDepth      : SV_Target0,
		   out float4 outalbedo     : SV_Target1,
		   out float4 outNormal     : SV_Target2)
{
    float depthValue = inPos.z / inPos.w;
    
    outDepth = float4(depthValue, depthValue, depthValue, 1.0f);
    
    outalbedo = g_Texture.Sample(g_SamplerState, inTexCoord);
   
    inNormal = normalize(inNormal);
    
    outNormal = 0.5 * (normalize(inNormal) + 1.0);
    
    outNormal.w = 1.0f;
}