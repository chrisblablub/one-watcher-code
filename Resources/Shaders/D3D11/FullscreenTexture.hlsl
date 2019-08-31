/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

Texture2D t0 : register(t0);

sampler s0 : register(s0);

struct VSOutput
{
    float4 posH : SV_Position;
    float2 texCoords : TEXCOORD0;
};

VSOutput VS(float3 vPos : POSITION0, float2 tex0 : TEXCOORD0)
{
    VSOutput vsOutput;
    vsOutput.posH = float4(vPos, 1);
    vsOutput.texCoords = float2(tex0);
    return vsOutput;
}

float4 PS(VSOutput input) : SV_Target0
{
    float4 color = t0.Sample(s0, input.texCoords.xy);

    return float4(color.rgb, 1);
}
