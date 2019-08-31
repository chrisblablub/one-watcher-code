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

cbuffer cb_GeometryCharPS
{
    float4 gCharMtrl;
    float2 gCharParams;
};

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);

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
    VSOutputMesh outVS = (VSOutputMesh)0;

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

float4 PS(VSOutputMesh input) : SV_Target0
{
    float3x3 model;
    model[0] = gModel[0].xyz;
    model[1] = gModel[1].xyz;
    model[2] = gModel[2].xyz;

    /*if (gHasNormalMap)
    {
        input.normal = SampleNormalMap(model, input.normal, input.biNormal, input.tangent, t1, s1, input.tex.xy);
    }
    else*/
    {
        input.normal = mul(input.normal, model).xyz;
    }

    input.normal = normalize(mul(float4(input.normal, 0.0f), gView).xyz);

    float3 worldNormal = input.normal;

    input.normal = normalize(mul(float4(input.normal, 0.0f), gView).xyz);

    float4 diffColor = float4(gDiffuseMtrl.rgb, 1);

    if (gHasTexture > 0)
    {
        diffColor *= t0.Sample(s0, input.tex.xy).rgba;
    }

    float diffFactor = abs(dot(float3(-1, 0, 0), worldNormal));
    float4 fadeColor = float4(gCharMtrl.rgb * 1, 1);
    float4 finalColor = lerp(fadeColor, diffColor, pow(1-diffFactor, 0.75));
    

    return float4(finalColor.rgb, 1);
}

