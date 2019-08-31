/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef COLOR_UTILITY_H
#define COLOR_UTILITY_H

#include <Common/Common.h>
#include <string>

namespace env
{
    //-----------------------------------------------------------------------------------
    inline Vec3& DoGamma(Vec3& color)
    {
        color.x = std::pow(color.x, 1.0f / 2.2f);
        color.y = std::pow(color.y, 1.0f / 2.2f);
        color.z = std::pow(color.z, 1.0f / 2.2f);

        return color;
    }
    //-----------------------------------------------------------------------------------
    inline Vec4& DoGamma(Vec4& color)
    {
        color.x = std::pow(color.x, 1.0f / 2.2f);
        color.y = std::pow(color.y, 1.0f / 2.2f);
        color.z = std::pow(color.z, 1.0f / 2.2f);
        //color.w = std::pow(color.w, 1.0f / 2.2f);

        return color;
    }

    //-----------------------------------------------------------------------------------
    inline Vec3& UndoGamma(Vec3& color)
    {
        color.x = std::pow(color.x, 2.2f);
        color.y = std::pow(color.y, 2.2f);
        color.z = std::pow(color.z, 2.2f);

        return color;
    }

    //-----------------------------------------------------------------------------------
    inline Vec4& UndoGamma(Vec4& color)
    {
        color.x = std::pow(color.x, 2.2f);
        color.y = std::pow(color.y, 2.2f);
        color.z = std::pow(color.z, 2.2f);
        //color.w = std::pow(color.w, 2.2f);

        return color;
    }

    //-----------------------------------------------------------------------------------
    inline Vec4& RGBByte2Float(Vec4& color)
    {
        color.x = color.x / 255.0f;
        color.y = color.y / 255.0f;
        color.z = color.z / 255.0f;
        color.w = color.w / 255.0f;

        return color;
    }

    //-----------------------------------------------------------------------------------
    inline Vec3& RGBByte2Float(Vec3& color)
    {
        color.x = color.x / 255.0f;
        color.y = color.y / 255.0f;
        color.z = color.z / 255.0f;

        return color;
    }

    //-----------------------------------------------------------------------------------
    inline Vec3 RGBByte2Float(unsigned int r, unsigned int g, unsigned int b)
    {
        return Vec3(static_cast<float>(r) / 255.0f,
                    static_cast<float>(g) / 255.0f,
                    static_cast<float>(b) / 255.0f);
    }

} // env 
#endif // COLOR_UTILITY_H