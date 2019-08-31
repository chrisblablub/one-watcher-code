/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#include "NormalMap.hlsl"
#include "TerrainUtility.hlsl"

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
};

struct PSOutput
{
    float4 RT1 : SV_Target0;
};


VSOutputTerrain VS(float3 pos : POSITION0,
                   float3 biNormal : BINORMAL0,
                   float3 tangent : TANGENT0,
                   float2 tex0 : TEXCOORD0,
                   float4 color : COLOR0)
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
    return outVS;
}

PSOutput PS(VSOutputTerrain input)
{
    PSOutput outPS;

    outPS.RT1 = float4(0, 0, 0, 1);

    return outPS;
}
