/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#include "NormalMap.hlsl"
#include "GeometryUtility.hlsl"

cbuffer cb_GeometryPS
{
    float4   gDiffuseMtrl;
    float4   gEmissiveMtrl;
    float4   gEnvMtrl;
    bool     gHasNormalMap;
    bool     gHasSpecularMap;
    bool     gHasEmissiveMap;
    bool     gHasTexture;
    bool     gHasEnvMap;
};

Texture2D t0 : register(t0);
Texture2D t1 : register(t1);
Texture2D t2 : register(t2);
Texture2D t3 : register(t3);
Texture2D t6 : register(t6);
TextureCube t4 : register(t4);

sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);
sampler s6 : register(s6);

struct VSOutputMesh
{
    float4 posH : SV_Position;
    float2 depth : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 tex : TEXCOORD2;
    float3 biNormal : TEXCOORD3;
    float3 tangent : TEXCOORD4;
    float3 pos : TEXCOORD5;
};


struct PSOutput
{
    float4 RT1 : SV_Target0;
    float4 RT2 : SV_Target1;
    float4 RT3 : SV_Target2;
    float3 RT4 : SV_Target3;
};


VSOutputMesh VS(float3 pos : POSITION0,
                float3 normal : NORMAL0,
                float3 biNormal : BINORMAL0,
                float3 tangent : TANGENT0,
                float2 tex0 : TEXCOORD0)
{
    VSOutputMesh outVS = (VSOutputMesh)0;

    float4 posL = float4(pos, 1.0f);

    outVS.posH = mul(posL, mul(gModel, gViewProjection));
    outVS.pos = mul(posL, gModel).xyz;
    outVS.depth = outVS.posH.zw;
    outVS.normal = normal;
    outVS.tangent = tangent;
    outVS.biNormal = biNormal;
    outVS.tex = tex0;

    return outVS;
}


PSOutput PS(VSOutputMesh input
          /*float2 depth : TEXCOORD0,
            float3 normal : TEXCOORD1,
            float2 tex : TEXCOORD2,
            float3 biNormal : TEXCOORD3,
            float3 tangent : TEXCOORD4,
            float3 pos : TEXCOORD5*/)
{
    float3x3 model;
    model[0] = gModel[0].xyz;
    model[1] = gModel[1].xyz;
    model[2] = gModel[2].xyz;

    if (gHasNormalMap)
    {
        input.normal = SampleNormalMap(model, input.normal, input.biNormal, input.tangent, t1, s1, input.tex.xy);
    }
    else
    {
        input.normal = mul(input.normal, model).xyz;
    }

    input.normal = normalize(mul(input.normal, gView));

    float4 diff = float4(gDiffuseMtrl.rgb, 1);

    if (gHasTexture)
    {
        diff *= t0.Sample(s0, input.tex.xy).rgba;
    }

    float3 spec = float3(1.0, 1.0, 1.0);

    if (gHasSpecularMap)
    {
        spec.rgb = t2.Sample(s2, input.tex.xy).rgb;
    }

    float3 emissive = float3(0.0, 0.0, 0.0);

    if (gHasEmissiveMap)
    {
        emissive.rgb = t3.Sample(s3, input.tex.xy).rgb;
    }

    if (gHasEnvMap)
    {
        float3 viewSurfacePos = input.pos;
        float3 viewSurfaceRay = normalize(-viewSurfacePos);
        float3 viewReflectedRay = reflect(viewSurfaceRay, input.normal.xyz);

        float3 reflectedLight = t4.Sample(s4, viewReflectedRay).rgb * 1;

        diff.rgb = lerp(diff.rgb, reflectedLight * gEnvMtrl.rgb, gEnvMtrl.a);
    }


    float snow = max(dot(normalize(input.normal), float3(0, 1, -0.3)), 0.0);
    //float3 snowTexture = tex2D(s6, tex.xy).rgb;
    float3 snowTexture = t6.Sample(s6, input.tex.xy).rgb;

    snow = snow * snow * 2;
    diff.xyz = lerp(diff.xyz, snowTexture * gDiffuseMtrl.rgb, snow);

    PSOutput outPS;
    outPS.RT1 = float4(diff.rgba);
    outPS.RT2 = float4(input.normal.rgb, 1);
    outPS.RT3 = float4(input.depth.x / input.depth.y, 0, 0, 0);
    outPS.RT4 = float3(emissive.rgb *  gEmissiveMtrl.rgb);
    return outPS;
}
