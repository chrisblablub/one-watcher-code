/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#define SHADOW_EPS_DIRECTIONAL 0.005f
#define SHADOW_EPS 0.0005f

cbuffer cb_LightingOmniVS
{
    float4x4 gV;
    float4x4 gCamWVPMatrix;
}

cbuffer cb_LightingOmniPS
{
    float4 gLightPosition;
    float4 gLightDiffuse;
    float4 gLightSpecular;
    float2 gInvScreenSize;
    float  gLightRadius;
    bool   gSpecular;
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


struct VSOutputPointSpot
{
    float4 posH : SV_Position;
    float3 pos : TEXCOORD0;
};


VSOutputPointSpot VS(float3 pos : POSITION0)
{
    VSOutputPointSpot outVS = (VSOutputPointSpot)0;

    outVS.posH = mul(float4(pos, 1.0f), gCamWVPMatrix);

    float3 posL = mul(float4(pos, 1.0f), gV).xyz;
    outVS.pos = posL.xyz;
    outVS.pos.z = outVS.posH.w;

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
    float4 specular = float4(clr, clr, clr, 180);
        float specFactor = pow(max(dot(H, N), 0.0f), specular.a);


    return float3(specFactor*(specular.rgb*gLightSpecular.rgb));
}


float4 PS(VSOutputPointSpot input, float4 tex0 : SV_Position) : SV_Target0
{

    float2 texCoords;
    float4 viewNormalSpecular;
    float3 diffuseMtrl;
    float  depth;
    GetCoordsNormalDepthDiffuseSpecular(tex0.xy, texCoords, viewNormalSpecular, depth, diffuseMtrl);
    //viewNormalSpecular.a = 1.0;
    //return float4(depth, depth, depth, 1);

    float3 viewRay = normalize(float3(input.pos.xy / input.pos.z, 1.0f));

        float3 viewSurfacePos = viewRay * ComputeLinearDepth(depth) / viewRay.z;

        float3 viewLight = (gLightPosition.xyz - viewSurfacePos);
        float viewLightLength = length(viewLight);
    viewLight /= viewLightLength;

    float attenuation = (1.0 - (viewLightLength / gLightRadius));

    if (attenuation < 0.0)
        return float4(0, 0, 0, 0);


    float3 color = max(dot(viewNormalSpecular.xyz, viewLight), 0.0f)*(diffuseMtrl*gLightDiffuse.xyz);
    
   // return float4(color * attenuation, 1);

    if (gSpecular)
    {
        float3 viewH = normalize(viewLight - viewRay);
            color += ComputeSpecular(viewNormalSpecular.a, viewH, viewNormalSpecular.xyz);

        //color += float3(1, 0, 0);
       
    }

    return float4(color.rgb * attenuation, 1.0f);
}