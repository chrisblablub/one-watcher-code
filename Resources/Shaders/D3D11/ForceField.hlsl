/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

cbuffer cb_ForcefieldVS
{
    float4x4 gCamVPMatrix;
};

cbuffer cb_ForcefieldPS
{
    float4 gTimeDirection;
    float4 gRotMatrix;
    float2 gDirection;
    float2 gInvScreenSize;
    float3 gColor;
};

Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);

sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);

struct OutputVS
{
    float4 posH : SV_Position;
    float2 tex : TEXCOORD0;
    float3 pos : TEXCOORD1;
};


OutputVS VS(float3 pos : POSITION0,
            float2 tex : TEXCOORD0)
{
    OutputVS vsOut = (OutputVS)0;
    vsOut.pos = pos;
    vsOut.posH = mul(float4(pos, 1.0f), gCamVPMatrix);
    vsOut.tex = tex;
    return vsOut;
}


float4 PS(OutputVS input, float4 vpos : SV_Position) : SV_Target
{
    float2 texCoords = float2(input.pos.x / 200.0, -input.pos.y / 200.0f);

    float2 screenCoords = vpos.xy * gInvScreenSize;
    float3 source = t1.Sample(s1, screenCoords).rgb;
    //float4 ff3 = tex2D(s3, texCoords).rgba;

    float alpha = 1.0;
    alpha = 1.0 - pow(abs(input.tex.x - 0.5) / 0.5, 4);
    alpha *= 1.0 - pow(abs(input.tex.y - 0.5) / 0.5, 4);

    //return float4(ff3.rgb, 1);

    float4 time = gTimeDirection * 1.5;
        float2 dir = gDirection;
        float2 rotTexCoords = float2(0, 0);

        float2 texTmp = texCoords * 0.7 + time.xy;
        rotTexCoords.x = texTmp.x * gRotMatrix.x + texTmp.y * gRotMatrix.y;
    rotTexCoords.y = texTmp.x * gRotMatrix.z + texTmp.y * gRotMatrix.w;
    rotTexCoords.y = 1 - rotTexCoords.y;
    float2 water = t2.Sample(s2, texCoords * 0.3 + time.xy).rg * 0.5;



        texTmp = texCoords  * 1.4 + water.xy + dir * time.w;

    rotTexCoords.x = texTmp.x * gRotMatrix.x + texTmp.y * gRotMatrix.y;
    rotTexCoords.y = texTmp.x * gRotMatrix.z + texTmp.y * gRotMatrix.w;
    rotTexCoords.y = 1 - rotTexCoords.y;

    float4 ff = t3.Sample(s3, rotTexCoords).rgba;


        texTmp = texCoords * 1.2 + water.xy + dir * time.w * 1.5;

    rotTexCoords.x = texTmp.x * gRotMatrix.x + texTmp.y * gRotMatrix.y;
    rotTexCoords.y = texTmp.x * gRotMatrix.z + texTmp.y * gRotMatrix.w;
    rotTexCoords.y = 1 - rotTexCoords.y;

    float4 ff3 = t3.Sample(s3, rotTexCoords).rgba;

        ff += ff3;

    float4 final = float4(ff.rgb * gColor/*float3(1, 0.3, 0)*/, pow(ff.a, 2.5) * alpha * 1);

        return final;
}
