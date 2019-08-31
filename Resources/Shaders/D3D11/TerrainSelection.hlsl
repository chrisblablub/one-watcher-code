/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#include "NormalMap.hlsl"
#include "TerrainUtility.hlsl"

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);

struct VSOutputTerrain
{
    float4 posH : SV_Position;
    float2 depth : TEXCOORD0;
    float2 tex : TEXCOORD1;
    float3 biNormal : TEXCOORD2;
    float4 tangent : TEXCOORD3;
    float3 texVertex : TEXCOORD4;
    float painterAlpha : TEXCOORD5;
    float3 color : COLOR0;
    float3 colorLighting : COLOR1;
};

struct PSOutput
{
    float4 RT1 : SV_Target0;
    float4 RT2 : SV_Target1;
    float4 RT3 : SV_Target2;
    float3 RT4 : SV_Target3;
};


VSOutputTerrain VS(float3 pos : POSITION0,
                   float3 tangent : TANGENT0,
                   float3 biNormal : BINORMAL0,
                   float2 tex0 : TEXCOORD0,
                   float4 color : COLOR0,
                   float4 colorLighting : COLOR1)
{
    VSOutputTerrain outVS = (VSOutputTerrain)0;

    outVS.texVertex = pos.xyz;
    outVS.posH = mul(float4(outVS.texVertex, 1.0f), gViewProjection);
    outVS.depth.xy = outVS.posH.zw;
    outVS.tex = tex0;
    outVS.biNormal = biNormal;
    outVS.tangent.xyz = tangent;
    outVS.color = color.rgb;
    outVS.painterAlpha = color.a;
    outVS.colorLighting = colorLighting.rgb * colorLighting.a;

    return outVS;
}

PSOutput PS(VSOutputTerrain input)
{
    PSOutput outPS;

    float3 normal = float3(0.0f, 0.0f, -1.0f);

    float3x3 tangentToWorld;
    tangentToWorld[0] = input.tangent.xyz;
    tangentToWorld[1] = input.biNormal;
    tangentToWorld[2] = normal;

    float3 normalNMap = t1.Sample(s1, input.tex.xy).xyz;
    normalNMap = 2.0f * normalNMap - float3(1.0f, 1.0f, 1.0f);
    normal = mul(normalNMap, tangentToWorld).xyz;
    normal = normalize(mul(normal, gView));

    float4 diff = float4(input.color, 1.0f) * t0.Sample(s0, input.tex.xy);

    //diff.rgb = lerp(diff.xyz, gTerrainPainterFirstColor * input.color, input.painterAlpha * (1.0f - input.texVertex.z / 300.0f));

    if (diff.a <= 0.1)
        discard;

    float3 spec = float3(0.0, 0.0, 0.0);

    float3 emissive = float3(0.0, 0.0, 0.0);

    //diff.rgb = lerp(diff.rgb, float3(1, 0, 0), 0.5);
    diff.rgb = 0.6 * float3(1, 0, 0) + 0.6 * diff.rgb;

    outPS.RT1 = float4(diff.rgba);
    outPS.RT2 = float4(0.0f, 0.0f, -1.0f, dot(input.color, float3(0.2125f, 0.7154f, 0.0721f)) * spec.r);//float4(normal, 0.2 * dot(input.color, float3(0.2125f, 0.7154f, 0.0721f)));
    outPS.RT3 = float4(input.depth.x / input.depth.y, 0, 0, 0);
    outPS.RT4 = float3(emissive.rgb);

    return outPS;
}
