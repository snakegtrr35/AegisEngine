//*****************************************************************************
// 定数バッファ
//*****************************************************************************

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in float4 inPosition      : POSITION0,
		  in float4 inNormal        : NORMAL0,
          in float4 inColor         : COLOR0,
		  in float2 inTexCoord      : TEXCOORD0,

		  out float4 outDepth       : SV_Target0,
		  out float4 outAlbedo      : SV_Target1,
		  out float4 outNormal      : SV_Target2)
{ 
    float depthValue = inColor.z / inColor.w;
    
    outDepth = float4(depthValue, depthValue, depthValue, 1.0f);
    
    outAlbedo = g_Texture.Sample(g_SamplerState, inTexCoord);
    
    float4 normal = 0.5 * (normalize(inNormal) + 1.0);
    
    outNormal = float4(normal.xyz, 1.0);
}