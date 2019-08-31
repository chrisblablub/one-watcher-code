/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_LightRaysPS
{
    float3 gLightRaysColor;
    float  gLightRaysDecay;
    float2 gLightRaysOrigin;
    float  gLightRaysIllumDecay;
    bool   gLightRaysOcclusionMask;
};

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

static const int NUM_SAMPLES = 100;

float4 PS(OutputVS input) : SV_Target
{
    float3 color = t0.Sample(s0, input.tex).rgb;

    float2 sunProj = gLightRaysOrigin;

    float2 deltaTexCoords = input.tex - sunProj.xy;

    float len = length(deltaTexCoords);
    if (len > 0.7)
        return float4(0, 0, 0, 1);

    float maxDistance = 0.15;

    //int numSamples = length(deltaTexCoords) / stepSize;
    //deltaTexCoords = normalize(deltaTexCoords) * maxDistance;
    //deltaTexCoords *= len / length(deltaTexCoords);

    deltaTexCoords *= 1.0f / NUM_SAMPLES;

    float3 accumColor = float3(0, 0, 0);

    float illumDecay = gLightRaysIllumDecay;


    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        input.tex -= deltaTexCoords;

        float4 sample = t0.Sample(s0, input.tex).rgba;

        if (sample.a < 0.9)
        {
            accumColor = sample.rgb * illumDecay + accumColor;
        }

        illumDecay *= gLightRaysDecay;
    }

    color.rgb = accumColor.rgb * gLightRaysColor;

    return float4(color, 1.0);
}
