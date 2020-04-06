#include    "Commom_Hlsl.hlsli"

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
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
    
    // c0 → c1 4バイト
    // c0 → c4 16バイト
    // 4 * 256(ボーンの最大数)
}

//=============================================================================
// 頂点シェーダ
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