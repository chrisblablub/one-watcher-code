/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/
#include "NormalMap.hlsl"
#include "GeometryUtility.hlsl"
#include "LightingUtility.hlsl"

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

cbuffer cb_GeometryWetPS 
{ 
    float4 gTime; 
}

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);
TextureCube t4 : register(t4);
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

float3 GetMeshWaterNormalSpec(float3 texCoords, float3 normal, float3x3 tangentToWorld)
{
    texCoords.y += gTime.x * 0.2f;

    float3 waterNormal01 = t5.Sample(s5, texCoords.xy).xyz;

    waterNormal01 = 2.0f * waterNormal01 - float3(1.0f, 1.0f, 1.0f);
    waterNormal01 = mul(waterNormal01, tangentToWorld).xyz;

    return float3(waterNormal01.rgb);

    /* float3 worldRight = float3(1, 0, 0);
    worldRight = normalize(mul(worldRight, tangentToWorld).xyz);

    float3 waterNormal02 = tex2D(s2, waterTexCoords.zy).xyz;
    waterNormal02 = 2.0f * waterNormal02 - 1.0f;
    waterNormal02 = normalize(mul(waterNormal02, tangentToWorld).xyz);

    float3 waterNormal03 = tex2D(s2, waterTexCoords.xz).xyz;
    waterNormal03 = 2.0f * waterNormal03 - 1.0f;
    waterNormal03 = normalize(mul(waterNormal03, tangentToWorld).xyz);

    float3 waterNormal = lerp(waterNormal01, waterNormal02, abs(worldRight.z));
    waterNormal = lerp(waterNormal, waterNormal03, abs(normal.y));


    return float3(normalize(normal +  waterNormal * 4));*/
}

VSOutputMesh VS(float3 pos : POSITION0,
                float3 normal : NORMAL0,
                float3 biNormal : BINORMAL0,
                float3 tangent : TANGENT0,
                float2 tex0 : TEXCOORD0)
{
    VSOutputMesh outVS = (VSOutputMesh)0;

    float4 posL = float4(pos.xyz, 1.0f);

    outVS.posH = mul(posL, mul(gModel, gViewProjection));
    outVS.pos = mul(posL, mul(gModel, gView)).xyz;
    outVS.depth = outVS.posH.zw;
    outVS.normal = normal.xyz;
    outVS.tangent = tangent.xyz;
    outVS.biNormal = biNormal.xyz;
    outVS.tex = tex0;

    return outVS;
}

PSOutputTerrain PS(VSOutputMesh input)
{
    float3x3 model;
    model[0] = gModel[0].xyz;
    model[1] = gModel[1].xyz;
    model[2] = gModel[2].xyz;

    if (gHasNormalMap)
    {
        
        float4x4 tangentToModel;
        tangentToModel[0] = float4(input.tangent, 0);
        tangentToModel[1] = float4(input.biNormal, 0);
        tangentToModel[2] = float4(input.normal, 0);
        tangentToModel[3] = float4(input.pos, 0);

        float4x4 modelToWorld;
        modelToWorld[0] = float4(gModel[0].xyz, 0);
        modelToWorld[1] = float4(gModel[1].xyz, 0);
        modelToWorld[2] = float4(gModel[2].xyz, 0);
        modelToWorld[3] = float4(gModel[3].xyz, 0);

        float3x3 modelToTangentOrient = transpose(float3x3(tangentToModel[0].xyz, tangentToModel[1].xyz, tangentToModel[2].xyz));
        float3x3 worldToModelOrient = transpose(float3x3(modelToWorld[0].xyz, modelToWorld[1].xyz, modelToWorld[2].xyz));

        float4x4 modelToTangent;
        modelToTangent[0] = float4(modelToTangentOrient[0], 0);
        modelToTangent[1] = float4(modelToTangentOrient[1], 0);
        modelToTangent[2] = float4(modelToTangentOrient[2], 0);
        modelToTangent[3] = float4(-input.pos, 0);

        float4x4 worldToModel;
        worldToModel[0] = float4(worldToModelOrient[0], 0);
        worldToModel[1] = float4(worldToModelOrient[1], 0);
        worldToModel[2] = float4(worldToModelOrient[2], 0);
        worldToModel[3] = float4(-gModel[3].xyz, 0);


        float4 worldTexCoords = mul(float4(input.tex.xy, 0, 1), tangentToModel);
        worldTexCoords = mul(worldTexCoords, modelToWorld);


        float4 screenCoords = mul(worldTexCoords, worldToModel);
        screenCoords = mul(screenCoords, modelToTangent);

        input.normal.xy = screenCoords.yy;
        //input.normal = SampleNormalMap(model, input.normal, input.biNormal, input.tangent, t5, s5, screenCoords.xy);

        //float3 worldScreenCoords = mul(float3(input.tex, 0.0f), tangentToWorld).xyz;

        // worldScreenCoords.y += gTime * 0.01f;

        // Project texcoords back to tangent space
        //float3 tangentScreenCoords = mul(worldScreenCoords, worldToTangent).xyz;

        //diff.rgba = float4(worldScreenCoords.xy, 0, 1);

        //input.normal = GetMeshWaterNormalSpec(input.pos.xyz / 400.0f, input.normal, tangentToWorld).xyz;

        
       // input.normal /= 2;
    }
    else
    {
        input.normal = mul(input.normal, model).xyz;
    }

    input.normal = normalize(mul(float4(input.normal, 0.0f), gView).xyz);

    float4 diff = float4(gDiffuseMtrl.rgb, 1);

    if (gHasTexture)
    {
        //diff *= t0.Sample(s0, input.tex.xy).rgba;
        diff.rgb = input.normal;
    }

    float3 spec = float3(1.0, 1.0, 1.0);

    if (gHasSpecularMap)
    {
        spec.rgb = t2.Sample(s2, input.tex.xy).rgb;
    }

    float3 emissive = float3(0.0, 0.0, 0.0);

    if (gHasEmissiveMap)
    {
        emissive.rgb = t3.Sample(s3, input.tex.xy).rgb;
    }

    if (gHasEnvMap)
    {
        float3 viewSurfacePos = input.pos;
        float3 viewSurfaceRay = normalize(-viewSurfacePos);
        float3 viewReflectedRay = reflect(viewSurfaceRay, input.normal.xyz);

        float3 reflectedLight = t4.Sample(s4, viewReflectedRay).rgb * 1;

        diff.rgb = lerp(diff.rgb, reflectedLight * gEnvMtrl.rgb, gEnvMtrl.a);
    }

    PSOutputTerrain outPS;
    outPS.RT1 = float4(diff);
    outPS.RT2 = float4(input.normal.rgb, 1);
    outPS.RT3 = float4(input.depth.x / input.depth.y, 0, 0, 0);
    outPS.RT4 = float3(emissive.rgb *  gEmissiveMtrl.rgb);

    return outPS;
}