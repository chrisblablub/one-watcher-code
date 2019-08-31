/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SUN_RENDER_OBJECT_H
#define SUN_RENDER_OBJECT_H

namespace env
{
    struct SunRenderObject
    {
        SunRenderObject()
            : m_diffuseColor(1.0f, 1.0f, 1.0f)
            , m_position(0.0f, 0.0f, 0.0f) {}
         
        Vec3 m_diffuseColor;
        Vec3 m_position;
    };

} // env
#endif // SUN_RENDER_OBJECT_H 