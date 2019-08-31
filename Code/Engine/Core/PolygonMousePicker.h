/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PPOLYGON_MOUSE_PICKER_H
#define PPOLYGON_MOUSE_PICKER_H

#include <Engine/EngineCommon.h>

namespace env
{
    class CCamera;

    class CPolygonMousePicker
    {
    public:

        CPolygonMousePicker();
        ~CPolygonMousePicker();

        void AddPoint(const Vec3& point);
        bool CheckPicked(const Matrix4x4& mtxTransform, CCamera* pCamera, float* pDistance);

    private:
        std::vector< Vec3 > m_points;
    };
} // env

#endif // PPOLYGON_MOUSE_PICKER_H 