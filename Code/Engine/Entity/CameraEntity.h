/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CAMERA_ENTITY_H
#define CAMERA_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Core/Camera.h>

namespace env
{
    class CCameraEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            CAMERA_EVENT_ENABLE = MESH_EVENT_END,
            CAMERA_EVENT_SMOOTHENABLE,
            CAMERA_EVENT_DISABLE,
            CAMERA_EVENT_SHAKELIGHT,
            CAMERA_EVENT_SHAKESTRONG,
            CAMERA_EVENT_END,
            CAMERA_EVENT_SHAKE = CAMERA_EVENT_END + 15
        };

        CCameraEntity();

        virtual ~CCameraEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnResetDevice();
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        void SetTarget(CEntity* pTarget) { m_pTarget = pTarget; }
        CEntity* GetTarget() const { return m_pTarget; }

        void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
        float GetRotationSpeed() const { return m_rotationSpeed; }

        void SetSmoothEnableTime(float time) { m_smoothEnableTime = time; }
        float GetSmoothEnableTime() const { return m_smoothEnableTime; }

        void SetRollToTarget(bool bRollToTarget) { m_bRollToTarget = bRollToTarget; }
        bool GetRollToTarget() const { return m_bRollToTarget; }

        void Shake(float dur, const Vec2& shakeStrength = Vec2(-10.0f, 10.0f)) { m_currentShakeTime = dur; m_shakeStrength = shakeStrength; }

        CCamera* GetCamera() { return &m_camera; }

    protected:
        virtual void OnShake(float dt);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_pTarget;
            ar & m_rotationSpeed;
            ar & m_smoothEnableTime;
            ar & m_initialPos;
            ar & m_initialOrientation;

            if (version > 0)
                ar & m_bRollToTarget;
        }

    protected:

        CEntity* m_pTarget;

        CCamera m_camera;

        float m_smoothEnableTime;
        float m_currentSmoothEnableTime;
        float m_rotationSpeed;

        float m_currentShakeTime;
        Vec2 m_shakeStrength;
        Vec3 m_shakePosition;

        bool m_bRollToTarget;

        Vec3 m_smoothTargetPos;
        Vec3 m_smoothStartPos;
        CQuaternion m_smoothStartOrientation;
        CQuaternion m_smoothTargetOrientation;

        Vec3 m_initialPos;
        CQuaternion m_initialOrientation;


    };

    class CCharacter3DEntity;

    class CPlayerCameraEntity : public CCameraEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            PLAYERCAMERA_EVENT_ZOOMIN = CAMERA_EVENT_END,
            PLAYERCAMERA_EVENT_ZOOMOUT
        };

        CPlayerCameraEntity();

        virtual ~CPlayerCameraEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        void SetRespectLevelBoundaries(bool b) { m_bRespectLevelBoundaries = b; }
        bool GetRespectLevelBoundaries() const { return m_bRespectLevelBoundaries; }

    protected:

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CCameraEntity >(*this);
        }

    protected:
        CCharacter3DEntity* m_pCharacter;

        float m_zoomDistance;

        float m_zoomTime;
        float m_currentZoomInTime;
        float m_currentZoomOutTime;

        bool m_bRespectLevelBoundaries;
    };



} // env

#endif // CAMERA_ENTITY_H 