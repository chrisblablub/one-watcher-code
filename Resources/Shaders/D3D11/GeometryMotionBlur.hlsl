/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#include "GeometryUtility.hlsl"

cbuffer cb_gInvScreenSize
{
    float2 gInvScreenSize;
}
cbuffer cb_gPrevModel
{
    float4x4 gPrevModel;
};

cbuffer cb_gIntensity
{
    float gIntensity;
}

Texture2D t6 : register(t6);

sampler s6 : register(s6);


struct VSOutputMesh
{
    float4 posH : SV_Position;
    float4 pos : TEXCOORD0;
    float4 prevPos : TEXCOORD1;
};

struct PSOutputTerrain
{
    float4 RT1 : SV_Target0;
};


VSOutputMesh VS(float3 pos : POSITION0,
                float3 normal : NORMAL0,
                float3 biNormal : BINORMAL0,
                float3 tangent : TANGENT0,
                float2 tex0 : TEXCOORD0)
{
    VSOutputMesh outVS = (VSOutputMesh) 0;

    float4 posL = mul(float4(pos, 1.0f), gModel);
    float4 prevPosL = mul(float4(pos, 1.0f), gPrevModel);

    posL.z -= 2;
    prevPosL.z -= 2;

    float4 posH = mul(posL, gViewProjection);
    float4 prevPosH = mul(prevPosL, gViewProjection);

    outVS.posH = posH;
    outVS.pos = posH;
    outVS.prevPos = prevPosH;

    return outVS;
}

float4 PS(VSOutputMesh input, float4 vpos : SV_Position) : SV_Target0
{
    float2 a = (input.pos.xy / input.pos.w) * 0.5 + 0.5;
    float2 b = (input.prevPos.xy / input.prevPos.w) * 0.5 + 0.5;
    
    float2 velocity = a - b;
    velocity.x = -velocity.x;
    velocity *= gIntensity;
    float speed = length(velocity / gInvScreenSize);
    speed *= 0.5;
    int nSamples = clamp(int(speed), 1, 8);

    float2 uv = vpos.xy * gInvScreenSize;
    //uv.y = 1.0 - uv.y;
    //return float4(uv, 0, 1);

    float3 c = t6.Sample(s6, uv);

    for (int i = 1; i < nSamples; ++i)
    {
        float2 uvOffset = 1 * velocity * (float(i) / float(nSamples - 1) - 0.5);
        c += t6.Sample(s6, uv + uvOffset);
    }
    c /= float(nSamples);




    return float4(c, 1);

}

