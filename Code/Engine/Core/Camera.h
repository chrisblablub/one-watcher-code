/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/AttachableObject.h>
#include <Graphic/RenderObjects/CameraRenderObject.h>
#include <Common/Core/Frustum.h>

namespace env
{
    struct RenderQueue;

    class CCamera : public CAttachableObject
    {
    public:
        static const std::string ENTITY_META_TYPENAME;

        enum PROJECTION_TYPE
        {
            PT_PERSPECTIVE = 1,
            PT_ORTHO
        };


        CCamera(float aspectRatio = 1.0f, float fov = PI * 0.25f, float zNear = 1.0f, float zFar = 100.0f);
        CCamera(const CFrustum& frustum);

        ~CCamera();
        void Update();
        void AddToRenderQueue(float dt, RenderQueue& renderQueue);

        void Reset();
        void BuildViewMatrix();
        void BuildProjectionMatrix();
        void SetProjectionType(PROJECTION_TYPE projection);

        CFrustum& GetFrustum();
        void SetFrustum(const CFrustum& frustum);

        float GetFrustumHalfWidth(float z);
        float GetFrustumHalfHeight(float z);

        const Matrix4x4& GetViewMatrix();
        const Matrix4x4& GetProjectionMatrix();
        Matrix4x4 GetViewProjectionMatrix() const;

        void SetDefaultPosition(const Vec3& defaultPosition) { m_vDefaultPosition = defaultPosition; }
        const Vec3& GetDefaultPosition();

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CAttachableObject >(*this);
            ar & m_frustum;
            ar & m_projection;
        }

    private:
        Matrix4x4 m_mtxView;
        Matrix4x4 m_mtxProj;
        Matrix4x4 m_mtxViewProj;

        Vec3 m_vDefaultPosition;

        CFrustum m_frustum;

        PROJECTION_TYPE m_projection;

        CameraRenderObject m_cameraRenderObject;
    };

} // env

#endif // CAMERA_H

