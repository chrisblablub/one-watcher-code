/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#include "ParticleCommon.hlsl"


Texture2D t5 : register(t5);

sampler s5 : register(s5);

VSParticleDistort VS(float4 color : COLOR0,
                    float4 pos : POSITION0,
                    float4 binormal : BINORMAL0,
                    float4 tangent : TANGENT0,
                    float2 texCoords : TEXCOORD0,
                    float blending : TEXCOORD1,
                    float distortion : TEXCOORD2)
{
    VSParticleDistort vsOut = (VSParticleDistort) 0;
    vsOut.posH = mul(pos, gVP);
    vsOut.color = color;
    vsOut.blending = blending;
    vsOut.tex = texCoords;
    vsOut.pos = pos.xyz;
    vsOut.binormal = binormal.xyz;
    vsOut.tangent = tangent.xyz;
    vsOut.normal = cross(binormal.xyz, tangent.xyz);
    vsOut.distort = distortion;
    return vsOut;
}
float4 PS(VSParticleDistort input, float4 screenPos : SV_Position) : SV_Target
{
    
    float3x3 TBN;
    TBN[0] = input.tangent;
    TBN[1] = input.binormal;
    TBN[2] = input.normal;

    float3x3 tangentToWorld = TBN;
    float4 c = t6.Sample(s6, input.tex.xy).xyzw;
    c.xy = pow(c.xy, 1.0 / 2.2);


    input.normal = float3(2.0f, 2.0f, 2.0f) *  c.xyz - float3(1.0f, 1.0f, 1.0f);
    input.normal = normalize(input.normal);
    //input.normal = mul(input.normal, tangentToWorld).xyz;

    // Compute actual displacement vec
    input.normal = normalize(input.normal) * 1;
    
    // Compute displaced position
    float4 pos = float4(input.pos, 1.0f) + float4(input.normal, 0) * input.distort * input.color.a;

    // Project displaced pos to screen
    float4 projPos = mul(pos, gVP);
    projPos.xyz /= projPos.w;

    // Setup displaced screen coords
    float2 displacedScreenCoords = projPos.xy * float2(0.5f, -0.5) + float2(0.5, 0.5);

    // Sample current screen color
    float4 distortedSourceColor = t5.Sample(s5, displacedScreenCoords).rgba;

    distortedSourceColor.rgb *= input.color.rgb;

    distortedSourceColor.a = c.a;

    return distortedSourceColor;
}