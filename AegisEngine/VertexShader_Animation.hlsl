//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer ConstantBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}

// �V���h�E�}�b�v�p�̃}�g���b�N�X
cbuffer ShadowBuffer : register( b1 )
{
    matrix ShadowView;
    matrix ShadowProjection;
}

// �}�e���A���o�b�t�@
struct MATERIAL
{
    float4  Ambient;
    float4  Diffuse;
    float4  Specular;
    float4  Emission;
    float   Shininess;
    float3  Dummy; //16bit���E�p
};

cbuffer MaterialBuffer : register( b3 )
{
    MATERIAL Material;
}


cbuffer BoneBuffer : register( b5 )
{
    //matrix      BoneMatrix[2]           : packoffset(c0);
    //matrix      BoneMatrixDmmy[253]     : packoffset(c8);
    //matrix      EndBoneMatrix           : packoffset(c1020);
    
    // c0 �� c1 4�o�C�g
    // c0 �� c4 16�o�C�g
    // 4 * 256(�{�[���̍ő吔)
    
    float4x4 BoneMatrix[256];
}



//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void main(  in float4 inPosition         : POSITION0,
		    in float4 inNormal           : NORMAL0,
		    in float4 inDiffuse          : COLOR0,
		    in float2 inTexCoord         : TEXCOORD0,
            in uint4  inIneces           : BLENDINDICE,
            in float4 inWeight           : BLENDWEIGHT,

            out float4 outPosition       : POSITION0,
		    out float4 outNormal         : NORMAL0,
		    out float2 outTexCoord       : TEXCOORD0,
		    out float4 outDiffuse        : COLOR0,
		    out float4 outWPos           : POSITION1,
            out float4 outShadowMapPos   : POSITION_SHADOWMAP )
{
    float4x4 BoneTransform = BoneMatrix[inIneces[0]] * inWeight[0]
                           + BoneMatrix[inIneces[1]] * inWeight[1]
                           + BoneMatrix[inIneces[2]] * inWeight[2]
                           + BoneMatrix[inIneces[3]] * inWeight[3];

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    outPosition = mul(inPosition, BoneTransform);
    outPosition = mul(outPosition, wvp);

    inNormal.w = 0.0;
    outNormal = mul(inNormal, BoneTransform);
    outNormal = mul(outNormal, World);

    outTexCoord = inTexCoord;
	
    float4 worldNormal, normal;
    normal = float4(outNormal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);

    outDiffuse = inDiffuse;
    
    outWPos = mul(inPosition, BoneTransform);
    outWPos = mul(outPosition, World);
}