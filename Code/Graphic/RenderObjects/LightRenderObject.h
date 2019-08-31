/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LIGHT_RENDER_OBJECT_H
#define LIGHT_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    enum LIGHTTYPE
    {
        LT_UNSPECIFIED = 0,
        LT_SPOT,
        LT_POINT,
        LT_DIRECTIONAL
    };


    struct LightRenderObject
    {
        LIGHTTYPE m_type;

        Vec3 m_position;
        Vec3 m_direction;
        Vec4 m_diffuseColor;
        Vec4 m_specularColor;
        Vec3 m_scale;

        Matrix4x4 m_mtxTransform;

        float m_energy;
        float m_radius;

        bool m_bSpecular;
        bool m_bStatic;
        bool m_bAffectsMeshes;
        bool m_bAffectsTerrain;

        static bool cmp(LightRenderObject* left, LightRenderObject* right)
        {
            if (left->m_type == LT_DIRECTIONAL && right->m_type != LT_DIRECTIONAL)
                return true;
            else if (left->m_type == LT_SPOT && right->m_type == LT_POINT)
                return true;

            return false;
        }
    };

    typedef std::list< LightRenderObject* > listLightRenderObjects;

} // env
#endif // LIGHT_RENDER_OBJECT_H 