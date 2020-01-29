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

struct VS_IN {
    float4 Position   : POSITION0;
    float4 Normal     : NORMAL0;
    float4 Diffuse    : COLOR0;
    float2 TexCoord   : TEXCOORD0;
    uint4  Ineces     : BLENDINDICE;
    float4 Weight     : BLENDWEIGHT;
};

struct PS_IN {
    float4 Position     : SV_POSITION;
    float4 Pos          : POSITION1;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
PS_IN main(VS_IN Input)
{
    PS_IN Output = (PS_IN) 0;
    
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