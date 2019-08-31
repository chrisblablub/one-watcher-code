/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
cbuffer cb_CharacterVS
{
    float4x4 gWVP;
}

cbuffer cb_CharacterPS 
{ 
    float4 gTime; 
}


Texture2D t0 : register(t0);
Texture2D t1 : register(t1);

sampler s0 : register(s0);
sampler s1 : register(s1);

struct OutputVS
{
    float4 posH : SV_Position;
    float2 tex : TEXCOORD0;
};


OutputVS VS(float3 pos : POSITION0,
            float2 tex : TEXCOORD0)
{
    OutputVS vsOut = (OutputVS)0;

    vsOut.posH = mul(float4(pos.x, pos.y, pos.z, 1.0), gWVP);
    vsOut.tex = tex;

    return vsOut;
}

float4 PS(OutputVS input) : SV_Target
{
    float4 character = t0.Sample(s0, input.tex);

    float2 distCoords = t1.Sample(s1, input.tex * 0.15 + float2(gTime.x, gTime.x)).rg;
    
    float4 characterDistorted = t0.Sample(s0, input.tex + float2(0.455, 0.435) * distCoords);

    character += characterDistorted;

    return character / 1.2;
}

