/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#define SHADOW_EPS_DIRECTIONAL 0.005f
#define SHADOW_EPS 0.0005f

cbuffer cb_LightingDirectionalVS
{
    float4x4 gCamInvPMatrix;
}

cbuffer cb_LightingDirectionalPS
{
    float4 gLightDiffuse;
    float4 gLightSpecular;
    float4 gLightDirection;
    float2 gInvScreenSize;
    bool   gSpecular;
};

Texture2D t0 : register(t0); // colors
Texture2D t1 : register(t1); // normals
Texture2D t2 : register(t2); // normals
Texture2D t3 : register(t3); // depths
TextureCube t4 : register(t4); // depths

sampler colorSampler : register(s0);
sampler normalSampler : register(s1);
sampler specularSampler : register(s2);
sampler depthSampler : register(s3);
sampler s4 : register(s4);


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
    return outVS;
}


void GetCoordsNormalDepthDiffuseSpecular(float2 vpos, out float2 texCoords, out float4 normalSpecular, out float depth, out float3 diffuse)
{
    texCoords = vpos * gInvScreenSize;
    normalSpecular = t1.Sample(normalSampler, texCoords).rgba;
    depth = t3.Sample(depthSampler, texCoords).r;
    diffuse = t0.Sample(colorSampler, texCoords).rgb;
}


float3 ComputeSpecular(float clr, float3 H, float3 N)
{
    float4 specular = float4(clr, clr, clr, 150);
    float specFactor = pow(max(dot(H, N), 0.0f), specular.a);

    return float3(specFactor*(specular.rgb*gLightSpecular.rgb));
}

float4 PS(VSOutputDirectional input, float4 tex0 : SV_Position) : SV_Target0
{
    float2 texCoords;
    float4 viewNormalSpecular;
    float3 diffuseMtrl;
    float  depth;
    GetCoordsNormalDepthDiffuseSpecular(tex0.xy, texCoords, viewNormalSpecular, depth, diffuseMtrl);
    //viewNormalSpecular.a = 1;

    if (depth <= 0)
        discard;

    float linearDepth = ComputeLinearDepth(depth);

    

    float3 viewSurfacePos = input.pos;
        float3 viewRay = input.pos;

    float viewZDist = dot(float3(0, 0, 1), viewRay);

    viewSurfacePos = viewRay * linearDepth / viewZDist;

    float3 viewLight = normalize(-gLightDirection.xyz);


    float diffuseFactor = max(dot(viewLight, viewNormalSpecular.xyz), 0.0f);


    float3 color = diffuseFactor*(diffuseMtrl*gLightDiffuse.rgb);

    if (gSpecular)
    {
        float3 viewH = normalize(viewLight - viewRay);
            color += ComputeSpecular(viewNormalSpecular.a, viewH, viewNormalSpecular.xyz);
    }

    return float4(color, 1.0f);
}
