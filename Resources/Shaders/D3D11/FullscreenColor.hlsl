/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

void VS(float3 vPos : POSITION0,
        float2 tex0 : TEXCOORD0,
        out float2 texCoords : TEXCOORD0,
        out float4 vPosH : SV_POSITION)
{
    vPosH = float4(vPos, 1);
    texCoords = tex0;
}

float4 PS(float2 texCoords : TEXCOORD0) : SV_TARGET
{
    return float4(0, 0, 0, 0);
}
