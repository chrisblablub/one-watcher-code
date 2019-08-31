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

cbuffer cb_HaloPS
{
    float gTime;
    float gHaloIntensityLeft;
    float gHaloIntensityRight;
};

Texture2D t1 : register(t1);
Texture2D t2 : register(t2);

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
    float2 texCoords = input.tex;


    float3 normal = input.normal;
    float3 pos = input.pos;
    float direction = -1.0;

    if (pos.z < 0.0)
        direction = 1.0;

    float2 water = t2.Sample(s2, (texCoords - gTime * 0.3 * float2(0.1, 0.1))).rg;


    float2 texCoordsDistorted = texCoords * 1.0 + water * 0.07f;


    float4 skyDomeEnergy = t1.Sample(s1, 2 * (texCoordsDistorted.yx - gTime * float2(0.05, 0) * direction));

    skyDomeEnergy.a = pow(skyDomeEnergy.a, 2.5) * 2.5;
    skyDomeEnergy.rgb *= skyDomeEnergy.a;

    float param = 0.5 * abs(pos.z) / 890;

    skyDomeEnergy *= param;

    float x2 = (length(pos) - 500) / 500.0f;

    if (pos.z < 0.0f)
        skyDomeEnergy.rgba *= float4(3.0, 0.3, 0, x2 * x2 * x2 * x2 * 2);
    else
        skyDomeEnergy.rgba *= float4(0.0, 1.0, 5, x2 * x2 * x2 * x2 * 1.3);

    

    if (pos.z > 0.0f)
    {
        float haloParam = 1 - gHaloIntensityLeft;
        float thres = haloParam  * 0.9;
        float thres2 = haloParam  * 0.1;
        float val = abs(pos.z) / 900;

        if (val < thres)
            skyDomeEnergy.a = 0;
        else if (val < haloParam)
            skyDomeEnergy.a *= (val - thres) / thres2;

        skyDomeEnergy.a = saturate(skyDomeEnergy.a);
        skyDomeEnergy.a *= (1 - haloParam);
    }
    else
    {
        float haloParam = 1 - gHaloIntensityRight;
        float thres = haloParam  * 0.9;
        float thres2 = haloParam  * 0.1;
        float val = abs(pos.z) / 900;

        if (val < thres)
            skyDomeEnergy.a = 0;
        else if (val < haloParam)
            skyDomeEnergy.a *= (val - thres) / thres2;

        skyDomeEnergy.a = saturate(skyDomeEnergy.a);
        skyDomeEnergy.a *= (1 - haloParam);
    }

    skyDomeEnergy.rgb *= 1.35;
    
    
    return skyDomeEnergy.rgba;
}

