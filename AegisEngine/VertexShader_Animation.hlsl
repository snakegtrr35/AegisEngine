//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer ConstantBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}


// シャドウマップ用のマトリックス
cbuffer ShadowBuffer : register( b1 )
{
    matrix ShadowView;
    matrix ShadowProjection;
}


cbuffer BoneBuffer : register( b6 )
{
    matrix BoneMatrix[2]          : packoffset(c0);
    matrix BoneMatrixDmmy[253]    : packoffset(c8);
    matrix EndBoneMatrix          : packoffset(c1020);
    
    // c0 → c1 4バイト
    // c0 → c4 16バイト
    // 4 * 256(ボーンの最大数)
}


struct VS_IN {
    float4 Position     : POSITION0;
    float4 Normal       : NORMAL0;
    float4 Diffuse      : COLOR0;
    float2 TexCoord     : TEXCOORD0;
    uint4  Ineces       : BLENDINDICE;
    float4 Weight       : BLENDWEIGHT;
};

struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    float4 WPos : POSITION1;
    float4 ShadowMapPos : POSITION_SHADOWMAP;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
PS_IN main(  VS_IN Input)
{
    PS_IN Output = (PS_IN) 0;
    
    matrix BoneTransform = BoneMatrix[Input.Ineces[0]] * Input.Weight[0]
                         + BoneMatrix[Input.Ineces[1]] * Input.Weight[1]
                         + BoneMatrix[Input.Ineces[2]] * Input.Weight[2]
                         + BoneMatrix[Input.Ineces[3]] * Input.Weight[3];

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Output.Position = mul(Input.Position, BoneTransform);
    Output.Position = mul(Output.Position, wvp);

    Input.Normal.w = 0.0;
    Output.Normal = mul(Input.Normal, BoneTransform);
    Output.Normal = mul(Output.Normal, World);

    Output.TexCoord = Input.TexCoord;
	
    float4 worldNormal, normal;
    normal = float4(Output.Normal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);

    Output.Diffuse = Input.Diffuse;
    
    Output.WPos = mul(Input.Position, BoneTransform);
    Output.WPos = mul(Output.Position, World);
    
    wvp = mul(World, ShadowView);
    wvp = mul(wvp, ShadowProjection);
    Output.ShadowMapPos = mul(Input.Position, BoneTransform);
    Output.ShadowMapPos = mul(Input.Position, wvp);
    
    return Output;
}