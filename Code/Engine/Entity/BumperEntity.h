/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BUMPER_ENTITY_H
#define BUMPER_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/AcceleratorEntity.h>

namespace env
{
    const float BUMPER_BUMP_TIME = 0.25f;

    class CBumperEntity : public CAcceleratorEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CBumperEntity();

        virtual ~CBumperEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);


        virtual Vec3 GetForce();

        void Bump();
        float GetCurrentBumpTime() const { return m_bumpTime; }

    protected:
        void SetupSounds() override;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_strength;
            ar & m_duration;

            if (version > 0)
                ar & m_pFan;

            m_version = version;
        }

    private:

        CEntity* m_pCharacter;

        CMeshEntity* m_pFan;

        CSprite m_sprite;

        float m_bumpTime;

        unsigned int m_version;

    };



} // env

#endif // BUMPER_ENTITY_H 