/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

Texture2D t0 : register(t0);

sampler s0 : register(s0);

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
    return t0.Sample(s0, input.tex);
}


