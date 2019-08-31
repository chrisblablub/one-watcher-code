/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */



float ComputeLinearDepth(float normalizedDepth)
{
    float ProjectionA = ZFAR / (ZFAR - ZNEAR);
    float ProjectionB = (-ZFAR * ZNEAR) / (ZFAR - ZNEAR);
    return (ProjectionB / (normalizedDepth - ProjectionA));
}

float ComputeLinearDepth(Texture2D tex, SamplerState sam, float2 screenCoords)
{
    float depth = tex.Sample(sam, screenCoords).r;

    return ComputeLinearDepth(depth);
}
