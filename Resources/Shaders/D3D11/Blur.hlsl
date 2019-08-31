/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/
static const float Weights[4] = { 70.0f / 256.0f, 56.0f / 256.0f, 28.0f / 256.0f, 8.0f / 256.0f };

float3 BlurPixels(float3 p1, float3 p2, float3 p3, float3 p4, float3 p5, float3 p6, float3 p7)
{
    return Weights[0] * p4 + Weights[1] * (p3 + p5) + Weights[2] * (p2 + p6) + Weights[3] * (p1 + p7);
}

float3 SampleAndBlurPixels(Texture2D t0, sampler s0, float2 texCoords, float2 pixelSize)
{
    float2 texOffsetHalf = pixelSize * 0.5f;
    float2 texCoords0 = texCoords - texOffsetHalf;
    float2 texCoords1 = texCoords + texOffsetHalf;

    //float3 color = t0.Sample(s0, texCoords).rgb;
    //return BlurPixels(color, color, color, color, color, color, color);

    return BlurPixels(
        t0.Sample(s0, texCoords0 + pixelSize * -3.0).rgb,
        t0.Sample(s0, texCoords0 + pixelSize * -2.0).rgb,
        t0.Sample(s0, texCoords0 + pixelSize * -1.0).rgb,
        t0.Sample(s0, texCoords).rgb,
        t0.Sample(s0, texCoords1 + pixelSize *  1.0).rgb,
        t0.Sample(s0, texCoords1 + pixelSize *  2.0).rgb,
        t0.Sample(s0, texCoords1 + pixelSize *  3.0).rgb
    );
}