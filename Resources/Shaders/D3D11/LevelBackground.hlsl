/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

//uniform extern float4x4 gVP;
//uniform extern float3 gCharPos;
//uniform extern float3 gPosition;
//uniform extern float2 gAlpha;
//uniform extern float2 gTexCoordOffset;
//uniform extern float2 gInvScreenSize;
//uniform extern float gTime;

cbuffer cb_LevelBackgroundVS
{
    float4x4 gVP;
    float4 gPosition;
    float4 gColor;
};

texture2D t0 : register(t0);

sampler s0 : register(s5);
sampler s5 : register(s0);
sampler s1 : register(s1);

struct OutputVS
{
    float4 posH : SV_Position;
    float2 tex : TEXCOORD0;
    float3 pos : TEXCOORD1;
};

OutputVS VS(float3 pos : POSITION0,
            float2 tex : TEXCOORD0)
{
    OutputVS vsOut = (OutputVS)0;

    vsOut.pos = pos * float3(1.1, 1.1, 1) * gColor.a + gPosition.xyz;

    vsOut.posH = mul(float4(vsOut.pos, 1.0), gVP);
    vsOut.tex = tex;

    return vsOut;
}

float4 PS(OutputVS input) : SV_Target0
{
    float4 color = t0.Sample(s0, input.tex);

    return color * gColor;
    //float3 pos = input.pos;
    //float2 tex = input.tex;

    //float dist = length(pos.xy - gCharPos.xy);
    //float alpha = 1 - min(1, dist / 150);

    //float4 cellColor = tex2D(s0, tex);
    //if (gTexCoordOffset.y > 0.1)
    //    cellColor.a *= 1;

    //float4 sourceColor = tex2D(s5, input.posH.xy/*t * gInvScreenSize*/);


    //float4 c3 = sourceColor.rgba;
    //c3.rgb *= float3(0, 0.5, 1);
    //c3.a = cellColor.a * 1.4;


    //float4 c1 = float4(cellColor.rgb, cellColor.a * 1.0);
    //c3 = tex2D(s0, tex + float2(0, 0.5));
    //c1.rgb *= 1 * float3(0, 0.5, 1) * 4;
    //c3.rgb *= 1 * float3(0, 0.5, 1) * 4;
    //c3.rgb *= c3.a;
    //float alpha2 = 1 - min(1, dist / 550);
    //alpha2 = 1;
    //if (dist > 100)
    //    alpha2 = 1 - min(1, (dist - 100) / 550);

    //c1 = lerp(c1, c3, 0);

    //float4 noise = tex2D(s1, pos.xy / 400 + gTime * 0.1);
    //noise += tex2D(s1, pos.xy / 400 + gTime * 0.15);
    //noise.r *= 1;
    //noise /= 2;
    //noise *= float4(0, 0.5, 1, c1.a * gAlpha.x);

    //noise.rgb += c3.rgb * alpha * 0.25;
    //return noise * 1 * alpha2;
    //c1.rgb = lerp(c1.rgb, noise.rgb, 0.75);
    //float4 final = c1 * gAlpha.x * alpha;

    //return final;
}
