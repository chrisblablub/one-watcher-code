/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_SpritesVS
{
    float4x4 gWVP;
    float4   gTexRect;
};

cbuffer cb_SpritesPS
{
    float4 gColor;
};


Texture2D t0 : register(t0);

sampler s0 : register(s0);

struct VSOutput
{
    float4 posH : SV_Position;    
    float2 tex : TEXCOORD0;
    float4 center : TEXCOORD1;
    float vis : TEXCOORD2;
};

VSOutput VS(float3 pos : POSITION0,
            float2 texCoords : TEXCOORD0)
{
    VSOutput vsOut = (VSOutput)0;
    vsOut.posH = mul(float4(pos, 1.0f), gWVP);
    vsOut.center = mul(float4(0, 0, 0, 1.0f), gWVP);
    vsOut.tex = float2(gTexRect.x + texCoords.x * gTexRect.z, gTexRect.y + (1.0f - texCoords.y) * gTexRect.w);


    float2 ndcCorner = vsOut.posH.xy / vsOut.posH.w;
    float2 ndcCenter = vsOut.center.xy / vsOut.center.w;

    float h = abs(ndcCorner.y - ndcCenter.y);
    float w = abs(ndcCorner.x - ndcCenter.x);

    float screenWidth = min(1.0, ndcCenter.x + w) - max(-1.0, ndcCenter.x - w);
    float screenHeight = min(1.0, ndcCenter.y + h) - max(-1.0, ndcCenter.y - h);

    float screenArea = screenWidth * screenHeight;
    float area = w * h * 4;

    float visibility = saturate(screenArea / area);

    vsOut.vis = visibility;

    return vsOut;
}

float4 PS(VSOutput input) : SV_Target
{
    
    return t0.Sample(s0, input.tex.xy) * gColor * float4(1, 1, 1, 1);
}
