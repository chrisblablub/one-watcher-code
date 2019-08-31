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

cbuffer cb_EnergyFieldPS
{
    float4   gFogColor;
    float4   gSkyColor;
    float4   gEnergyFieldColor;
    float    gTime;
};

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);

struct VSOutput
{
    float4 posH : SV_Position;
    float2 tex : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 pos : TEXCOORD2;
};

VSOutput VS(float3 posL : POSITION0,
            float3 normal : NORMAL0,
            float2 tex : TEXCOORD0)
{
    VSOutput vsOutput;

    vsOutput.posH = mul(float4(posL, 1.0f), gModel);
    vsOutput.pos.xyz = vsOutput.posH.xyz;
    vsOutput.posH = mul(vsOutput.posH, gWVP);
    vsOutput.tex = tex;
    vsOutput.normal = mul(float4(normal, 0), gModelInvTrans).xyz;

    return vsOutput;
}

float4 PS(VSOutput input) : SV_Target
{
    float alpha = 1;

    float alpha2 = 1;
    alpha2 = clamp((input.pos.y + 300.0) / 1500.0f, 0, 1);


    float3 clr = lerp(gFogColor.xyz, gSkyColor.xyz, alpha2);

    float2 water = t2.Sample(s2, (input.tex - gTime * 0.3 * float2(0.02, 0.1))).rg;

    float2 newTex2 = input.tex + water * 0.01f;
    float2 newTex = input.tex + water * 0.3f;

    float4 skyDomeEnergy = t1.Sample(s1, float2(newTex.x, newTex.y - gTime * 0.1) * float2(15, 0.8));
    skyDomeEnergy += t1.Sample(s1, float2(newTex.x, newTex.y - gTime * 0.1) * float2(30, 1.8));
    skyDomeEnergy /= 1.0;

    float4 fin = float4(t0.Sample(s0, newTex2).rgb * clr * alpha, 1);
    skyDomeEnergy.rgb *= gEnergyFieldColor.xyz;

    float param = 0.7;
    skyDomeEnergy *= param;
    if (input.pos.y > 1040)
    {
        skyDomeEnergy = lerp(skyDomeEnergy, float4(gEnergyFieldColor.xyz, 1), (input.pos.y - 1040) / 90);
    }

    float alpha4 = max(input.pos.y, 0) / 1700;

    fin.rgb = lerp(fin.rgb, skyDomeEnergy.rgb, skyDomeEnergy.a * skyDomeEnergy.a * alpha4 );

    return fin;
}
