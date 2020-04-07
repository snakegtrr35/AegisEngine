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


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
PS_IN main(VS_IN Input)
{
    PS_IN Output = (PS_IN) 0;
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Output.Position = mul(Input.Position, wvp);

    Output.Diffuse = Input.Diffuse;
    
    Output.WPos = Input.Position;

    return Output;
}