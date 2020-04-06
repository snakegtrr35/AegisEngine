#include    "Commom_Hlsl.hlsli"

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
PS_IN main( VS_IN Input)
{
    PS_IN Output = (PS_IN) 0;
    
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

    //Input.Position.x += inInstansID % 100;
    //Input.Position.z += inInstansID / 100;

    Output.Position = mul(Input.Position, wvp);

    Input.Normal.w = 0.0;
    Output.Normal = mul(Input.Normal, World);

    Output.TexCoord = Input.TexCoord;
    
    Output.Diffuse = Input.Diffuse;
    
    Output.WPos = mul(Input.Position, World);
    
    wvp = mul(World, ShadowView);
    wvp = mul(wvp, ShadowProjection);
    Output.ShadowMapPos = mul(Input.Position, wvp);
    
    return Output;
}