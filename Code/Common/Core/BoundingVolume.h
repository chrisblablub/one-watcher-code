/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <Common/Common.h>

namespace env
{
    class CBoundingVolume
    {
    public:
        CBoundingVolume();
        CBoundingVolume(const CBoundingVolume& other);
        CBoundingVolume(const CBoundingVolume* other);

        virtual ~CBoundingVolume();

        bool ComputeBoundingVolume(const Vec3& vMin, const Vec3& vMax);

        //bool IsRayIntersecting(const VECTOR3& vOrigin, const VECTOR3& vDirection, float* pDistance = NULL);
        void Transform(const Matrix4x4& mtxTransform);

        void GetMinMax(Vec3& vMin, Vec3& vMax);
        const Vec3& GetOrigin() const { return m_vOrigin; }
        const Vec3& GetHalfLengths() const { return m_vHalfLengths; }

        const Vec3& GetRight() const { return m_vRight; }
        const Vec3& GetUp() const { return m_vUp; }
        const Vec3& GetForward() const { return m_vForward; }

    protected:

        Vec3 m_vRight;
        Vec3 m_vForward;
        Vec3 m_vUp;
        Vec3 m_vHalfLengths;
        Vec3 m_vOrigin;
        Vec3 m_vMin;
        Vec3 m_vMax;
    };

} // End namespace

#endif // BOUNDINGVOLUME_H