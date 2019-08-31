/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <Common/Common.h>
#include <Common/Core/Serialize.h>

namespace env
{

    class CFrustum
    {
    public:
        CFrustum();
        CFrustum(float n, float f, float aspectRatio, float fov);

        void ComputeFrustumPoints(Vec3* pPoints, const Vec3& vPosition, const Vec3& vUp,
                                  const Vec3& vRight, const Vec3& vDirection);

        void SetViewProj(const Matrix4x4& mtxViewProj);
        void SetNearClipping(float nearClipping);
        void SetFarClipping(float farClipping);
        void SetAspectRatio(float aspectRatio);
        void SetFieldOfView(float fov);
        void SetWidth(float width);
        void SetHeight(float height);


        inline float GetNearClipping() const { return m_nearClipping; }
        inline float GetFarClipping()  const { return m_farClipping; }
        inline float GetAspectRatio()  const { return m_aspectRatio; }
        inline float GetFieldOfView()  const { return m_fov; }
        inline float GetWidth()        const { return m_width; }
        inline float GetHeight()       const { return m_height; }

        void GetNearHalfSize(Vec2& nearHalfSize) const;

        inline const Vec4& GetFrustumClippingPlane(unsigned int index) const { return m_ViewFrustum[index]; }

    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_nearClipping;
            ar & m_farClipping;
            ar & m_aspectRatio;
            ar & m_fov;
            ar & m_width;
            ar & m_height;
        }

    private:
        float m_nearClipping;
        float m_farClipping;
        float m_aspectRatio;
        float m_fov;
        float m_width;
        float m_height;

        Matrix4x4 m_mtxViewProj;
        Vec4 m_ViewFrustum[5];
    };
} // env

#endif // FRUSTUM_H