/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SKY_PLANE_RENDER_OBJECT_H
#define SKY_PLANE_RENDER_OBJECT_H

namespace env
{
    struct SkyPlaneRenderObject
    {
        SkyPlaneRenderObject()
            : m_alpha(1.0f) {}

        mapMeshInstanceRenderObjects m_meshInstances;

        float m_alpha;
    };

} // env
#endif // SKY_PLANE_RENDER_OBJECT_H 