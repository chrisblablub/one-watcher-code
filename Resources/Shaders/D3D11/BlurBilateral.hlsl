/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_gCamInvVMatrix
{
    float4x4 gCamInvVMatrix;
}
cbuffer cb_gCamPMatrix
{
    float4x4 gCamPMatrix;
}


cbuffer cb_gCamInvPMatrix
{
    float4x4 gCamInvPMatrix;
}
cbuffer cb_gCharPos
{
    float3 gCharPos;
}
cbuffer cb_gSilhouetteRadiusOpacity
{
    float2 gSilhouetteRadiusOpacity;
}
cbuffer cb_gColorGradingMatrix
{
    float4x4 gColorGradingMatrix;
}
cbuffer cb_gPosterizationAmount
{
    float gPosterizationAmount;
}
cbuffer cb_gScreenSize
{
    float2 gScreenSize;
}
cbuffer cb_gSilhouetteIntensity
{
    float gSilhouetteIntensity;
}
cbuffer cb_gSilhouetteColor
{
    float3 gSilhouetteColor;
}
cbuffer cb_gFogColor
{
    float3 gFogColor;
}
cbuffer cb_gFogRange
{
    float gFogRange;
}
cbuffer cb_gFogStart
{
    float gFogStart;
}
cbuffer cb_gDistortionScaling
{
    float2 gDistortionScaling;
}
cbuffer cb_gDistortionDirection
{
    float2 gDistortionDirection;
}
cbuffer cb_gDistortionColor
{
    float3 gDistortionColor;
}
cbuffer cb_gDistortionOffset
{
    float gDistortionOffset;
}
cbuffer cb_gDistortionAmount
{
    float gDistortionAmount;
}
cbuffer cb_gTexCoordDXDY
{
    float2 gTexCoordDXDY;
}

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);
Texture2D t4 : register(t4);
Texture2D t5 : register(t5);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);
sampler s5 : register(s5);

struct OutputVS
{
    float4 posH : SV_Position;
    float3 posQuad : TEXCOORD1;
    float2 tex : TEXCOORD0;
};

static const float ZFAR = 10000.0f;
static const float ZNEAR = 1.0f;
static const float POSTERIZATION_GAMMA = 0.1f;
static const float POSTERIZATION_NUM_COLORS = 30.0f;

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


float ComputeLinearDepth(float normalizedDepth)
{
    float ProjectionA = ZFAR / (ZFAR - ZNEAR);
    float ProjectionB = (-ZFAR * ZNEAR) / (ZFAR - ZNEAR);
    return (ProjectionB / (normalizedDepth - ProjectionA));
}

float GetColorBlur(float2 uv, float intensity)
{
    float3x3 gradientMatrix;

    // Initializing X direction gradient kernel
    //
    gradientMatrix[0][0] = -1;
    gradientMatrix[0][1] = 0;
    gradientMatrix[0][2] = 1;
    gradientMatrix[1][0] = -2;
    gradientMatrix[1][1] = 0;
    gradientMatrix[1][2] = 2;
    gradientMatrix[2][0] = -1;
    gradientMatrix[2][1] = 0;
    gradientMatrix[2][2] = 1;
    // Looping to findout change in X diretion
    float sumX = 0.0f, sumY = 0.0f;
    float2 step = float2(1.0f / (gScreenSize.x * 1.0f), 1.0f / (gScreenSize.y * 1.0f));
    for (int I = -1; I <= 1; I++)
    {
        for (int J = -1; J <= 1; J++)
        {
            sumX += (gradientMatrix[I + 1][J + 1] * t0.Sample(s0, uv + step * float2(I, J)).rgb) * intensity;

        }
    }

    // Initializing Y direction gradient kernel
    //
    gradientMatrix[0][0] = 1;
    gradientMatrix[0][1] = 2;
    gradientMatrix[0][2] = 1;
    gradientMatrix[1][0] = 0;
    gradientMatrix[1][1] = 0;
    gradientMatrix[1][2] = 0;
    gradientMatrix[2][0] = -1;
    gradientMatrix[2][1] = -2;
    gradientMatrix[2][2] = -1;
    for (int I = -1; I <= 1; I++)
    {
        for (int J = -1; J <= 1; J++)
        {
            sumY += (gradientMatrix[I + 1][J + 1] * t0.Sample(s0, uv + step * float2(I, J)).rgb) * intensity;
        }
    }
    float amount = saturate(sqrt(sumX * sumX + sumY * sumY));

    return amount;
}

float3 GetBilateralBlur(float2 uv, float2 noise, float3 color)
{
    float2 texCoordOffset = float2(0.5 / gScreenSize.x, 0.5 / gScreenSize.y) * 0.5;

    float3 bilateralBlur = float3(0, 0, 0);

    float weights = 0.0;

    //color = log(color);
    for (int x = -8; x <= 8; ++x)
    {
        for (int y = -8; y <= 8; ++y)
        {
            float2 uvOffset = noise.xy * 0.0;

            float3 neighbor = log(t0.Sample(s0, uv + uvOffset + texCoordOffset * float2(x, y)).rgb);

            float w = (1.0 / length(neighbor - color));
            weights += w;
            bilateralBlur += w * neighbor;
        }
    }

    bilateralBlur /= weights;
    bilateralBlur = exp(bilateralBlur + float3(1, 1, 1) * 0.01);

    return bilateralBlur;
}

float3 GetPosterizedColor(float3 color)
{
    float3 c1 = color.rgb;
    c1 = pow(c1, float3(POSTERIZATION_GAMMA, POSTERIZATION_GAMMA, POSTERIZATION_GAMMA));
    c1 = c1 * POSTERIZATION_NUM_COLORS;
    c1 = floor(c1);
    c1 = c1 / POSTERIZATION_NUM_COLORS;
    c1 = pow(c1, 1.0 / POSTERIZATION_GAMMA * float3(1.0, 1.0, 1.0));

    color.rgb = lerp(color.rgb, c1, 1);
    return color;
}

float3 ComputeViewSurfacePos(float2 uv, float3 posQuad, Texture2D tex, sampler sam)
{
    float depth = tex.Sample(sam, uv).r;
    float linearDepth = ComputeLinearDepth(depth);
    float3 viewSurfacePos = posQuad * linearDepth;
    return viewSurfacePos;
}

float4 PS(OutputVS input) : SV_Target
{
    //return float4(1, 0, 0, 1);
    float3 viewSurfacePos = ComputeViewSurfacePos(input.tex, input.posQuad, t1, s1);

    float2 uvNoise = mul(float4(viewSurfacePos, 1.0), gCamInvVMatrix).xy / 850;

    float3 noise = t3.Sample(s3, uvNoise).rgb;

    float3 color = (t0.Sample(s0, input.tex).rgb);
  
    float3 bilateralBlur = GetBilateralBlur(input.tex, noise.xy, (color));
    return float4(bilateralBlur, 1);
}

