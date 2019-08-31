#include "../Math/MathIntersection.h"
#include <Common/Core/Frustum.h>
#include <Common/Core/BoundingVolume.h>

namespace env
{
    namespace intersection
    {
        //-----------------------------------------------------------------------------------
        INTERSECTION FrustumIntersectsAABB(const CFrustum& frustum, Vec3 vBMin, Vec3 vBMax)
        {
            bool bIntersetion = false;
            Vec3 vMin, vMax;
            for (int k = 0; k < 4; k++)
            {
                // x
                if (frustum.GetFrustumClippingPlane(k).x >= 0)
                {
                    vMin.x = vBMin.x;
                    vMax.x = vBMax.x;
                }
                else
                {
                    vMin.x = vBMax.x;
                    vMax.x = vBMin.x;
                }
                // y
                if (frustum.GetFrustumClippingPlane(k).y >= 0)
                {
                    vMin.y = vBMin.y;
                    vMax.y = vBMax.y;
                }
                else
                {
                    vMin.y = vBMax.y;
                    vMax.y = vBMin.y;
                }
                // z
                if (frustum.GetFrustumClippingPlane(k).z >= 0)
                {
                    vMin.z = vBMin.z;
                    vMax.z = vBMax.z;
                }
                else
                {
                    vMin.z = vBMax.z;
                    vMax.z = vBMin.z;
                }

                Vec3 vNormal = Vec3(frustum.GetFrustumClippingPlane(k).x,
                    frustum.GetFrustumClippingPlane(k).y,
                    frustum.GetFrustumClippingPlane(k).z);

                if (glm::dot(vNormal, vMin) + frustum.GetFrustumClippingPlane(k).w > 0)
                    return INTERSECTION_OUTSIDE;

                if (glm::dot(vNormal, vMax) + frustum.GetFrustumClippingPlane(k).w >= 0)
                    bIntersetion = true;
            }

            if (bIntersetion)
                return INTERSECTION_INTERSECTING;
            else
                return INTERSECTION_INSIDE;
        }

        //-----------------------------------------------------------------------------------
        INTERSECTION FrustumIntersectsBB(const CFrustum& frustum, const CBoundingVolume& box, const Matrix4x4& mtxBBoxTransform)
        {
            Vec3 vRight = box.GetRight();
            Vec3 vUp = box.GetUp();
            Vec3 vForward = box.GetForward();
            Vec3 vHalfLengths = box.GetHalfLengths();
            Vec3 vOrigin = box.GetOrigin();

            Vec3 vPoints[8];
            int l = 0;
            for (int i = -1; i < 2; i += 2)
            {
                // right
                Vec3 v1 = vOrigin + (float)i * vRight * vHalfLengths[0];

                for (int j = -1; j < 2; j += 2)
                {
                    // Up
                    Vec3 v2 = v1 + (float)j * vUp * vHalfLengths[1];

                    for (int k = -1; k < 2; k += 2, ++l)
                    {
                        Vec3 v3 = v2 + (float)k * vForward * vHalfLengths[2];
                        // Forward
                        vPoints[l] = v3;
                    }
                }
            }

            for (int i = 0; i < 8; ++i)
            {
                vPoints[i] = Vec3(Vec4(vPoints[i], 1.0f) * mtxBBoxTransform);
            }


            INTERSECTION result = INTERSECTION_INSIDE;
            int pointsOut = 0,
                pointsIn = 0;

            for (int i = 0; i < 4; i++)
            {
                pointsOut = 0;
                pointsIn = 0;

                for (int k = 0; k < 8 && (pointsIn == 0 || pointsIn == 0); k++)
                {
                    float distance = glm::dot(frustum.GetFrustumClippingPlane(i), Vec4(vPoints[k], 1.0f));

                    if (distance < 0)
                        pointsOut++;
                    else
                        pointsIn++;
                }
                //if all corners are out
                if (pointsIn == 0)
                    return INTERSECTION_OUTSIDE;
                // if some corners are out and others are in	
                else if (pointsOut > 0)
                    result = INTERSECTION_INTERSECTING;
            }

            return result;
        }

        //-----------------------------------------------------------------------------------
        bool FrustumIntersectsPoint(const CFrustum& frustum, const Vec3& vPoint)
        {
            for (int i = 0; i < 5; i++)
            {
                if (glm::dot(frustum.GetFrustumClippingPlane(i), Vec4(vPoint, 1.0f)))
                    return false;
            }

            return true;
        }

        //-----------------------------------------------------------------------------------
        bool FrustumIntersectsSphere(const CFrustum& frustum, const Vec3& vCenter, float radius)
        {
            INTERSECTION intersection = INTERSECTION_INSIDE;
            for (int i = 0; i < 5; ++i)
            {
                float distance = glm::dot(frustum.GetFrustumClippingPlane(i), Vec4(vCenter, 1.0f));

                if (distance < -radius)
                    return false;
                else if (distance < radius)
                    intersection = INTERSECTION_INTERSECTING;
            }

            return true;

        }

        //-----------------------------------------------------------------------------------
        bool RayIntersectsBB(const CBoundingVolume& bbox, const Vec3& vOrigin, const Vec3& vDirection, float* pDistance)
        {
            if (pDistance)
                *pDistance = 0.0f;

            float eps = std::pow(10.0f, FLT_MIN_10_EXP);
            float tmin = FLT_MIN;
            float tmax = FLT_MAX;
            Vec3 p = bbox.GetOrigin() - vOrigin;


            // x
            float halfLengths[3];
            halfLengths[0] = bbox.GetHalfLengths().x;
            halfLengths[1] = bbox.GetHalfLengths().y;
            halfLengths[2] = bbox.GetHalfLengths().z;

            Vec3 vOrientation[3];
            vOrientation[0] = bbox.GetRight();
            vOrientation[1] = bbox.GetUp();
            vOrientation[2] = bbox.GetForward();

            for (int i = 0; i < 3; ++i)
            {
                float e = glm::dot(vOrientation[i], p);
                float f = glm::dot(vOrientation[i], vDirection);
                if (std::abs(f) > eps)
                {
                    float t1 = (e + halfLengths[i]) / f;
                    float t2 = (e - halfLengths[i]) / f;
                    if (t1 > t2)
                        std::swap(t1, t2);
                    if (t1 > tmin)
                        tmin = t1;
                    if (t2 < tmax)
                        tmax = t2;
                    if (tmin > tmax)
                        return false;
                    if (tmax < 0)
                        return false;
                }
                else if (-e - halfLengths[i] > 0 || -e + halfLengths[i] < 0)
                    return false;
            }

            if (tmin > 0)
            {
                if (pDistance)
                    *pDistance = tmin;
                return true;
            }
            else
            {
                if (pDistance)
                    *pDistance = tmax;
                return true;
            }
            return true;
        }
    } // intersection

      //-----------------------------------------------------------------------------------
    bool ComputeIntersectionRayPlane(const Vec3& orig, const Vec3& dir,
        const Vec3& planeOrig, const Vec3& planeNormal,
        float& intersectionDistance)
    {
        float d = glm::dot(dir, planeNormal);
        float epsilon = std::numeric_limits<float>::epsilon();

        if (d < epsilon || d > epsilon)
        {
            intersectionDistance = glm::dot(planeOrig - orig, planeNormal) / d;
            return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    void ComputeIntersection3D(Vec3& intersection, const Vec3& rayOrigin, const Vec3& rayDirection, const Vec3& planeOrigin, const Vec3& planeNormal)
    {
        float distance = 0.0f;
        glm::intersectRayPlane(rayOrigin, rayDirection, planeOrigin, planeNormal, distance);
        intersection = rayOrigin + glm::normalize(rayDirection) * distance;
    }

    //-----------------------------------------------------------------------------------
    void ComputeIntersection3D(Vec3& intersection, const Vec3& rayOrigin, const Vec3& rayDirection, const Vec3& p0, const Vec3& p1, const Vec3& p2)
    {
        Vec3 bary;
        glm::intersectRayTriangle(rayOrigin, rayDirection, p0, p1, p2, bary);

        intersection = bary.x * p0 + bary.y * p1 + bary.z * p2;
    }

    //-----------------------------------------------------------------------------------
    bool ComputeIntersection2D(const Vec2& p1, const Vec2& p2,
        const Vec2& p3, const Vec2& p4, Vec2* pIntersection, float* pS, float* pT)
    {
        if (pIntersection)
            *pIntersection = Vec2(0, 0);

        float denominator = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

        if (Equals(denominator, 0.0f))
            return false;

        float numeratorFirst = ((p1.y - p3.y) * (p4.x - p3.x)) - ((p1.x - p3.x) * (p4.y - p3.y));
        float numeratorSecond = ((p1.y - p3.y) * (p2.x - p1.x)) - ((p1.x - p3.x) * (p2.y - p1.y));

        if (pIntersection)
        {
            //pIntersection->x = ((p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x)) / denominator;
            //pIntersection->y = ((p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x)) / denominator;

            float s = numeratorFirst / denominator;

            *pIntersection = p1 + (p2 - p1) * s;

            if (pS)
            {
                *pS = numeratorFirst / denominator;
            }

            if (pT)
            {
                *pT = numeratorSecond / denominator;
            }
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool ComputeIntersection3D(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4,
        Vec3* pIntersection, float* pS)
    {
        Vec3 da = p2 - p1;
        Vec3 db = p4 - p3;
        Vec3 dc = p3 - p1;

        Vec3 vCross1(0, 0, 0), vCross2(0, 0, 0), vCross3(0, 0, 0);
        vCross1 = glm::cross(da, db);
        if (glm::dot(dc, vCross1) != 0.0f) // lines are not coplanar
            return false;

        vCross1 = glm::cross(dc, db);
        vCross2 = glm::cross(da, db);
        vCross3 = glm::cross(da, db);

        float s = glm::dot(vCross1, vCross2) / (vCross3.length() * glm::length(vCross3));
        if (s >= 0.0 && s <= 1.0)
        {
            if (pIntersection)
                *pIntersection = p1 + da * s;

            if (pS)
                *pS = s;

            return true;
        }

        return false;
    }
}