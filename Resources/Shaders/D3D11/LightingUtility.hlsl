/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


//-----------------------------------------------------------------------------------
float ComputeFresnelSchlickApprox(float n1, float n2, float3 normal, float3 ray)
{
    float R0 = (n1 - n2) / (n1 + n2);

    R0 *= R0;

    return saturate(R0 + (1 - R0) * pow(1 - dot(normal, ray), 5));
}