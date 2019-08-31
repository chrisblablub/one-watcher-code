/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
 //-----------------------------------------------------------------------------------
float3 SampleNormalMap(float3x3 modelMatrix, float3 normal, float3 binormal, float3 tangent, Texture2D tex, SamplerState sam, float2 coords)
{
    float3x3 TBN;
    TBN[0] = tangent;
    TBN[1] = binormal;
    TBN[2] = normal;

    float3x3 tangentToWorld = mul(TBN, modelMatrix);

    float3 n = 2.0f * tex.Sample(sam, coords).xyz - float3(1.0f, 1.0f, 1.0f);

    return mul(n, tangentToWorld).xyz;
}

//-----------------------------------------------------------------------------------
float3 SampleNormalMap(float3 normal, float3 binormal, float3 tangent, Texture2D tex, SamplerState sam, float2 coords)
{
    float3x3 TBN;
    TBN[0] = tangent;
    TBN[1] = binormal;
    TBN[2] = normal;

    float3x3 tangentToWorld = TBN;

    float3 n = 2.0f * tex.Sample(sam, coords).xyz - float3(1.0f, 1.0f, 1.0f);

    return mul(n, tangentToWorld).xyz;
}
