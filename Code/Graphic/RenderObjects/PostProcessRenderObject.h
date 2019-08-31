/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef POSTPROCESS_RENDER_OBJECT_H
#define POSTPROCESS_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    struct ImageDistortion
    {
        ImageDistortion()
            : alpha(0.0f), amount(0.0f), dir(0.0f, 0.0f),
            scaling(0.0f, 0.0f), color(0.0f, 0.0f, 0.0f) {}

        float alpha;
        float amount;
        Vec2 dir;
        Vec2 scaling;
        Vec3 color;
    };

    struct PostProcessRenderObject
    {
        PostProcessRenderObject()
            : m_silhouetteIntensity(0.0f)
            , m_posterizationAmount(0.0f)
            , m_silhouetteCharacterOpacity(0.0f)
            , m_silhouetteCharacterRadius(0.0f)
            , m_silhouetteColor(0.0f, 0.0f, 0.0f) 
            , m_saturation(1.0f) {}

        float m_silhouetteIntensity;
        float m_posterizationAmount;
        float m_silhouetteCharacterOpacity;
        float m_silhouetteCharacterRadius;

        Vec3 m_silhouetteColor;

        ImageDistortion m_imageDistortion;

        Vec3 m_colorScaling;
        Vec3 m_colorOffset;

        float m_contrast;
        float m_sepia;
        float m_hueRotation;
        float m_saturation;
    };

} // env
#endif // POSTPROCESS_RENDER_OBJECT_H 