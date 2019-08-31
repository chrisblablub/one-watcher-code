/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CAMERA_STATE_H
#define CAMERA_STATE_H

#include <Engine/EngineCommon.h>
#include <Common/Core/Frustum.h>


namespace env
{
    class CCamera;

    struct CameraState
    {
        CameraState() {}

        void setFromCamera(CCamera* pCam, int screenWidth, int screenHeight);

        Matrix4x4 m_mtxProjection;
        Matrix4x4 m_mtxView;
        Matrix4x4 m_mtxViewProjection;

        Matrix4x4 m_mtxInverseProjection;
        Matrix4x4 m_mtxInverseView;

        Vec3 m_position;
        Vec3 m_up;
        Vec3 m_forward;
        Vec3 m_right;

        int m_screenWidth;
        int m_screenHeight;

        CFrustum m_frustum;
    };

} // End namespace 

#endif // CAMERA_STATE_H

