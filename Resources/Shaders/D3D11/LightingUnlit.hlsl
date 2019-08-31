/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#define SHADOW_EPS_DIRECTIONAL 0.005f
#define SHADOW_EPS 0.0005f

cbuffer cb_LightingUnlitVS
{
    float4x4 gCamInvPMatrix;
}

cbuffer cb_LightingUnlitPS
{
    float2 gInvScreenSize;
};


Texture2D t0 : register(t0); // colors
Texture2D t1 : register(t1); // normals
Texture2D t3 : register(t3); // depths

sampler colorSampler : register(s0);
sampler normalSampler : register(s1);
sampler specularSampler : register(s2);
sampler depthSampler : register(s3);
sampler shadowMapSampler01 : register(s4);

static const float ZFAR = 10000.0f;
static const float ZNEAR = 1.0f;



float ComputeLinearDepth(float normalizedDepth)
{
    float ProjectionA = ZFAR / (ZFAR - ZNEAR);
    float ProjectionB = (-ZFAR * ZNEAR) / (ZFAR - ZNEAR);
    return (ProjectionB / (normalizedDepth - ProjectionA));
}

struct VSOutputDirectional
{
    float4 posH : SV_Position;
    float3 pos : TEXCOORD0;
};


VSOutputDirectional VS(float3 pos : POSITION0)
{
    VSOutputDirectional outVS = (VSOutputDirectional)0;

    outVS.posH = float4(pos, 1.0f);

    float3 p = mul(float4(pos, 1.0f), gCamInvPMatrix).xyz;
        outVS.pos = float3(p.xy / p.z, 1.0f);
    //outVS.pos = pos;
    return outVS;
}


void GetCoordsNormalDepthDiffuseSpecular(float2 vpos, out float2 texCoords, out float4 normalSpecular, out float depth, out float3 diffuse)
{
    texCoords = vpos * gInvScreenSize;
    normalSpecular = t1.Sample(normalSampler, texCoords).rgba;
    depth = t3.Sample(depthSampler, texCoords).r;
    diffuse = t0.Sample(colorSampler, texCoords).rgb;
}


float4 PS(VSOutputDirectional input, float4 tex0 : SV_Position) : SV_Target0
{
    float2 texCoords;
    float4 viewNormalSpecular;
    float3 diffuseMtrl;
    float  depth;
    GetCoordsNormalDepthDiffuseSpecular(tex0.xy, texCoords, viewNormalSpecular, depth, diffuseMtrl);

    //if (depth >= 0.9999)
    //    discard;

    return float4(diffuseMtrl, 1.0f);
}

