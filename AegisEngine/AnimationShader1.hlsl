//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

cbuffer BoneMatrix : register(b3)
{
    matrix Bone[256];
}

// マテリアルバッファ
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float3 Dummy; //16bit境界用
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}


// ライトバッファ
struct LIGHT
{
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}

float4 main(in float4 inPosition : POSITION0,
			in float4 inNormal : NORMAL0,
			in float4 inDiffuse : COLOR0,
			in float2 inTexCoord : TEXCOORD0,
            in int4 inBlendIndecs : BLENDINDICE0,
            in float4 inWeight : BLENDWEIGHT0,

			out float4 outPosition : SV_POSITION,
			out float4 outNormal : NORMAL0,
			out float2 outTexCoord : TEXCOORD0,
			out float4 outDiffuse : COLOR0) : SV_POSITION
{
    matrix bone_matrix = Bone[inBlendIndecs[0]] * inWeight[0] * Bone[inBlendIndecs[1]] * inWeight[1] * Bone[inBlendIndecs[2]] * inWeight[2] * Bone[inBlendIndecs[3]] * inWeight[3];

    matrix wvp;
    matrix mat = mul(bone_matrix, World);
    wvp = mat;
    wvp = mul(wvp, View);
    wvp = mul(wvp, Projection);
	
    float4 temp_normal = float4(inNormal.x, inNormal.y, inNormal.z, 0.0f);
    outNormal = normalize(mul(temp_normal, mat));

    float4 worldNormal, normal;
    normal = float4(inNormal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);

    float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

    outDiffuse = inDiffuse * Material.Diffuse * light * Light.Diffuse;
    outDiffuse += inDiffuse * Material.Ambient * Light.Ambient;
    outDiffuse.a = inDiffuse.a * Material.Diffuse.a;
}