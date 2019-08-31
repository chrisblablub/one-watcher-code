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


cbuffer cb_SkyPlanePS
{
    float3 gSkyColor;
    float  gTime;
    float  gCloudIntensity;
};

Texture2D t0 : register(t0);

sampler s0 : register(s0);

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
    float2 tex2 = input.tex * 2 - float2(1, 1);

    float alpha = 1;
    if (input.tex.y > 0.75)
        alpha = 1 - (input.tex.y - 0.75) / 0.25;

    float2 tex01 = input.tex * 10.0;
        tex01 += gTime * float2(0.0, -0.05);
    float2 tex02 = input.tex * 5.0;
        tex02 += gTime * float2(0.03, -0.05);

    float4 diffuse = t0.Sample(s0, tex01).rgba;
        diffuse += t0.Sample(s0, tex02).rgba;
    diffuse *= 0.5f;

    diffuse.a *= (alpha * gCloudIntensity);
    diffuse.rgb *= gSkyColor;


    return diffuse;
}
