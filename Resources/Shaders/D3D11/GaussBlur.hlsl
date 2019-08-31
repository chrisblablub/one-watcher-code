/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_Samples
{
    float4 gSampleOffsets[16];
    float4 gSampleWeights[16];
};


Texture2D t0 : register(t0);

sampler s0 : register(s0);

struct VSOutput
{
    float4 posH : SV_Position;
    float2 tex : TEXCOORD1;
};

VSOutput VS(float3 pos : POSITION0,
            float2 tex : TEXCOORD0)
{
    VSOutput vsOut = (VSOutput) 0;
    vsOut.posH = float4(pos, 1.0f);
    vsOut.tex = tex;

    return vsOut;
}


float4 PS(VSOutput input) : SV_Target
{
    //return float4(input.tex, 0, 1);
    //return float4(t0.Sample(s0, input.tex).rgb, 1);

    float4 sample = 0.0f;

    for (int i = 0; i < 13; i++)
    {
        sample += gSampleWeights[i] * t0.Sample(s0, input.tex + gSampleOffsets[i].xy).rgba;
    }
    
    return float4(sample.rgb, 1);
}
