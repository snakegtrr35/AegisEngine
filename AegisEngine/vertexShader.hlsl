//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}


// �V���h�E�}�b�v�p�̃}�g���b�N�X
cbuffer ShadowBuffer : register(b1)
{
    matrix ShadowView;
    matrix ShadowProjection;
}


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void main( in float4 inPosition		    : POSITION0,
		   in float4 inNormal		    : NORMAL0,
		   in float4 inDiffuse		    : COLOR0,
		   in float2 inTexCoord		    : TEXCOORD0,
		   in uint   inInstansID        : SV_InstanceID,
           
		   out float4 outPosition	    : SV_POSITION,
		   out float4 outNormal		    : NORMAL0,
		   out float2 outTexCoord	    : TEXCOORD0,
		   out float4 outDiffuse	    : COLOR0,
		   out float4 outWPos           : POSITION1,
           out float4 outShadowMapPos   : POSITION_SHADOWMAP)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

    //inPosition.x += inInstansID % 100;
    //inPosition.z += inInstansID / 100;

	outPosition = mul( inPosition, wvp);

    inNormal.w = 0.0;
    outNormal = mul(inNormal, World);

	outTexCoord = inTexCoord;
    
    outDiffuse = inDiffuse;
    
    outWPos = mul(inPosition, World);
    
    wvp = mul(World, ShadowView);
    wvp = mul(wvp, ShadowProjection);
    outShadowMapPos = mul(inPosition, wvp);
}