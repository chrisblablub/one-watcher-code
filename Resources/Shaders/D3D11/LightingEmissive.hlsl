/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_LightingEmissiveVS
{
    float4x4 gCamInvPMatrix;
}

cbuffer cb_LightingEmissivePS
{
    float2 gInvScreenSize;
};

Texture2D t0 : register(t0);

sampler s0 : register(s0);


struct VSOutputDirectional
{
    float4 posH : SV_Position;
    float3 pos : TEXCOORD0;
};


VSOutputDirectional VS(float3 pos : POSITION0)
{
    VSOutputDirectional outVS = (VSOutputDirectional)0;

    outVS.posH = float4(pos, 1.0f);

    float3 p = mul(float4(pos, 1.0f), gCamInvPMatrix).xyz;
        outVS.pos = float3(p.xy / p.z, 1.0f);
    //outVS.pos = pos;
    return outVS;
}

float4 PS(VSOutputDirectional input, float4 tex0 : SV_Position) : SV_Target0
{
    float2 tex = tex0 * gInvScreenSize;;

    return float4(t0.Sample(s0, tex).rgb, 1.0f);
}
