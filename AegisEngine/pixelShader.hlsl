

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
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float2 inTexCoord     : TEXCOORD0,
		   in float4 inDiffuse      : COLOR0,
		   in float4 outWPos        : TEXCOORD1,

           
		   out float4 outDiffuse	: SV_Target )
{
    //float4 worldNormal, normal;
    //normal = float4(inNormal.xyz, 0.0);
    //worldNormal = mul(normal, World);
    //worldNormal = normalize(worldNormal);

    //float light = 0.5 - 0.5 * dot(normalize(Light.Direction.xyz), inNormal.xyz);

    //float4 Color;

    //Color = inDiffuse * Material.Diffuse * light * Light.Diffuse;
    //Color += inDiffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0f);
    //float4 diffuse = inDiffuse * float4(Material.Diffuse.rgb * light * Light.Diffuse.rgb, 1.0f);
    //float4 ambient = inDiffuse * Material.Ambient * Light.Ambient;
    //Color.a = inDiffuse.a * Material.Diffuse.a;


    float3 L = normalize(-Light.Position.xyz - outWPos.xyz); // ���̕\�ʏ�̓_��������ւ̕����x�N�g��
    float3 N = normalize(inNormal); // �\�ʏ�̓_�ɂ�����@��
    float LN = 0.5 - 0.5 * dot(L, N);

    //float3 R = normalize(2 * LN * N - normalize(LightPos.xyz)); // ���������˂�������
    //float3 V = normalize(CameraPos.xyz - outWPos.xyz); // ���_�Ɍ���������
    //float RV = 0.5 - 0.5 * dot(R, V);
 
    // �����ˌ�
    float4 ambient = inDiffuse * float4(Material.Ambient.rgb * Light.Ambient.rgb, 1.0f);
    // �g�U���ˌ�
    float4 diffuse = inDiffuse * float4(Material.Diffuse.rgb * LN * Light.Diffuse.rgb, 1.0f);

    float4 color = (float4)0.0;

    //float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //�v�Z�̂��߂Ƀ��C�g�̕����𔽓]���܂��B
    //float3 lightDir =L;

    //
    //float3 R = 2.0f * N * dot(N.xyz, L) - L;
    //float3 V = normalize(CameraPos.xyz - outWPos.xyz);

    //float specPower = pow(saturate(dot(R, V)), Light.Specular.w);
    
    //

    //
    // ���[���h��Ԃł̃��C�g�̈ʒu
    float3 lightPosWorld = -Light.Position.xyz;

    // ���ݎQ�Ƃ��Ă��镨�̕\�ʂ��烉�C�g�ւ̕��������߂�
    float3 toLightDir = normalize(lightPosWorld.xyz - outWPos.xyz);

    // ���ݎQ�Ƃ��Ă��镨�̕\�ʂ��王���ւ̕������v�Z
    float3 toCameraDir = normalize(CameraPos.xyz - outWPos.xyz);

    // ���˃x�N�g�����n�[�t�x�N�g���ŋߎ�
    float3 halfDir = normalize(toLightDir + toCameraDir);
    //float3 toCameraDir = normalize(CameraPos.xyz - outWPos.xyz);
    //float3 R = -toCameraDir + 2.0f * dot(inNormal.xyz, toCameraDir) * inNormal.xyz;
    
    // �X�y�L�������������߂�
    //float specular = pow( saturate( dot(halfDir, inNormal.xyz) ), Light.Specular.w );
    float specular = pow(saturate(dot(halfDir, inNormal.xyz)), Light.Specular.w);

    //
 

    ////���̃s�N�Z���̌��ʂ��v�Z���܂��B
    //float lightIntensity = saturate(dot(inNormal.xyz, lightDir));
    ////lightIntensity = 1.0f;
    //float3 view = normalize(CameraPos.xyz - outWPos.xyz);
    //if (lightIntensity > 0.0f)
    //{
       
    //    color += (diffuse * lightIntensity);
    //    color = saturate(color);

    //    float3 reflection = normalize(50.0f * lightIntensity * inNormal.xyz - lightDir);
    //    //float i = pow(saturate(dot(h, v)), Light.Specular.w);
    //    specular = pow(saturate(dot(reflection, view)), Light.Specular.w);
    //    // ���ʔ��ˌ�
    //    //float4 specular = saturate(60.0 * pow(RV, 20.0) * (Light.Specular * Material.Specular) );
    //    //specular = i * (inDiffuse * float4(Light.Specular.rgb * Material.Specular.rgb, 1.0f));
    //}

    color = (diffuse * g_Texture.Sample(g_SamplerState, inTexCoord)) + specular
            + (ambient * g_Texture.Sample(g_SamplerState, inTexCoord));

    //color = saturate(color + specular);

    //outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);

    outDiffuse = color;

    if (outDiffuse.a <= 0.0)
        discard;


    //outDiffuse += specular;
}
