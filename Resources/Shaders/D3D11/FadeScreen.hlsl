/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

cbuffer cb_FadeScreenPS
{
    float4 gFadeScreenColorTop;
    float4 gFadeScreenColorBot;
};

struct VSOutput
{
    float4 posH : SV_Position;
    float2 tex0 : TEXCOORD0;
};

VSOutput VS(float3 pos : POSITION0,
            float2 tex0 : TEXCOORD0)
{
    VSOutput outVS = (VSOutput)0;

    outVS.posH = float4(pos, 1.0f);
    outVS.tex0 = tex0;

    return outVS;
}


float4 PS(VSOutput input) : SV_Target
{
    float4 fadeScreenColor = lerp(gFadeScreenColorTop, gFadeScreenColorBot, input.tex0.y);

    return fadeScreenColor;
}

