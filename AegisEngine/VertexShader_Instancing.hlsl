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


// シャドウマップ用のマトリックス
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