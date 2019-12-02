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


// �}�e���A���o�b�t�@
struct MATERIAL
{
    float4      Ambient;
    float4      Diffuse;
    float4      Specular;
    float4      Emission;
    float       Shininess;
    float3      Dummy; //16bit���E�p
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}


// ���C�g�o�b�t�@
struct LIGHT
{
    float4      Direction;
    float4      Position;
    float4      Diffuse;
    float4      Ambient;
    float4      Specular;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}


cbuffer CameraBuffer : register(b5)
{
    float4 CameraPos;
}


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D       g_Texture : register( t0 );
Texture2D       g_ShadowMap : register(t1);     // �V���h�E�}�b�v

SamplerState	g_SamplerState : register( s0 );
SamplerState    g_ShadowSamplerState : register(s1);        // �V���h�E�}�b�v�p�̃T���v���[


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float2 inTexCoord     : TEXCOORD0,
		   in float4 inDiffuse      : COLOR0,
		   in float4 outWPos        : TEXCOORD1,
           in float4 inShadowMapPos : POSITION_SHADOWMAP,
           
		   out float4 outDiffuse	: SV_Target )
{
    /*{
        float4 worldNormal, normal;
        normal = float4(inNormal.xyz, 0.0);
        worldNormal = mul(normal, World);
        worldNormal = normalize(worldNormal);

        float light = 0.5 - 0.5 * dot(normalize(Light.Direction.xyz), inNormal.xyz);

        float4 Color;

        Color = inDiffuse * Material.Diffuse * light * Light.Diffuse;
        Color += inDiffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0f);
        float4 diffuse = inDiffuse * float4(Material.Diffuse.rgb * light * Light.Diffuse.rgb, 1.0f);
        float4 ambient = inDiffuse * Material.Ambient * Light.Ambient;
        Color.a = inDiffuse.a * Material.Diffuse.a;
    }*/


    float3 L = normalize(-Light.Position.xyz - outWPos.xyz);
    float3 N = normalize(inNormal);
    float LN = 0.5 - 0.5 * dot(L, N);
 
    // �����ˌ�
    float4 ambient = inDiffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0f);
    // �g�U���ˌ�
    float4 diffuse = inDiffuse * float4(Material.Diffuse.rgb * LN * Light.Diffuse.rgb, 1.0f);

    float4 color = (float4)0.0;

    //
    // ���[���h��Ԃł̃��C�g�̈ʒu
    float3 lightPosWorld = -Light.Position.xyz;

    // ���ݎQ�Ƃ��Ă��镨�̕\�ʂ��烉�C�g�ւ̕��������߂�
    float3 toLightDir = normalize(lightPosWorld.xyz - outWPos.xyz);

    // ���ݎQ�Ƃ��Ă��镨�̕\�ʂ��王���ւ̕������v�Z
    //float3 toCameraDir = normalize(CameraPos.xyz - outWPos.xyz);//

    // ���˃x�N�g�����n�[�t�x�N�g���ŋߎ�
    //float3 halfDir = normalize(toLightDir + toCameraDir);//
    float3 toCameraDir = normalize(CameraPos.xyz - outWPos.xyz);
    float3 R = -toCameraDir + 2.0f * dot(inNormal.xyz, toCameraDir) * inNormal.xyz;
    
    // �X�y�L�������������߂�
    //float specular = pow( saturate( dot(halfDir, inNormal.xyz) ), Light.Specular.w );//
    //float specular = pow(saturate(dot(halfDir, inNormal.xyz)), Light.Specular.w);//
    float specular = pow(saturate(dot(R, inNormal.xyz)), Light.Specular.w);

    // �V���h�E�}�b�v
    float shadow;
    float2 ShadowTexCoord;
    ShadowTexCoord.x = inShadowMapPos.x / inShadowMapPos.w / 2.0f + 0.5f;
    ShadowTexCoord.y = -inShadowMapPos.y / inShadowMapPos.w / 2.0f + 0.5f;
    
    //if ((saturate(ShadowTexCoord.x) == ShadowTexCoord.x) && (saturate(ShadowTexCoord.y) == ShadowTexCoord.y))
    //{
        float depthValue = g_ShadowMap.Sample(g_ShadowSamplerState, ShadowTexCoord).r;

        float lightDepthValue = inShadowMapPos.z / inShadowMapPos.w;

        lightDepthValue = lightDepthValue - 0.005f;

        if (lightDepthValue < depthValue)
        {
            shadow = 1.0f;
            //shadow = 0.5f;
        }
        else
        {
            //shadow = 1.0f;
            shadow = 0.5f;
        }
   // }
    
    color = (diffuse * g_Texture.Sample(g_SamplerState, inTexCoord)) + specular
            + (ambient * g_Texture.Sample(g_SamplerState, inTexCoord));

    color.rgb *= shadow;
    
   // color = float4(.0f, 0.f, 0.f, 1.0f);
    
    outDiffuse = color;

    //if (outDiffuse.a <= 0.0)
    //    discard;
}