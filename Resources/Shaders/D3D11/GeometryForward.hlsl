/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#include "NormalMap.hlsl"
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

sampler s0 : register(s0);
sampler s1 : register(s1);


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
};


VSOutputMesh VS(float3 pos : POSITION0,
                float3 normal : NORMAL0,
                float3 biNormal : BINORMAL0,
                float3 tangent : TANGENT0,
                float2 tex0 : TEXCOORD0)
{
    VSOutputMesh outVS = (VSOutputMesh)0;

    float4 posL = float4(pos, 1.0f);

    outVS.posH = mul(posL, mul((gModel), (gViewProjection)));
    outVS.pos = mul(posL, (gModel)).xyz;
    outVS.depth = outVS.posH.zw;
    outVS.normal = normal;
    outVS.tangent = tangent;
    outVS.biNormal = biNormal;
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

    input.normal = normalize(mul(input.normal, gView));

    float diffFactor = max(dot(input.normal, float3(0.0f, 0.0f, -1.0f)), 0.0f);

    float4 diff = float4(gDiffuseMtrl.rgb * diffFactor, 1);

    if (gHasTexture)
    {
        diff *= t0.Sample(s0, input.tex.xy).rgba;
    }

    PSOutputTerrain outPS;
    outPS.RT1 = float4(gDiffuseMtrl.rgb, 1);
    return outPS;
}

