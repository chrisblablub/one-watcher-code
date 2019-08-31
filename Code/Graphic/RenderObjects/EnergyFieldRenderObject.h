/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENERGY_FIELD_RENDER_OBJECT_H
#define ENERGY_FIELD_RENDER_OBJECT_H

namespace env
{
    struct EnergyFieldRenderObject
    {
        EnergyFieldRenderObject()
            : m_color(1.0f, 1.0f, 1.0f) {}

        mapMeshInstanceRenderObjects m_meshInstances;

        Vec3 m_color;
    };

} // env
#endif // ENERGY_FIELD_RENDER_OBJECT_H 