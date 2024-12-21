/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#include "Common.hlsl"

cbuffer cb_CartoonVS
{
    float4x4 gCamInvPMatrix;
};

cbuffer cb_CartoonPS
{
    float3   gCharPos;
    float    gSilhouetteIntensity;
    float3   gSilhouetteColor;
    float    gFogRange;
    float3   gFogColor;
    float    gFogStart;
    float2   gSilhouetteRadiusOpacity;
    float2   gTexCoordDXDY;
};

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);
Texture2D t5 : register(t5);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s5 : register(s5);

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

    OutputVS vsOut = (OutputVS)0;
    vsOut.posH = float4(pos, 1.0f);
    vsOut.tex = tex;
    vsOut.posQuad = float3(p.xy, 1.0f);

    return vsOut;
}

//-----------------------------------------------------------------------------------
float ComputeEdgeIntensity(Texture2D tex, SamplerState sam, float2 screenCoords)
{
    float depth = tex.Sample(sam, screenCoords.xy).r;
    float A = tex.Sample(sam, screenCoords.xy + float2(-gTexCoordDXDY.x, gTexCoordDXDY.y)).r;
    float B = tex.Sample(sam, screenCoords.xy + float2(0.0f, gTexCoordDXDY.y)).r;
    float C = tex.Sample(sam, screenCoords.xy + float2(gTexCoordDXDY.x, gTexCoordDXDY.y)).r;
    float D = tex.Sample(sam, screenCoords.xy + float2(-gTexCoordDXDY.x, 0.0f)).r;
    float E = tex.Sample(sam, screenCoords.xy + float2(gTexCoordDXDY.x, 0.0f)).r;
    float F = tex.Sample(sam, screenCoords.xy + float2(-gTexCoordDXDY.x, -gTexCoordDXDY.y)).r;
    float G = tex.Sample(sam, screenCoords.xy + float2(0.0f, -gTexCoordDXDY.y)).r;
    float H = tex.Sample(sam, screenCoords.xy + float2(gTexCoordDXDY.x, -gTexCoordDXDY.y)).r;
    
    float a = abs(A - depth);
    float b = abs(B - depth);
    float c = abs(C - depth);
    float d = abs(D - depth);
    float e = abs(E - depth);
    float f = abs(F - depth);
    float g = abs(G - depth);
    float h = abs(H - depth);

    float strength = 1.0 / 8.0 * (a + 2 * b + c + 2 * d + 2 * e + f + 2 * g + h);

    return saturate(strength / 0.00001f);
}

//-----------------------------------------------------------------------------------
void DoEdgeShading(const in float2 screenPos, 
                   const in float2 viewPos, 
                   const in float distanceThreshold, 
                   inout float3 color)
{
    float edgeIntensity = ComputeEdgeIntensity(t1, s1, screenPos);
    
    float dist = length(viewPos - gCharPos.xy);
    
    if (dist < distanceThreshold)
    {
        dist = 1 - saturate(dist / distanceThreshold);
        color += gSilhouetteColor * edgeIntensity * dist * gSilhouetteRadiusOpacity.y;
    }

    color = lerp(color.rgb, gSilhouetteColor + (1.0f - edgeIntensity) * color.rgb, edgeIntensity * gSilhouetteIntensity);
}

//-----------------------------------------------------------------------------------
void DoSilhouetteShading(const in float2 screenCoords, 
                         const in float angleThreshold, 
                         const in float lengthThreshold, 
                         inout float3 color)
{
    float3 viewSurfaceNormal = t2.Sample(s2, screenCoords).rgb;

    float cosAngle = dot(viewSurfaceNormal, float3(0, 0, -1.0f));
    if (cosAngle < angleThreshold && length(viewSurfaceNormal) > lengthThreshold)
    {
        float3 c = color.rgb;
        color = lerp(c * float3(0.25, 0.25, 0.25), c, 1 - gSilhouetteIntensity);
    }

}

//-----------------------------------------------------------------------------------
void DoFogShading(const in float linearDepth, 
                  inout float3 color)
{
    color.rgb = lerp(color.rgb, gFogColor, saturate((linearDepth - gFogStart) / gFogRange));
}

//-----------------------------------------------------------------------------------
float4 PS(OutputVS input) : SV_Target
{	
    float3 color = t0.Sample(s0, input.tex).rgb;
    
    float linearDepth = ComputeLinearDepth(t1, s1, input.tex);

    float3 viewSurfacePos = input.posQuad * linearDepth;

    // Edge shading
    DoEdgeShading(input.tex, viewSurfacePos.xy, 300.0f, color);

    // Silhouette shading
    DoSilhouetteShading(input.tex, 0.65f, 0.2f, color);

    // Fog shading
    DoFogShading(linearDepth, color);

    return float4(color, 1.0f);
}

