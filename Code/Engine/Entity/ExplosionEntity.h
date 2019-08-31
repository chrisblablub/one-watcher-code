/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef EXPLOSION_ENTITY_H
#define EXPLOSION_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Entity/SoundSpriteEntity.h>

namespace env
{
    const float EXPLOSION_TIME = 2.0f;

    class CCameraEntity;

    class CExplosionEntity : public CParticleSystemEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CExplosionEntity();
        ~CExplosionEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();

        void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);

        virtual void SetPickable(bool bPickable);

        void SetEnabled(bool bEnable = true);

        bool GetExploding() const { return m_bExploding; }

        void SetRadius(float radius) { m_radius = radius; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CParticleSystemEntity >(*this);
            ar & m_pLight;

            if (version > 2)
                ar & m_pSound;
        }

    protected:
        bool m_bExploding;

        CSoundSpriteEntity* m_pSound;
        void SetupSounds();


        float m_radius;

        CTimer m_timer;
        CLightSpriteEntity* m_pLight;

        CCameraEntity* m_pCamera;
        CEntity* m_pCharacter;
    };
} // env

#endif // EXPLOSION_ENTITY_H