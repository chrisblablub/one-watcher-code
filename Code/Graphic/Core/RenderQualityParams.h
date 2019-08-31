/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef RENDER_QUALITY_PARAMS_H
#define RENDER_QUALITY_PARAMS_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>

namespace env
{
    struct RenderQualityParams
    {
        RenderQualityParams()
            : Adapter(0)
            , zNear(1.0f)
            , zFar(13000)
            , strWindowTitle("ENGINE")
            , bloomQuality(1)
            , bWindowed(true)
            , bVSync(true)
            , bLightRays(true)
            , screenWidth(800)
            , screenHeight(600) {}

        int Adapter;
        bool bLightRays;
        int screenWidth;
        int screenHeight;

        float zNear;
        float zFar;

        std::string strWindowTitle;

        int bloomQuality;
        bool bWindowed;
        bool bVSync;
    };

} // env

#endif // RENDER_QUALITY_PARAMS_H