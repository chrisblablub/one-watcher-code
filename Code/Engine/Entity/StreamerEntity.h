/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef STREAMER_ENTITY_H
#define STREAMER_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Scene/TerrainCollidable.h>
#include <Engine/Core/Input.h>

namespace env
{
    enum STREAMER_TYPE
    {
        STREAMER_TYPE_SMALL = 0,
        STREAMER_TYPE_BIG
    };

    class CStreamerEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS 
        {
            STREAMER_EVENT_SET_DISABLED = ENTITY_EVENT_END + 40
        };

        CStreamerEntity();
        virtual ~CStreamerEntity();

        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);

        void SetBurnPause(float del);
        float GetBurnPause();

        void SetBurnDuration(float dur);
        float GetBurnDuration();

        void SetType(STREAMER_TYPE type);
        STREAMER_TYPE GetType() const { return m_type; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_pParticleSys;
            ar & m_burnPause;
            ar & m_burnDuration;

            if (version > 0)
                ar & m_type;
        }

    protected:
        CParticleSystemEntity* m_pParticleSys;
        void SetupSounds() override;

        bool m_bDisabled;

        float m_burnPause;
        float m_burnDuration;

        float m_particleLifetime;
        float m_particleSpeed;

        float m_currentBBoxExpandTime;
        float m_currentBBoxShrinkTime;
        float m_currentBurnDuration;
        float m_currentBurnPause;

        bool m_bBurning;

        STREAMER_TYPE m_type;
    };

} // env

#endif // STREAMER_ENTITY_H 