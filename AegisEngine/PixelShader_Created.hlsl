#define LIGHT_COUNT 9

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


cbuffer CameraBuffer : register(b5)
{
    float4 CameraPos;
}


struct Light
{
    float3 Position;
    float Radius;
};


cbuffer ConstantBuffer : register(b6)
{
    Light Lights[LIGHT_COUNT];
    float4 Scale;
    float4 Bias;
}

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

Texture3D<uint> Clusters : register(t2);

void main( in float4 inPosition     : POSITION0,
		   in float4 inNormal       : NORMAL0,
		   in float2 inTexCoord     : TEXCOORD0,
		   in float4 inDiffuse      : COLOR0,
		   in float4 inWPos         : POSITION1,
           in float4 inShadowMapPos : POSITION_SHADOWMAP,
           
		   out float4 outDiffuse : SV_Target)
{
    float3 base = g_Texture.Sample(g_SamplerState, inTexCoord).rgb;
    
    float3 view_vec = normalize(CameraPos - inWPos);
    float3 refl_vec = reflect(-view_vec, inNormal.xyz);

	// Compute cluster and fetch its light mask
    int4 coord = inWPos * Scale + Bias;
    uint light_mask = Clusters.Load(coord);

    float3 total_light = 0.2f * base;
    
    while (light_mask)
    {
		// Extract a light from the mask and disable that bit
        uint i = firstbitlow(light_mask);
        light_mask &= ~(1 << i);

        const Light light = Lights[i];

		// Lighting vectors
        float3 lVec = light.Position - inWPos.xyz;
        float3 light_vec = normalize(lVec);

        float atten = saturate(1.0f - dot(lVec, lVec) * (1.0f / (light.Radius * light.Radius)));
        atten *= atten * atten;

		// Lighting
        float diffuse = saturate(dot(light_vec, inNormal.xyz));
        diffuse.r *= 1.2;
        float specular = 0.2f * pow(saturate(dot(refl_vec, light_vec)), 10.0f);

        total_light += atten * (diffuse * base + specular);
    }

    outDiffuse = float4(total_light, 1.0f);
}