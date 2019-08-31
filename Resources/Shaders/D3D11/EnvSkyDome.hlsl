/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


cbuffer cb_EnvVS
{
    float4x4 gModel;
    float4x4 gModelInvTrans;
    float4x4 gWVP;
};

cbuffer cb_SkyDomePS
{
    float3   gFogColor;
    float    gTime;
    float3   gSkyColor;
    float    gEnergyFieldIntensity;
};

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);

struct VSOutput
{
    float4 posH : SV_Position;
    float2 tex : TEXCOORD0;
    float3 pos : TEXCOORD1;
};


VSOutput VS(float3 posL : POSITION0,
            float2 tex : TEXCOORD0)
{
    VSOutput vsOutput;
    vsOutput.posH = mul(float4(posL, 1.0f), gModel);
    vsOutput.pos.xyz = vsOutput.posH.xyz;
    vsOutput.posH = mul(vsOutput.posH, gWVP);
    vsOutput.tex = tex;
    return vsOutput;
}

float4 PS(VSOutput input) : SV_Target
{
    float alpha = (input.pos.y + 300.0) / 1500.0f;

    float3 clr = lerp(gFogColor, gSkyColor, alpha);

    float2 water = t2.Sample(s2, (input.tex * float2(2.5, 2.5) - gTime * 0.5 * float2(0.01, 0.11))).rg;

    float2 newTex = water * 0.03f + input.tex;

    float skyDomeEnergyAlpha = t3.Sample(s3, input.tex.xy + gTime * float2(0.03, -0.1)).r;


    float param = 0.7 * gEnergyFieldIntensity;
    param *= (1.0 - max(0, (input.pos.y - 1030)) / 100);
    param *= (min(1, abs(input.pos.y + 300) / 300));

    float skyDomeEnergy = t1.Sample(s1, (newTex.yx + float2(gTime * -0.04, 0.0)) * float2(1.1, 4.0)).a;
    skyDomeEnergy *= 2;
    skyDomeEnergy *= skyDomeEnergy;
    skyDomeEnergy *= param;

    return float4(lerp(t0.Sample(s0, input.tex).rgb * clr, clr, skyDomeEnergyAlpha * skyDomeEnergy * skyDomeEnergy * skyDomeEnergy), 1.0);
}

