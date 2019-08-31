/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ACCELERATOR_ENTITY_H
#define ACCELERATOR_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>

namespace env
{
    class CAcceleratorEntity : public CMeshEntity
    {
    public:
        enum TRIGGERABLE_EVENTS
        {
            ACCELERATOR_EVENT_SETDISABLED = MESH_EVENT_END,
            ACCELERATOR_EVENT_SETBLUEPARTICLES
        };
        
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CAcceleratorEntity();

        virtual ~CAcceleratorEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        virtual Vec3 GetForce();

        void SetStrength(float velo) { m_strength = velo; }
        float GetStrength() const { return m_strength; }

        void SetDuration(float duration) { m_duration = duration; }
        float GetDuration() const { return m_duration; }

        void SetDisabled(bool bDisabled) { m_bDisabled = bDisabled; }
    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_strength;
            ar & m_duration;
            ar & m_pParticles;
        }

    protected:
        float m_strength;
        float m_duration;

        CParticleSystemEntity* m_pParticles;

        void SetupSounds() override;

        bool m_bDisabled;
    };



    class CAcceleratorClusterEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CAcceleratorClusterEntity();

        virtual ~CAcceleratorClusterEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void SetMeshFilename(const std::string& strName);

        void SetOffsetRadius(float radius);
        float GetOffsetRadius() const { return m_offsetRadius; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            m_version = version;

            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_magnets;

            if (version > 0)
                ar & m_offsetRadius;
        }

        void UpdateAccelerators();

    private:
        std::vector< CAcceleratorEntity* > m_magnets;

        float m_offsetRadius;

        unsigned int m_version;

    };


    class CAcceleratorBossEntity : public CAcceleratorEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CAcceleratorBossEntity();

        virtual ~CAcceleratorBossEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CAcceleratorEntity >(*this);
        }
    };

} // env

#endif // ACCELERATOR_ENTITY_H 