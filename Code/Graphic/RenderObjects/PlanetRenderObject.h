/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PLANET_RENDER_OBJECT_H
#define PLANET_RENDER_OBJECT_H

namespace env
{
    struct PlanetRenderObject
    {
        PlanetRenderObject()
            : m_color(1.0f, 1.0f, 1.0f) {}

        mapMeshInstanceRenderObjects m_meshInstances;

        Vec3 m_color;
    };

} // env
#endif // PLANET_RENDER_OBJECT_H 