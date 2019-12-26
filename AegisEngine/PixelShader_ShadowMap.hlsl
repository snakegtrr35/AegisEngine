//*****************************************************************************
// 定数バッファ
//*****************************************************************************

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in float4 inPosition      : POSITION0,
          in float4 inPos           : COLOR0,

		  out float4 outDepth       : SV_Target)
{ 
    float depthValue = inPos.z / inPos.w;
    
    outDepth = float4(depthValue, depthValue, depthValue, 1.0f);
}