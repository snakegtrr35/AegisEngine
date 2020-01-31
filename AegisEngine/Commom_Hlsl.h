#ifndef COMMOM_HLSL_H
#define COMMOM_HLSL_H

struct VS_IN {
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
	uint   InstansID	: SV_InstanceID;
};

struct VS_IN_ANIMA {
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
	uint4  Ineces		: BLENDINDICE;
	float4 Weight		: BLENDWEIGHT;
};

struct VS_IN_INSTANCING {
	float4 Position			: POSITION0;
	float4 Normal			: NORMAL0;
	float4 Diffuse			: COLOR0;
	float2 TexCoord			: TEXCOORD0;
	matrix World_Matrix		: MATRIX;
	uint   InstansID		: SV_InstanceID;
};



struct PS_IN
{
	float4 Position		: SV_POSITION;
	float4 Normal		: NORMAL0;
	float2 TexCoord		: TEXCOORD0;
	float4 Diffuse		: COLOR0;
	float4 WPos			: POSITION1;
	float4 ShadowMapPos : POSITION_SHADOWMAP;
};

struct PS_IN_SHADOW {
    float4 Position	: SV_POSITION;
    float4 Pos		: POSITION1;
};

#endif //! COMMOM_HLSL_H