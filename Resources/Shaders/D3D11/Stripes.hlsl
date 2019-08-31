/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

//cbuffer cb_gInvScreenSize { float2 gInvScreenSize; }

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);
Texture2D t4 : register(t4);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);

struct VSOutput
{
    float4 posH : SV_Position;
    float4 color : TEXCOORD0;
    float3 pos : TEXCOORD1;
};

VSOutput VS(float4 pos : POSITION0,
            float4 color : COLOR0)
{
    VSOutput outVS = (VSOutput)0;

    outVS.posH = float4(pos.xyz, 1.0f);
    outVS.color = color;
    outVS.pos = pos;

    return outVS;
}


float4 PS(VSOutput input, float4 pos : SV_Position) : SV_Target
{
    //float2 tex = pos.xy * gInvScreenSize;

    /*float2 filteredTexCoords = t2.Sample(s2, tex).rg;

    float3 filteredSource = t0.Sample(s0, filteredTexCoords).rgb;
    float3 source = t0.Sample(s0, tex).rgb;

    float3 top = float3(0.1, 0.1, 0.1);
    float3 bot = float3(0.01, 0.01, 0.01);

    float3 grad = lerp(top, bot, tex.y);*/

    //float4 logo = t3.Sample(s3, tex);

   // logo.rgb *= input.color.rgb;

   // input.color.rgb = lerp(input.color.rgb, logo.rgb, logo.a);
    //logo.rgb *= grad;
    //input.color.rgb = lerp(input.color.rgb, logo.rgb, logo.a);

    return float4(input.color);
}

