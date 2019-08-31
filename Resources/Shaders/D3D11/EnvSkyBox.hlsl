/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_SkyBoxVS
{
    float4x4 gWVP;
}

TextureCube t0 : register(t0);

sampler s0 : register(s0);

struct VSOutput
{
    float4 posH : SV_Position;
    float3 pos : TEXCOORD1;
};

VSOutput VS(float3 posL : POSITION0,
            float2 tex : TEXCOORD0)
{
    VSOutput vsOutput;
    vsOutput.posH = mul(float4(posL, 1.0f), gWVP);
    vsOutput.pos.xyz = posL.xyz;
    return vsOutput;
}


float4 PS(VSOutput input) : SV_Target
{
    return t0.Sample(s0, input.pos);
}

