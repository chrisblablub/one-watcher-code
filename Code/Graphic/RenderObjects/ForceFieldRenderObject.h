/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FORCE_FIELD_RENDER_OBJECT_H
#define FORCE_FIELD_RENDER_OBJECT_H

namespace env
{
    struct ForceFieldRenderObject
    {
        ForceFieldRenderObject()
            : m_mtxTransform(1.0f) {}


        Matrix4x4 m_mtxTransform;

        CQuaternion m_orientation;

        Vec3 m_up;
        Vec3 m_color;
    };

} // env
#endif // FORCE_FIELD_RENDER_OBJECT_H 