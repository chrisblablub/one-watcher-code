/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


cbuffer cb_PolygonVS
{
    float4x4 gVP;
}

struct OutputVS
{
    float4 posH : SV_Position;
    float3 clr : TEXCOORD0;
};


OutputVS VS(float3 pos : POSITION0, float3 clr : TEXCOORD0)
{
    OutputVS vsOut = (OutputVS) 0;

    vsOut.posH = mul(float4(pos, 1.0f), gVP);
    vsOut.clr = clr;

    return vsOut;
}

float4 PS(OutputVS input) : SV_Target
{
    return float4(input.clr, 1.0f);
}