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


cbuffer BoneBuffer : register(b6)
{
    matrix BoneMatrix[2] : packoffset(c0);
    matrix BoneMatrixDmmy[253] : packoffset(c8);
    matrix EndBoneMatrix : packoffset(c1020);
    
    // c0 �� c1 4�o�C�g
    // c0 �� c4 16�o�C�g
    // 4 * 256(�{�[���̍ő吔)
}

//=============================================================================
// ���_�V�F�[�_
//=============================================================================
PS_IN_SHADOW main(VS_IN_ANIMA Input)
{
    PS_IN_SHADOW Output = (PS_IN_SHADOW) 0;
    
    matrix BoneTransform = BoneMatrix[Input.Ineces[0]] * Input.Weight[0]
                         + BoneMatrix[Input.Ineces[1]] * Input.Weight[1]
                         + BoneMatrix[Input.Ineces[2]] * Input.Weight[2]
                         + BoneMatrix[Input.Ineces[3]] * Input.Weight[3];
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Input.Position.w = 1.0;
    Output.Position = mul(Input.Position, BoneTransform);
    Output.Position = mul(Output.Position, wvp);
    
    Output.Pos = Output.Position;
    
    return Output;
}