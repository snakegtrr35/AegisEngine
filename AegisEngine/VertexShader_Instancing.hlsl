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


// �V���h�E�}�b�v�p�̃}�g���b�N�X
cbuffer ShadowBuffer : register(b1)
{
    matrix ShadowView;
    matrix ShadowProjection;
}

PS_IN main(VS_IN_INSTANCING Input)
{
    PS_IN Output = (PS_IN) 0;
    
    matrix wvp;
    wvp = mul(Input.World_Matrix, View);
    wvp = mul(wvp, Projection);

    Output.Position = mul(Input.Position, wvp);

    Input.Normal.w = 0.0;
    Output.Normal = mul(Input.Normal, Input.World_Matrix);
    
    Output.TexCoord = Input.TexCoord;
    
    Output.Diffuse = Input.Diffuse;
    
    Output.WPos = mul(Input.Position, Input.World_Matrix);
    
    wvp = mul(Input.World_Matrix, ShadowView);
    wvp = mul(wvp, ShadowProjection);
    Output.ShadowMapPos = mul(Input.Position, wvp);
    
    return Output;
}