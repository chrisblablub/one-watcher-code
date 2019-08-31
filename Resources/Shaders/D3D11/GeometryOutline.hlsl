/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#include "GeometryUtility.hlsl"

cbuffer cb_GeometryOutline { float4 gOutlineColor; }

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

    float4 posL = float4(pos.xyz, 1.0f);

    posL.xyz += 1.5 * normal;

    outVS.posH = mul(posL, mul((gModel), (gViewProjection)));
    outVS.pos = mul(posL, (gModel)).xyz;
    outVS.depth = outVS.posH.zw;
    outVS.normal = normal.xyz;
    outVS.tangent = tangent.xyz;
    outVS.biNormal = biNormal.xyz;
    outVS.tex = tex0;

    return outVS;
}

PSOutputTerrain PS(VSOutputMesh input)
{
    PSOutputTerrain outPS;
    outPS.RT1 = float4(gOutlineColor.rgb, 1.0f);
    return outPS;

}

