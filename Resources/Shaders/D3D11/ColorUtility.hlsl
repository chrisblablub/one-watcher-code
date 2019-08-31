/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */



//-----------------------------------------------------------------------------------
// Ported from http://serennu.com/colour/rgbtohsl.php
float3 ConvertRGB2HSL(float3 rgb)
{
    float var_min = min(rgb.r, min(rgb.g, rgb.b));
    float var_max = max(rgb.r, max(rgb.g, rgb.b));
    float del_max = var_max - var_min;

    float l = (var_max + var_min) / 2.0;

    float h = 0, s = 0;
    if (del_max <= 0)
    {
        h = 0;
        s = 0;
    }
    else
    {
        if (l < 0.5)
            s = del_max / (var_max + var_min);
        else
            s = del_max / (2.0 - var_max - var_min);

        float del_r = (((var_max - rgb.r) / 6.0) + (del_max / 2.0)) / del_max;
        float del_g = (((var_max - rgb.g) / 6.0) + (del_max / 2.0)) / del_max;
        float del_b = (((var_max - rgb.b) / 6.0) + (del_max / 2.0)) / del_max;

        if (rgb.r >= var_max)
            h = del_b - del_g;
        else if (rgb.g >= var_max)
            h = (1.0 / 3.0) + del_r - del_b;
        else if (rgb.b >= var_max)
            h = (2.0 / 3.0) + del_g - del_r;

        if (h < 0)
            h += 1;

        if (h > 1)
            h -= 1;
    };

    return float3(h, s, l);
}

//-----------------------------------------------------------------------------------
float ConvertHue2RGB(float v1, float v2, float vh)
{
    if (vh < 0)
        vh += 1;

    if (vh > 1)
        vh -= 1;

    if ((6 * vh) < 1)
        return (v1 + (v2 - v1) * 6 * vh);

    if ((2 * vh) < 1)
        return (v2);

    if ((3 * vh) < 2)
        return (v1 + (v2 - v1) * ((2.0 / 3.0 - vh) * 6.0));

    return (v1);
};

//-----------------------------------------------------------------------------------
float3 ConvertHSL2RGB(float h, float s, float l)
{
    float r = 0, g = 0, b = 0;

    if (s == 0)
    {
        r = l;
        g = l;
        b = l;
    }
    else
    {
        float var_2 = 0;
        float var_1 = 0;

        if (l < 0.5)
            var_2 = l * (1 + s);
        else
            var_2 = (l + s) - (s * l);

        var_1 = 2.0 * l - var_2;

        r = ConvertHue2RGB(var_1, var_2, h + (1.0 / 3.0));
        g = ConvertHue2RGB(var_1, var_2, h);
        b = ConvertHue2RGB(var_1, var_2, h - (1.0 / 3.0));
    };

    return float3(r, g, b);
}

//-----------------------------------------------------------------------------------
float ComputeInverseHue(float h)
{
    // compute opposite hue
    float h2 = h + 0.5;

    if (h2 > 1)
    {
        h2 -= 1;
    };

    return h2;
}


//-----------------------------------------------------------------------------------
float3 ComputeLinearizedColor(const in float3 gammaColor)
{
    return pow(gammaColor, GAMMA_CORRECTION_EXPONENT);
}

//-----------------------------------------------------------------------------------
float3 ComputeGammarizedColor(const in float3 linearColor)
{
    return pow(linearColor, 1.0 / GAMMA_CORRECTION_EXPONENT);
}
