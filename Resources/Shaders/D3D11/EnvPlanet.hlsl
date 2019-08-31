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

cbuffer cb_PlanetPS
{
    float3 gFogColor;
    float  gTime;
    float3 gEnergyFieldColor;
    float  gCloudIntensity;
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
    // Add planet texture
    float2 water = t3.Sample(s3, input.tex * 1.3 - gTime * 0.0005).rg;

    float2 newTex = input.tex + water * 0.25f;
    float4 skyDomeEnergy = t1.Sample(s1, newTex * 1.5);

    skyDomeEnergy.rgb *= (gEnergyFieldColor * skyDomeEnergy.a * 0.7);

    float3 color = t0.Sample(s0, input.tex).rgb * 0.5 + skyDomeEnergy.rgb;

        // Add clouds
        float4 clouds = t2.Sample(s2, input.tex * float2(3, 3) + gTime * float2(0.0, 0.05));
        clouds.a *= (clouds.a * gCloudIntensity);

    clouds.rgb *= clouds.a;

    color += clouds.rgb;
    color *= gFogColor;

    // Lerp planet color and fog
    float alpha = clamp((input.pos.z - 250) / 2000.0, 0, 1);
    alpha = pow(alpha, 2);
    color = lerp(color.rgb, gFogColor, alpha * 0.9);

    return float4(color, 1);
}
