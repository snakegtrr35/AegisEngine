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


//=============================================================================
// 頂点シェーダ
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