/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef STRIPE_RENDER_OBJECT_H
#define STRIPE_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    struct Stripe
    {
//         Stripe()
//             : m_startTopColor(0.05f, 0.05f, 0.05f, 1.0f)
//             , m_startBotColor(0.05f, 0.05f, 0.05f, 1.0f)
//             , m_endTopColor(0.0f, 0.0f, 0.0f, 1.0f)
//             , m_endBotColor(0.0f, 0.0f, 0.0f, 1.0f) {}

        Vec2 m_startTopPos;
        Vec2 m_startBotPos;
        Vec2 m_endTopPos;
        Vec2 m_endBotPos;

        Vec4 m_startTopColor;
        Vec4 m_startBotColor;
        Vec4 m_endTopColor;
        Vec4 m_endBotColor;

        float m_startWidth;
        float m_endWidth;

        float m_timeSpawnStart;
        float m_timeSpawnDuration;
    };

    typedef std::vector<Stripe> vecStripes;

    struct StripeRenderObject
    {
        StripeRenderObject()
            : m_stripesAmount(0.0f)
            , m_stripeTime(1.0f) {}

        float m_stripesAmount;
        float m_stripeTime;

        vecStripes m_stripes;
    };


} // env
#endif // SPRITE_RENDER_OBJECT_H #pragma once
