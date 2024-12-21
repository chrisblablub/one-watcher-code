/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#include "Common.hlsl"
#include "ColorUtility.hlsl"
#include "Posterize.hlsl"

cbuffer cb_PostProcessVS
{
    float4x4 gCamInvPMatrix;
}

cbuffer cb_PostProcessPS
{
    float4x4 gColorGradingMatrix;
    float gPosterizationAmount;
}

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);
Texture2D t4 : register(t4);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);

struct OutputVS
{
    float4 posH : SV_Position;
    float3 posQuad : TEXCOORD1;
    float2 tex : TEXCOORD0;
};

//-----------------------------------------------------------------------------------
OutputVS VS(float3 pos : POSITION0,
            float2 tex : TEXCOORD0)
{
    float4 p = mul(float4(pos, 1.0f), gCamInvPMatrix).xyzw;

    OutputVS vsOut = (OutputVS) 0;
    vsOut.posH = float4(pos, 1.0f);
    vsOut.tex = tex;
    vsOut.posQuad = float3(p.xy, 1.0f);

    return vsOut;
}

//-----------------------------------------------------------------------------------
void DoPosterization(inout float3 color)
{
    if (gPosterizationAmount > 0.0)
    {
        float3 c1 = ComputePosterizedColor(color.rgb);

        color.rgb = lerp(color.rgb, c1, gPosterizationAmount);
    }
}

//-----------------------------------------------------------------------------------
void DoColorInversion(const in float intensity, inout float3 color)
{
    float3 hsl = ConvertRGB2HSL(ComputeGammarizedColor(color));

    float complementHue = ComputeInverseHue(hsl.r);

    float3 invertedColor = ConvertHSL2RGB(complementHue, hsl.g, hsl.b);

    color = lerp(color.rgb, ComputeLinearizedColor(invertedColor), intensity);
}

//-----------------------------------------------------------------------------------
float4 PS(OutputVS input) : SV_Target
{
    float3 color = t0.Sample(s0, input.tex).rgb;

    float depth = t1.Sample(s1, input.tex).r;
    float linearDepth = ComputeLinearDepth(depth);
    float3 viewSurfacePos = input.posQuad * linearDepth;

    float2 projSurfacePos = input.tex;

    // Posterization
    DoPosterization(color.rgb);

    // Color Grading
    color.rgb = mul(float4(color.rgb, 1.0), gColorGradingMatrix).rgb;

    //float3 bloom = t2.Sample(s2, input.tex).rgb;
    //float3 lightrays = t3.Sample(s3, input.tex).rgb;
    //float3 lens = t4.Sample(s4, input.tex).rgb;
    
    //color.rgb += bloom.rrr *lens;
    //color.rgb = bloom;

    // Color Inversion
    // DoColorInversion(1.0f, color.rgb);
    return float4(color, 1.0f);
}

