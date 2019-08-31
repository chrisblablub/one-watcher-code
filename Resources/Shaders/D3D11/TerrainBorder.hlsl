/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_TerrainBorderVS
{
    float4x4 gVP;
}
cbuffer cb_TerrainBorderPS
{
    float4 gDiffuseMtrl;

    bool gUsePrecomputedLighting;
}

Texture2D t0 : register(t0);

sampler s0 : register(s0);


struct OutputVS
{
    float4 ndcPos : SV_Position;
    float2 tex : TEXCOORD0;
    float2 depth : TEXCOORD1;
    float3 worldPos : TEXCOORD2;
    float3 colorDiffLighting : TEXCOORD4;
};

struct PSOutput
{
    float4 RT1 : SV_Target0;
    float4 RT2 : SV_Target1;
    float4 RT3 : SV_Target2;
    float3 RT4 : SV_Target3;
};

OutputVS VS(float4 pos : POSITION0,
            float4 colorDiffMaterial : TEXCOORD0,
            float4 colorDiffLighting : TEXCOORD1,
            float2 tex : TEXCOORD2)
{
    OutputVS vsOut = (OutputVS) 0;

    vsOut.ndcPos = mul(pos, gVP);
    vsOut.worldPos = pos.xyz;
    vsOut.depth = vsOut.ndcPos.zw;
    vsOut.tex = tex.xy;
    vsOut.colorDiffLighting = colorDiffLighting.rgb * colorDiffLighting.a;
    return vsOut;
}

PSOutput PS(OutputVS input)
{
    float4 borderColor = float4(gDiffuseMtrl.rgb, 1);

    borderColor = t0.Sample(s0, input.tex) * borderColor;

    float3 emissive = 0;

    if (gUsePrecomputedLighting > 0)
    {
        emissive = t0.Sample(s0, input.tex).rgb * input.colorDiffLighting;
    }

    PSOutput psOutput;
    psOutput.RT1 = float4(borderColor);
    psOutput.RT2 = float4(0, 0, -1, 0);
    psOutput.RT3 = float4(input.depth.x / input.depth.y, 0, 0, 0);
    psOutput.RT4 = float3(emissive);

    return psOutput;
}