#include    "Commom_Hlsl.h"

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
PS_IN_SHADOW main(VS_IN Input)
{
    PS_IN_SHADOW Output = (PS_IN_SHADOW) 0;
    
    matrix wvp;

    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    Input.Position.w = 1.0;
    Output.Position = mul(Input.Position, wvp);
    
    Output.Pos = Output.Position;
    
    return Output;
}