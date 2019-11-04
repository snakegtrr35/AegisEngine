

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�e���A���o�b�t�@
cbuffer MaterialBuffer : register(b3)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float3 Dummy; //16bit���E�p
}



//// �}�g���N�X�o�b�t�@
//cbuffer WorldBuffer : register(b0)
//{
//    matrix World;
//}
//cbuffer ViewBuffer : register(b1)
//{
//    matrix View;
//}
//cbuffer ProjectionBuffer : register(b2)
//{
//    matrix Projection;
//}

//// �}�e���A���o�b�t�@
//struct MATERIAL
//{
//    float4 Ambient;
//    float4 Diffuse;
//    float4 Specular;
//    float4 Emission;
//    float Shininess;
//    float3 Dummy; //16bit���E�p
//};

//cbuffer MaterialBuffer : register(b3)
//{
//    MATERIAL Material;
//}


//// ���C�g�o�b�t�@
//struct LIGHT
//{
//    float4 Direction;
//    float4 Diffuse;
//    float4 Ambient;
//    //float4      Specular;
//};

//cbuffer LightBuffer : register(b4)
//{
//    LIGHT Light;
//}

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D       g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,

						 out float4 outDiffuse		: SV_Target )
{
    outDiffuse = g_Texture.Sample( g_SamplerState, inTexCoord );

    if (outDiffuse.a <= 0.0)
        discard;

    //inDiffuse = Diffuse;

	outDiffuse *= inDiffuse;
}
