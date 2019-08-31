/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SUN_ENTITY_H
#define SUN_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>

namespace env
{
    class CSunEntity : public CLightSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
        };

        CSunEntity();
        virtual ~CSunEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CLightSpriteEntity >(*this);

        }

    protected:
        CParticleSystemEntity* m_pSunParticles;

        SunRenderObject m_sunRenderObject;
    };



} // env

#endif // SUN_ENTITY_H