/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_gVP
{
    float4x4 gVP;
}


Texture2D t6 : register(t6);

sampler s6 : register(s6);


struct VSParticleColor
{
    float4 posH : SV_Position;
    float4 color : COLOR0;
    float2 tex : TEXCOORD0;
    float blending : TEXCOORD1;
};

struct VSParticleDistort
{
    float4 posH : SV_Position;
    float4 color : COLOR0;
    float2 tex : TEXCOORD0;
    float blending : TEXCOORD1;
    float distort : TEXCOORD2;
    float3 pos : TEXCOORD3;
    float3 tangent : TEXCOORD4;
    float3 binormal : TEXCOORD5;
    float3 normal : TEXCOORD6;
};

