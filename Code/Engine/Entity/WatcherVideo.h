/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef WATCHER_VIDEO_ENTITY_H
#define WATCHER_VIDEO_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Graphic/RenderObjects/WatcherVideoRenderObject.h>

namespace env
{
    const float WATCHER_SLEEPING_EYE_PITCH = -PI * 0.5f;

    class CWatcherVideoEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            WATCHER_VIDEO_EVENT_WAKE_EYE = MESH_EVENT_END,
            WATCHER_VIDEO_EVENT_SLEEP_EYE,
            WATCHER_VIDEO_EVENT_START_EYE_FAN,
            WATCHER_VIDEO_EVENT_STOP_EYE_FAN,
            WATCHER_VIDEO_EVENT_START_LEFT_EAR_FAN,
            WATCHER_VIDEO_EVENT_STOP_LEFT_EAR_FAN,
            WATCHER_VIDEO_EVENT_START_RIGHT_EAR_FAN,
            WATCHER_VIDEO_EVENT_STOP_RIGHT_EAR_FAN,
            WATCHER_VIDEO_EVENT_START_ILLUMINATION,
            WATCHER_VIDEO_EVENT_STOP_ILLUMINATION,
            WATCHER_VIDEO_EVENT_ROTATE_PLANET,
            WATCHER_VIDEO_EVENT_ROTATE_EYE
        };

        CWatcherVideoEntity();
        virtual ~CWatcherVideoEntity();


        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        float GetIllumination() const { return m_illumination; }
    private:

        void Initialize();

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
            {
                ar & m_pEye;
                ar & m_pEyeFan;
                ar & m_pEarLeftFanOuter;
                ar & m_pEarLeftFanInner;
                ar & m_pEarRightFanInner;
                ar & m_pEarRightFanOuter;
            }

            if (version > 1)
                ar & m_pEyeParticles;
        }

    protected:

        CMeshEntity* m_pEye;
        CMeshEntity* m_pEyeFan;
        CMeshEntity* m_pEarLeftFanOuter;
        CMeshEntity* m_pEarLeftFanInner;
        CMeshEntity* m_pEarRightFanOuter;
        CMeshEntity* m_pEarRightFanInner;
        CParticleSystemEntity* m_pEyeParticles;

        bool m_bWakeEye;
        bool m_bSleepEye;
        float m_currentWakeEyeTime;
        float m_currentSleepEyeTime;

        float m_illumination;

        float m_currentIllumFadeInTime;
        float m_currentIllumFadeOutTime;

        float m_currentRotationPlanetTime;
        float m_rotationPlanetDuration;

        float m_currentRotationEyeTime;
        float m_rotationEyeDuration;

        bool m_bIllumFadingIn;
        bool m_bIllumFadingOut;

        WatcherVideoRenderObject m_watcherVideoRenderObject;

    };
} // env
#endif // WATCHER_VIDEO_ENTITY_H