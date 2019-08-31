/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#include "ParticleCommon.hlsl"

VSParticleColor VS( float4 color : COLOR0,
                    float4 pos : POSITION0,
                    float2 texCoords : TEXCOORD0,
                    float blending : TEXCOORD1)
{
    VSParticleColor vsOut = (VSParticleColor) 0;
    vsOut.posH = mul(pos, gVP);
    vsOut.color = color;
    vsOut.blending = blending;
    vsOut.tex = texCoords;
    return vsOut;
}

float4 PS(VSParticleColor input) : SV_Target
{
    float4 sampledColor = t6.Sample(s6, input.tex.xy);

    sampledColor.rgba *= input.color.rgba;

    sampledColor.rgb *= sampledColor.a;

    sampledColor.a = lerp(sampledColor.a, 0, input.blending);

    return sampledColor;
}