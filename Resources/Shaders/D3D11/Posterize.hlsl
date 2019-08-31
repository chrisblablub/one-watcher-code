/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */



static const float POSTERIZATION_GAMMA = 0.1f;
static const float POSTERIZATION_NUM_COLORS = 30.0f;

float3 ComputePosterizedColor(float3 color, 
                              float numColors = POSTERIZATION_NUM_COLORS, 
                              float posterizeGamma = POSTERIZATION_GAMMA)
{
    float3 c1 = dot(color.rgb, GREY);
   
    c1 = pow(c1, float3(POSTERIZATION_GAMMA, POSTERIZATION_GAMMA, POSTERIZATION_GAMMA));
    c1 = c1 * POSTERIZATION_NUM_COLORS;
    c1 = floor(c1);
    c1 = c1 / POSTERIZATION_NUM_COLORS;
    c1 = pow(c1, 1.0 / POSTERIZATION_GAMMA * float3(1.0, 1.0, 1.0));

    return c1;
}