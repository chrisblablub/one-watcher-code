/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MAGNET_ENTITY_H
#define MAGNET_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/SoundSpriteEntity.h>

namespace env
{
    class CMagnetEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CMagnetEntity();

        virtual ~CMagnetEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);

        void SetSpeed(float speed) { m_speed = speed; }
        float GetSpeed() const { return m_speed; }

        void SetTime(float time) { m_time = time; }
        float GetTime() const { return m_time; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);

            if (version > 0)
            {
                ar & m_pParticles;
                ar & m_speed;
                ar & m_time;
            }

            if (version > 1)
                ar & m_pSound;
        }

    protected:
        float m_speed;
        float m_time;
        CEntity* m_pParticles;

        CSoundSpriteEntity* m_pSound;
        void SetupSounds();
    };


    class CMagnetClusterEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CMagnetClusterEntity();

        virtual ~CMagnetClusterEntity();
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

        void UpdateMagnets();

    private:
        std::vector< CMagnetEntity* > m_magnets;

        float m_offsetRadius;

        unsigned int m_version;

    };


//     class CMagnetBossEntity : public CMagnetEntity
//     {
//     public:
//         static ENTITY_TYPEID ENTITY_META_TYPEID;
// 
//         CMagnetBossEntity();
// 
//         virtual ~CMagnetBossEntity();
//         virtual void InitializeAtCreation();
//         virtual void InitializeAtLoad();
//         virtual void Update(float dt);
//         virtual void OnGameModeChanged(bool bGameMode);
// 
// 
//     private:
//         friend class boost::serialization::access;
// 
//         template< class Archive > void serialize(Archive& ar, const unsigned int version)
//         {
//             ar & boost::serialization::base_object< CMagnetEntity >(*this);
//         }
//     };

} // env

#endif // MAGNET_ENTITY_H 