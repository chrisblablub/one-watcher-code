/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CAMERA_RENDER_OBJECT_H
#define CAMERA_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>
#include <Common/Core/Frustum.h>


namespace env
{
    struct CameraRenderObject
    {
        CameraRenderObject() {}


        Matrix4x4 m_mtxProjection;
        Matrix4x4 m_mtxView;
        Matrix4x4 m_mtxViewProjection;

        Matrix4x4 m_mtxInverseProjection;
        Matrix4x4 m_mtxInverseView;

        Vec3 m_position;
        Vec3 m_up;
        Vec3 m_forward;
        Vec3 m_right;

        float m_near;
        float m_far;
        float m_l;
        float m_r;
        float m_t;
        float m_b;

        int m_screenWidth;
        int m_screenHeight;

        CFrustum m_frustum;
    };

} // env
#endif // CAMERA_RENDER_OBJECT_H 