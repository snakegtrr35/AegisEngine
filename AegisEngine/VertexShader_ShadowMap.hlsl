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

struct VS_IN {
   float4 Position    : POSITION0;
   float4 Normal      : NORMAL0;
   float4 Diffuse     : COLOR0;
   float2 TexCoord    : TEXCOORD0;
};

struct PS_IN {
    float4 Position     : SV_POSITION;
    float4 Pos          : POSITION1;
};


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
PS_IN main( VS_IN Input)
{
    PS_IN Output = (PS_IN) 0;
    
    matrix wvp;

    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    Input.Position.w = 1.0;
    Output.Position = mul(Input.Position, wvp);
    
    Output.Pos = Output.Position;
    
    return Output;
}