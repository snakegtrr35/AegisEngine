//*****************************************************************************
// 定数バッファ
//*****************************************************************************


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in float4 inPosition          : POSITION0,
		  in float4 inNormal            : NORMAL0,
		  in float2 inTexCoord          : TEXCOORD0,
		  in float4 inDiffuse           : COLOR0,
		  in float4 outWPos             : TEXCOORD1,
          in float4 inShadowMapPos      : POSITION_SHADOWMAP,

		  out float4 outDiffuse         : SV_Target)
{
    float depthValue = inShadowMapPos.z / inShadowMapPos.w;
    
    outDiffuse = float4(depthValue, depthValue, depthValue, 1.0f);
}