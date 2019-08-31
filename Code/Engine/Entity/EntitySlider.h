/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENTITYMOVER_H
#define ENTITYMOVER_H


#include <Engine/EngineCommon.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/AttachableObject.h>
#include <Engine/Scene/MeshInstance.h>

namespace env
{
#define SLIDER_X 1<<1
#define SLIDER_Y 1<<2
#define SLIDER_Z 1<<3
#define SLIDER_XY 1<<4
#define SLIDER_YZ 1<<5
#define SLIDER_XZ 1<<6

    class CEntitySlider : public CAttachableObject
    {
    public:
        enum class TransformMode
        {
            NONE = 0,
            MOVE,
            ROTATE
        };

        enum class RotationMode
        {
            ROTATOR_NONE = 0,
            ROTATOR_X,
            ROTATOR_Y,
            ROTATOR_Z
        };

        CEntitySlider();
        ~CEntitySlider();

        void Reset();

        bool InitializeAtCreation();
        bool InitializeAtLoad();

        void Update(float dt, const Vec3& vOrigin, const Vec3& vRay, CCamera* pCam);

        bool IsSliderSelected();
        bool IsActive();

        void Attach(CSceneNode* pObject);
        void Rotate(CAttachableObject* pObject);
        void Detach();

        CSceneNode* GetAttachedNode() { return m_pAttached; }
        CSceneNode* GetSliderNode() { return m_pTranslatorAxes->GetAttachedNode(); }
        void SetMode(TransformMode mode);
        TransformMode GetMode();

        void EnableSnap(bool bSnap = true);
        bool IsSnapEnabled();

        void ToggleSnapStepSize();
        void SetSnapStepSize(float stepSize);
        float GetSnapStepSize();
        void DeselectSliders();

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_pSliderNode;
            ar & m_pTranslatorNode;
            ar & m_pRotatorNode;
            ar & m_pTranslatorAxes;
            ar & m_pRotator;

            if (version > 0)
                ar & m_pTranslatorPlanes;

            if (version > 1)
                ar & m_version;

            if (version > 2)
            {
                ar & m_translatorPlanes;
                ar & m_translatorAxes;
                ar & m_rotator;
            }
        }

        bool Initialize();

        void ShowSlider(bool bShowTranslator, bool bShowRotator);

        void UpdateTranslator(float dt, const Vec3& vOrigin, const Vec3& vRay, CCamera* pCam);
        void UpdateRotator(float dt, const Vec3& vOrigin, const Vec3& vRay, CCamera* pCam);

        Vec3 ComputeMousePosition(unsigned long selectedSlider, const Matrix4x4& mtxProj, const Matrix4x4& mtxViewInv, bool bLocal = false, CCamera* pCam = nullptr);
        void ComputeMouseRay(Vec3& vMouseRayOrigin, Vec3& vMouseRayDir, const Matrix4x4& mtxProj, const Matrix4x4& mtxViewInv);
        void SetSelectedTranslator(unsigned long sliders, bool bSelected);
        void SetSelectedRotator(RotationMode rotator, bool bSelected);
        bool m_bTranslatorActivated;

        unsigned long m_selectedSliders;
        float m_snapTranslatorStepSize;
        float m_snapRotatorStepSize;

        CSceneNode* m_pSliderNode;
        CSceneNode* m_pTranslatorNode;
        CSceneNode* m_pRotatorNode;
        CMeshInstance* m_pTranslatorAxes;
        CMeshInstance* m_pTranslatorPlanes;
        CMeshInstance* m_pRotator;
        CSceneNode* m_pAttached;

        CMeshInstanceContainer m_translatorPlanes;
        CMeshInstanceContainer m_translatorAxes;
        CMeshInstanceContainer m_rotator;

        unsigned int m_version;

        Vec3 m_vLastMousePos;
        Vec3 m_vLastSliderPos;
        Vec3 m_vParentPos;
        Vec3 m_vSnappedPosition;
        float m_snappedRotation;

        Vec2 m_vProjTangent;
        bool m_bSnap;
        bool m_bLocalRotatorTransform;
        bool m_bLocalTranslatorTransform;

        TransformMode m_mode;
        RotationMode m_selectedRotator;
    };
} // env
#endif