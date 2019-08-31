/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MATH_INTERSECTION_H
#define MATH_INTERSECTION_H

#include <Common/Common.h>
#include <Common/Math/MathUtility.h>


namespace env
{
    class CFrustum;
    class CBoundingVolume;

    namespace intersection
    {
        enum INTERSECTION
        {
            INTERSECTION_INSIDE = 0,
            INTERSECTION_OUTSIDE,
            INTERSECTION_INTERSECTING
        };


        INTERSECTION FrustumIntersectsAABB(const CFrustum& frustum, Vec3 vBMin, Vec3 vBMax);
        INTERSECTION FrustumIntersectsBB(const CFrustum& frustum, const CBoundingVolume& box, const Matrix4x4& mtxBBoxTransform);
        bool FrustumIntersectsPoint(const CFrustum& frustum, const Vec3& vPoint);
        bool FrustumIntersectsSphere(const CFrustum& frustum, const Vec3& vCenter, float radius);
        bool RayIntersectsBB(const CBoundingVolume& bbox, const Vec3& vOrigin, const Vec3& vDirection, float* pDistance = NULL);
    }

    bool ComputeIntersectionRayPlane(const Vec3& orig, const Vec3& dir,
        const Vec3& planeOrig, const Vec3& planeNormal,
        float& intersectionDistance);

    void ComputeIntersection3D(Vec3& intersection, const Vec3& rayOrigin, const Vec3& rayDirection, const Vec3& planeOrigin, const Vec3& planeNormal);

    void ComputeIntersection3D(Vec3& intersection, const Vec3& rayOrigin, const Vec3& rayDirection, const Vec3& p0, const Vec3& p1, const Vec3& p2);

    bool ComputeIntersection2D(const Vec2& p1, const Vec2& p2,
        const Vec2& p3, const Vec2& p4, Vec2* pIntersection = NULL, float* s = NULL, float* t = NULL);

    bool ComputeIntersection3D(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4,
        Vec3* pIntersection = NULL, float* pS = NULL);
} // env
#endif // MATH_INTERSECTION_H