/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/
cbuffer cb_FadePS
{
    float4 gFadeScreenColorTop;
    float4 gFadeScreenColorBot;
    float4 gFadeFrameColorTop;
    float4 gFadeFrameColorBot;
    float4 gFadeBarsColor;
    float4 gFadeFrameAmount;
    float4 gScalingOffset;
    float  gFadeBarsAmount;
    int    gType;
};

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

static const float M = -0.0f;


float fct(float x, float b)
{
    return M * x + b;
}


static const float ZFAR = 10000.0f;
static const float ZNEAR = 1.0f;

float ComputeLinearDepth(float normalizedDepth)
{
    float ProjectionA = ZFAR / (ZFAR - ZNEAR);
    float ProjectionB = (-ZFAR * ZNEAR) / (ZFAR - ZNEAR);
    return (ProjectionB / (normalizedDepth - ProjectionA));
}

float4 PS(VSOutput input) : SV_Target
{
    float2 texCoords = input.tex0;


    float4 source = float4(t0.Sample(s0, texCoords).rgb, 1.0f);

    // Fade Frame
    float4 fadeFrame = gFadeFrameAmount;

    if (texCoords.x <= fadeFrame.x || texCoords.x >= fadeFrame.y ||
        texCoords.y <= fadeFrame.z || texCoords.y >= fadeFrame.w)
    {
        float4 fadeFrameColor = lerp(gFadeFrameColorTop, gFadeFrameColorBot, texCoords.y);

        source.rgb = lerp(source.rgb, fadeFrameColor.rgb, fadeFrameColor.a);

        return source;
    }

    float4 fadeScreenColor = lerp(gFadeScreenColorTop, gFadeScreenColorBot, input.tex0.y);

    // Fade Bars
    if (gType == 1)
        // Horizontal and sloped bars 
        //
    {

        // b1 and b2 are the y axis offsets for the corresponding functions (at x = 0).
        // They derive from the line equations and that x and y are in [0, 1].
        //

        float amount = gFadeBarsAmount;

        float b1 = lerp(0.0f, -0.5 * M + 0.6, amount);
        float b2 = lerp(1.0f - M, -0.5 * M + 0.4, amount);


        float targetY1 = fct(texCoords.x, b1);
        float targetY2 = fct(texCoords.x, b2);

        float currentY = texCoords.y;

        float4 clr = float4(source.rgb, 1);
        float fadeTex = 0;

        if (currentY <= targetY1)
        {
            // top bar
            float2 tex = float2(texCoords.x, currentY / targetY1);
            //fadeTex = t1.Sample(s1, float2(tex.x, tex.y));
            fadeTex = 1;
            //source = fadeTex;
        }
        if (currentY >= targetY2)
        {
            // bottom bar
            float2 tex = float2(texCoords.x, 1.0f - (currentY - targetY2) / (1.0f - targetY2));
            //fadeTex = max(fadeTex, t1.Sample(s1, float2(1.0f - tex.x, tex.y)));
            fadeTex = 1;
        }

        source = float4(lerp(source.rgb, gFadeBarsColor.rgb, fadeTex), 0.6);
    }
    else if (gType == 2)
    {
        // Fade from right to left
        texCoords.x = texCoords.x * 0.9f + 0.1f;

        // 1...0
        float test = (1.0f - gFadeBarsAmount);

        //float targetX = gScalingOffset.z;
        //if (texCoords.x > test + targetX)
        //    source = float4(0, 0, 0, 1);

        if (texCoords.x > test)
        {
            float2 tex = float2((1 - texCoords.x) + test, texCoords.y * gScalingOffset.y);
            source = float4(lerp(source.rgb, fadeScreenColor.rgb, t1.Sample(s1, tex.yx).a), 1);
        }
    }
    else if (gType == 3)
    {
        // Fade from left to right
        texCoords.x *= 0.9f;
        //float targetX = gScalingOffset.z;
        //if (texCoords.x < gFadeBarsAmount - targetX)
       //     source = float4(0, 0, 0, 1);
        if (texCoords.x < gFadeBarsAmount)
        {
            float2 tex = float2(texCoords.x - gFadeBarsAmount, texCoords.y * gScalingOffset.y);
            source = float4(lerp(source.rgb, fadeScreenColor.rgb, t1.Sample(s1, tex.yx).a), 1);
        }
    }
    else if (gType == 4)
    {
        // Fade from top to bottom
        float test = gFadeBarsAmount;


        texCoords.y *= 0.9f;
        //float targetY = gScalingOffset.w;
        //if (texCoords.y < test - targetY)
        //    source = float4(0, 0, 0, 1);

        if (texCoords.y < test)
        {
            float2 tex = float2(texCoords.x * gScalingOffset.x, texCoords.y - test);
            source = float4(lerp(source.rgb, fadeScreenColor.rgb, t1.Sample(s1, tex.xy).a), 1);
        }
    }
    else if (gType == 5)
    {
        // Fade from bottom to top
        //
        texCoords.y = texCoords.y * 0.9f + 0.1f;

        float test = (1.0f - gFadeBarsAmount);

        //float targetY = gScalingOffset.w;
        //if (texCoords.y > test + targetY)
        //    source = float4(1, 0, 0, 1);

        if (texCoords.y > test)
        {
            float2 tex = float2(texCoords.x * gScalingOffset.x, (1 - texCoords.y) + test);

            source.rgb = lerp(source.rgb, fadeScreenColor.rgb, t1.Sample(s1, tex.xy).a);
        }
    }



    return source;
}

