//*****************************************************************************
// 定数バッファ
//*****************************************************************************

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in float4 inPosition      : POSITION0,
		  in float4 inNormal        : NORMAL0,
          in float4 inColor         : COLOR0,

		  out float4 outDepth       : SV_Target0,
		  out float4 outalbedo      : SV_Target1,
		  out float4 outNormal      : SV_Target2)
{ 
    float depthValue = inPosition.z / inPosition.w;
    
    outDepth = float4(depthValue, depthValue, depthValue, 1.0f);
    
    outalbedo = inColor;
    
    float4 normal = 0.5 - 0.5 * normalize(inNormal);
    outNormal = float4(normal.xyz, 1.0);
}