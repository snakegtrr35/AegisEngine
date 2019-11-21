

//*****************************************************************************
// 定数バッファ
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D g_Texture01 : register(t0);
Texture2D g_Texture02 : register(t1);

SamplerState g_SamplerState : register(s0);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in float4 inPosition : POSITION0,
		   in float4 inNormal : NORMAL0,
		   in float2 inTexCoord : TEXCOORD0,
		   in float4 inDiffuse : COLOR0,
              
		   out float4 outDiffuse : SV_Target)
{
    float4 Color01, Color02;

    Color01 = g_Texture01.Sample(g_SamplerState, inTexCoord);

    Color02 = g_Texture02.Sample(g_SamplerState, inTexCoord);

    outDiffuse = saturate(Color01 * Color02 * 2.0f);
}