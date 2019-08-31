/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENVIRONMENT_RENDER_OBJECT_H
#define ENVIRONMENT_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    enum SCENARIO_WEATHER
    {
        SCENARIO_WEATHER_NORMAL = 0,
        SCENARIO_WEATHER_RAIN,
        SCENARIO_WEATHER_SNOW
    };

    struct EnvironmentRenderObject
    {
        EnvironmentRenderObject()
            : m_cloudIntensity(0.5f)
            , m_energyFieldIntensity(0.5f)
            , m_fogRange(500.0f)
            , m_fogStart(12000.0f)
            , m_fogColor(1.0f, 1.0f, 1.0f)
            , m_skyColor(1.0f, 1.0f, 1.0f)
            , m_borderColor(1.0f, 1.0f, 1.0f)
            , m_weather(SCENARIO_WEATHER_NORMAL) {}

        float m_cloudIntensity;
        float m_energyFieldIntensity;
        float m_fogRange;
        float m_fogStart;

        Vec3 m_fogColor;
        Vec3 m_skyColor;
        Vec3 m_borderColor;

        SCENARIO_WEATHER m_weather;

        std::string m_strBorderTexture;
    };

} // env
#endif // ENVIRONMENT_RENDER_OBJECT_H 