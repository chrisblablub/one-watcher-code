/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#include "GeometryUtility.hlsl"

cbuffer cb_GeometryPS
{
    float4   gDiffuseMtrl;
    float4   gEmissiveMtrl;
    float4   gEnvMtrl;
    bool     gHasNormalMap;
    bool     gHasSpecularMap;
    bool     gHasEmissiveMap;
    bool     gHasTexture;
    bool     gHasEnvMap;
};

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);
Texture2D t4 : register(t4);
Texture2D t5 : register(t5);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);
sampler s5 : register(s5);

struct VSOutputMesh
{
    float4 posH : SV_Position;
    float2 depth : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 biNormal : TEXCOORD3;
    float3 tangent : TEXCOORD4;
    float3 pos : TEXCOORD5;
};

struct PSOutputTerrain
{
    float4 RT1 : SV_Target0;
    float4 RT2 : SV_Target1;
    float4 RT3 : SV_Target2;
    float3 RT4 : SV_Target3;
};


VSOutputMesh VS(float3 pos : POSITION0,
                float3 normal : NORMAL0,
                float3 biNormal : BINORMAL0,
                float3 tangent : TANGENT0,
                float2 tex0 : TEXCOORD0)
{
    VSOutputMesh outVS = (VSOutputMesh) 0;

    float4 posL = float4(pos.xyz, 1.0f);

    outVS.posH = mul(posL, mul((gModel), (gViewProjection)));
    outVS.pos = mul(posL, (gModel)).xyz;
    outVS.depth = outVS.posH.zw;
    outVS.normal = normal.xyz;
    outVS.tangent = tangent.xyz;
    outVS.biNormal = biNormal.xyz;
    outVS.tex = tex0;

    return outVS;
}

float4 PS(VSOutputMesh input) : SV_Target
{
    float3x3 model;
    model[0] = gModel[0].xyz;
    model[1] = gModel[1].xyz;
    model[2] = gModel[2].xyz;

    if (gHasNormalMap)
    {
        float3x3 TBN;
        TBN[0] = input.tangent;
        TBN[1] = input.biNormal;
        TBN[2] = input.normal;

        float3x3 tangentToWorld = mul(TBN, model);

        input.normal = 2.0f * t1.Sample(s1, input.tex.xy).xyz - float3(1.0f, 1.0f, 1.0f);
        input.normal = mul(input.normal, tangentToWorld).xyz;
    }
    else
    {
        input.normal = mul(input.normal, model).xyz;
    }

    float3 worldNormal = input.normal;

    input.normal = normalize(mul(float4(input.normal, 0.0f), gView).xyz);

    float3 viewRay = normalize(-gView[3].xyz - input.pos.xyz);
    float3 refractedViewRay = refract(viewRay, worldNormal, 1.0);

    // Compute displaced position
    float4 pos = float4(input.pos, 1.0f) + float4(refractedViewRay, 0) * 4.0;

    // Project displaced pos to screen
    float4 projPos = mul(pos, gViewProjection);
    projPos.xyz /= projPos.w;

    // Setup displaced screen coords
    float2 displacedScreenCoords = projPos.xy * float2(0.5f, -0.5) + float2(0.5, 0.5);

    float3 displacedSourceColor = t5.Sample(s5, displacedScreenCoords.xy).rgb;


    float currentDepth = input.depth.x / input.depth.y;
    float sampledDepth = t4.Sample(s4, displacedScreenCoords.xy).x;



    float4 diff = float4(gDiffuseMtrl.rgb, 1);

    if (gHasTexture)
    {
        diff *= t0.Sample(s0, input.tex.xy).rgba;
    }

    float3 spec = float3(1.0, 1.0, 1.0);

    if (gHasSpecularMap)
    {
        spec.rgb = t2.Sample(s2, input.tex.xy).rgb;
            //diff.a = 1.0;
    }

    float3 emissive = float3(0.0, 0.0, 0.0);

    if (gHasEmissiveMap)
    {
        emissive.rgb = t3.Sample(s3, input.tex.xy).rgb;
        diff.rgb *= (emissive.rgb * gEmissiveMtrl);
    }


    //if ((currentDepth + 0.000005) > sampledDepth)
    //{
    //    float4 pos = float4(input.pos, 1.0f);

    //    // Project displaced pos to screen
    //    float4 projPos = mul(pos, gViewProjection);
    //    projPos.xyz /= projPos.w;

    //    // Setup displaced screen coords
    //    float2 screenCoords = projPos.xy * float2(0.5f, -0.5) + float2(0.5, 0.5);

    //    displacedSourceColor.rgb = t5.Sample(s5, screenCoords).rgb;
    //    displacedSourceColor.rgb = float3(1, 0, 0);
    //}

    diff.rgb = lerp(diff.rgb, displacedSourceColor.rgb, 1.0 - diff.a * diff.a * 0.25);

    return float4(diff.rgb, 1);
}

