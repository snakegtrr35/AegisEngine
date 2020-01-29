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


struct VS_IN {
    float4 Position   : POSITION0;
    float4 Normal     : NORMAL0;
    float4 Diffuse    : COLOR0;
    float2 TexCoord   : TEXCOORD0;
    uint   InstansID  : SV_InstanceID;
};

struct PS_IN {
    float4 Position      : SV_POSITION;
    float4 Normal        : NORMAL0;
    float2 TexCoord      : TEXCOORD0;
    float4 Diffuse       : COLOR0;
    float4 WPos          : POSITION1;
    float4 ShadowMapPos  : POSITION_SHADOWMAP;
};

//=============================================================================
// 頂点シェーダ
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