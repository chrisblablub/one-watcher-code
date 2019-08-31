/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LASER_RADIATOR_ENTITY_H
#define LASER_RADIATOR_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/RotatorEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>

namespace env
{
    const float LASER_TEXTURE_WIDTH = 128.0f;

    class CLaserRadiatorEntity : public CMeshEntity
    {
    public:
        enum LASER_STATE
        {
            LASER_STATE_PAUSED = 0,
            LASER_STATE_LOADING,
            LASER_STATE_ACTIVE
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { LASERRADIATOR_EVENT_ENABLE = MESH_EVENT_END, LASERRADIATOR_EVENT_DISABLE };

        CLaserRadiatorEntity();
        virtual ~CLaserRadiatorEntity();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void SetMeshFilename(const std::string& strName);

        void SetLaserCount(unsigned int numLasers);
        unsigned int GetLaserCount() const { return m_numLasers; }

        void SetLaserMaxDistance(float distance) { m_laserMaxDistance = distance; }
        float GetLaserMaxDistance() const { return m_laserMaxDistance; }

        void SetLaserPausingTime(float pauseTime) { m_laserPausingTime = pauseTime; }
        float GetLaserPausingTime() const { return m_laserPausingTime; }

        void SetLaserActiveTime(float activeTime) { m_laserActiveTime = activeTime; }
        float GetLaserActiveTime() const { return m_laserActiveTime; }

        void SetLaserLoadingTime(float loadingTime) { m_laserLoadingTime = loadingTime; }
        float GetLaserLoadingTime() const { return m_laserLoadingTime; }

        void SetLaserTerrainCollisions(bool bSet) { m_bLaserTerrainCollisions = bSet; }
        bool GetLaserTerrainCollisions() const { return m_bLaserTerrainCollisions; }

        void SetLaserDelay(float delay) { m_laserDelay = delay; }
        float GetLaserDelay() const { return m_laserDelay; }

        void SetLaserOffsetRadius(float rad) { m_laserOffsetRadius = rad; }
        float GetLaserOffsetRadius() const { return m_laserOffsetRadius; }

        void SetLaserColor(const Vec3& color) { m_laserColor = color; }
        const Vec3& GetLaserColor() const { return m_laserColor; }

        void SetAttachedMeshInstanceIndex(unsigned int index);
        unsigned int GetAttachedMeshInstanceIndex() const { return m_attachedMeshInstIndex; }

    protected:
        void SetupSounds() override;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_laserActiveTime;
            ar & m_laserPausingTime;
            ar & m_laserLoadingTime;
            ar & m_numLasers;

            if (version > 0)
                ar & m_laserMaxDistance;

            if (version > 1)
                ar & m_nodes;

            if (version > 2)
                ar & m_bLaserTerrainCollisions;

            if (version > 3)
                ar & m_laserDelay;

            if (version > 4)
                ar & m_laserOffsetRadius;

            if (version > 5)
                ar & m_laserColor;

            if (version > 6)
                ar & m_attachedMeshInstIndex;

            if (version > 7)
                ar & m_particles;
        }


    private:

        std::vector< CSprite > m_lasers;
        std::vector< CSceneNode* > m_nodes;
        std::vector< CParticleSystemEntity* > m_particles;

        unsigned int m_numLasers;
        unsigned int m_attachedMeshInstIndex;

        float m_laserActiveTime;
        float m_laserPausingTime;
        float m_laserLoadingTime;
        float m_laserDelay;
        float m_laserMaxDistance;
        float m_laserOffsetRadius;

        float m_currentLaserTime;
        float m_time;

        bool m_bDisabled;
        bool m_bLaserTerrainCollisions;
        bool m_bLaserPlatformCollisions;

        Vec3 m_laserColor;

        LASER_STATE m_currentLaserState;
    };

    class CLaserRadiatorAimingEntity : public CSpriteEntity
    {
    public:
        enum TRIGGERABLE_EVENTS 
        { 
            LASERRADIATOR_AIMING_EVENT_SET_RADIUS = SPRITE_EVENT_END ,
            LASERRADIATOR_AIMING_EVENT_SET_DISABLED
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CLaserRadiatorAimingEntity();
        virtual ~CLaserRadiatorAimingEntity();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        inline void AddTargetEntity(CEntity* pEntity) { m_targets.insert(pEntity); }
        inline void RemoveTargetEntity(CEntity* pEnttiy) { m_targets.erase(pEnttiy); if (m_pCurrentTarget == pEnttiy) m_pCurrentTarget = nullptr; }

        inline void SetRadius(float radius) { m_radius = radius; }
        inline  float GetRadius() const { return m_radius; }

        inline void SetOffset(float offset) { m_offset = offset; }
        inline float GetOffset() const { return m_offset; }

        inline void SetGrowDuration(float duration) { m_growDuration = duration; }
        inline float GetGrowDuration() const { return m_growDuration; }

        inline void SetShrinkDuration(float duration) { m_shrinkDuration = duration; }
        inline float GetShrinkDuration() const { return m_shrinkDuration; }

        const std::set<CEntity*>& GetTargetEntities() const { return m_targets; }

        inline bool GetLaserActive() const { return m_bLaserActive; }
        inline const Vec3& GetLaserTargetPos() const { return m_targetPos; }

        inline void SetDisabled(bool bDisabled) { m_bDisabled = bDisabled; }
        inline bool GetDisabled() const { return m_bDisabled; }

		void SetupSounds();

    private:

        void InitParticles();

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_pLaserNode;
            ar & m_targets;
            ar & m_laser;

            if (version > 0)
            {
                ar & m_radius;
                ar & m_growDuration;
                ar & m_shrinkDuration;
            }

            if (version > 1)
                ar & m_pLaserParticlesHitpoint;

            if (version > 2)
                ar & m_offset;
        }

        CSprite m_laser;
        CSceneNode* m_pLaserNode;
		CSoundSpriteEntity* m_pSound;

        CParticleSystemEntity* m_pLaserParticlesHitpoint;

        std::set<CEntity*> m_targets;

        bool m_bDisabled;
        bool m_bLaserActive;

        float m_radius;
        float m_offset;
        float m_growDuration;
        float m_shrinkDuration;

        float m_growTime;
        float m_shrinkTime;
        
        CEntity* m_pCharacter;
        CEntity* m_pCurrentTarget;

        Vec3 m_targetPos;

        Vec3 m_targetVelocity;

        
    };
} // env

#endif // LASER_RADIATOR_ENTITY_H 