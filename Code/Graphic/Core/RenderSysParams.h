/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef RENDER_SYS_PARAMS
#define RENDER_SYS_PARAMS

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>

namespace env
{
    struct VideoParameters
    {
        VideoParameters()
            : Adapter(0)
            , zNear(1.0f)
            , zFar(13000)
            , strWindowTitle("ENGINE")
            , bBloom(true)
            , bWindowed(true)
            , bVSync(true)
            , lightRays(3)
            , screenWidth(800)
            , screenHeight(600) {}

        int Adapter;
        int lightRays;
        int screenWidth;
        int screenHeight;

        float zNear;
        float zFar;

        std::string strWindowTitle;

        bool bBloom;
        bool bWindowed;
        bool bVSync;
    };

} // env

#endif // VIDEO_PARAMETERS_H