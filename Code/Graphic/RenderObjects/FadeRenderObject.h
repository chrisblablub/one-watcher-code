/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FADE_RENDER_OBJECT_H
#define FADE_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{

    enum FADE_BARS_TYPE
    {
        FADE_NONE = 0,
        FADE_BARS_FROM_TOP_AND_BOTTOM,
        FADE_BARS_FROM_RIGHT_TO_LEFT,
        FADE_BARS_FROM_LEFT_TO_RIGHT,
        FADE_BARS_FROM_TOP_TO_BOTTOM,
        FADE_BARS_FROM_BOTTOM_TO_TOP,
        FADE_BARS_COUNT
    };

    struct FadeBarsRenderObject
    {
        FadeBarsRenderObject()
            : fadeBarsType(FADE_NONE)
            , fadeBarsAmount(0.0f) {}

        FADE_BARS_TYPE fadeBarsType;

        float fadeBarsAmount;

        Vec4 fadeColor;
    };


    struct FadeScreenRenderObject
    {
        FadeScreenRenderObject()
            : m_fadeScreenColorTop(0.0f, 0.0f, 0.0f, 0.0f)
            , m_fadeScreenColorBot(0.0f, 0.0f, 0.0f, 0.0f) {}

        Vec4 m_fadeScreenColorTop;
        Vec4 m_fadeScreenColorBot;
    };

    struct FadeFrameRenderObject
    {
        FadeFrameRenderObject()
            : m_fadeFrame(0.0f, 0.0f, 0.0f, 0.0f) 
            , m_fadeFrameColorTop(0.0f, 0.0f, 0.0f, 1.0f)
            , m_fadeFrameColorBot(0.0f, 0.0f, 0.0f, 1.0f) {}

        // Fade frame amount for left (x), right(y), top(z) and bottom(w) bars in [0, 1].
        // 1 means, that the corresponding half of the screen is covered.
        Vec4 m_fadeFrame;
        Vec4 m_fadeFrameColorTop;
        Vec4 m_fadeFrameColorBot;
    };

} // env
#endif // FADE_RENDER_OBJECT_H 