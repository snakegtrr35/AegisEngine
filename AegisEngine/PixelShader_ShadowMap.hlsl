//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main(in float4 inPosition      : POSITION0,
          in float4 inColor         : COLOR0,

		  out float4 outDiffuse     : SV_Target)
{ 
    float depthValue = inColor.z / inColor.w;
    
    outDiffuse = float4(depthValue, depthValue, depthValue, 1.0f);
}