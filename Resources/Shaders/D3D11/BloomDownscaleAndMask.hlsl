/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);

sampler s0 : register(s0);
sampler s1 : register(s1);

struct OutputVS
{
    float4 posH : SV_Position;
    float2 tex : TEXCOORD0;
};


OutputVS VS(float3 pos : POSITION0,
            float2 tex : TEXCOORD0)
{
    OutputVS vsOut = (OutputVS)0;
    vsOut.posH = float4(pos, 1.0f);
    vsOut.tex = tex;
    return vsOut;
}

float4 PS(OutputVS input) : SV_Target
{
    return float4(t0.Sample(s0, input.tex).rgb, t1.Sample(s1, input.tex).a);
}


