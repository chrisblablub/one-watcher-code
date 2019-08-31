/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SAWBLADE_BOSS_ENTITY_H
#define SAWBLADE_BOSS_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>

namespace env
{
    const float FAN_ROTATION_SPEED = -4.0f;
    const float SAWBLADE_ROTATION_SPEED = -2.5f;

    struct EnergyShield
    {
        EnergyShield()
            : m_pEntity(0),
            m_illumination(0) {}

        CMeshEntity* m_pEntity;

        float m_illumination;

        Matrix4x4 mtxTransform;
    };

    class CSawbladeBossEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { SAWBLADE_BOSS_EVENT_WAKE = MESH_EVENT_END, SAWBLADE_BOSS_EVENT_SLEEP };

        CSawbladeBossEntity();
        virtual ~CSawbladeBossEntity();


        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

    private:

        void Initialize();

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
            {
                ar & m_pFan;
                ar & m_pCore;
            }

        }

    protected:

        EnergyShield m_energyShield;

        CMeshEntity* m_pFan;

        CParticleSystemEntity* m_pCore;


        float m_illumination;

        bool m_bWake;

        float m_currentWakeTime;
    };
} // env
#endif // SAWBLADE_BOSS_ENTITY_H