/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_BloomDarkpass
{
    float gBloomScale;
    bool gLightRays;
}

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t4 : register(t4);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s4 : register(s4);

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
    
    float4 clrSource = t0.Sample(s0, input.tex);

    float3 clrBright_1_4_blurred = t1.Sample(s1, input.tex).rgb;
    float3 clrBright_1_16_blurred = t2.Sample(s2, input.tex).rgb;

    float3 clrBright = clrBright_1_4_blurred + clrBright_1_16_blurred;
    clrBright /= 4;

    clrSource.rgb += (clrBright.rgb * gBloomScale);

    if (gLightRays)
    {
        clrSource.rgb += t4.Sample(s4, input.tex).rgb;
    }

    return clrSource;
}
