/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MATH_UTILITY_H
#define MATH_UTILITY_H

#include <Common/Common.h>


namespace env
{
    float GetRandomFloat(float a, float b);

    Vec3& GetRandomPoint(Vec3& vOut, const Vec3& vMin, const Vec3& vMax);

    Vec3& GetRandomVec(Vec3& vOut);

    Vec3& GetRandomVec(Vec3& vOut, const Vec3& vMin, const Vec3& vMax);

    Vec4& GetRandomPoint(Vec4& vOut, const Vec4& vMin, const Vec4& vMax);

    Vec4& GetRandomVec(Vec4& vOut);

    Vec4& GetRandomVec(Vec4& vOut, const Vec4& vMin, const Vec4& vMax);

    bool Equals(float a, float b);

    template< typename T > inline T Lerp(const T& x, const T& y, float p) 
    { 
        return (1 - p) * x + p * y; 
    }

    template< typename T > T Clamp(const T& val, const T& minVal, const T& maxVal)
    {
        T retVal = val;
        if (val > maxVal) retVal = maxVal;
        else if (val < minVal) retVal = minVal;
        return retVal;
    }

    template<> inline Vec4 Clamp<Vec4>(const Vec4& val, const Vec4& minVal, const Vec4& maxVal)
    {
        Vec4 v = val;
        v.x = Clamp(v.x, minVal.x, maxVal.x);
        v.y = Clamp(v.y, minVal.y, maxVal.y);
        v.z = Clamp(v.z, minVal.z, maxVal.z);
        v.w = Clamp(v.w, minVal.w, maxVal.w);
        return v;
    }

    void ComputeViewingRayDirection(const Matrix4x4& mtxProjection, const Matrix4x4& mtxView,
                                    unsigned long backbufferWidth, unsigned long backbufferHeight,
                                    float mouseX, float mouseY, Vec3& vRayDirection);


    float ComputePointOnLineProjection(const Vec3& point, const Vec3& linePos, const Vec3& lineDir);
    void ComputePointOnLineProjection(Vec3& projPos, const Vec3& point, const Vec3& linePos, const Vec3& lineEndPos);

    void ComputeReflectedVector(Vec3& reflected, const Vec3& incidentRay, const Vec3& normal);
    void ComputeReflectedVector(Vec2& reflected, const Vec2& incidentRay, const Vec2& normal);

    float ComputeRoll(const Vec3& v, const Vec3& baseAxis);

    float ComputeRoll(const Vec2& v, const Vec2& baseAxis);

    inline float ComputeRoll(const Vec2& v)
    {
        return std::atan2(-v[0], v[1]);
    }

    inline float ComputeRoll(const Vec3& v)
    {
        return std::atan2(-v.x, v.y);
    }

    bool ComputeRollRotationDirection(float* pRollDiff, float targetRoll, float initialRoll);

    void ComputeProjectionMatrix(Matrix4x4& mtxProjection, float l, float r, float t, float b, float n, float f);
    void ComputeProjectionMatrixOrtho(Matrix4x4& mtxProjection, float l, float r, float t, float b, float n, float f);


    float RadToDeg(float rad);
    float DegToRad(float deg);

    float sgn(float x);
} // env

#endif // MATH_UTILITY_H