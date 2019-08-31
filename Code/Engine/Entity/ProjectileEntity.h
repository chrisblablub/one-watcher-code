/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PROJECTILE_ENTITY_H
#define PROJECTILE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/ExplosionEntity.h>

namespace env
{



    class CProjectileEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CProjectileEntity();
        virtual ~CProjectileEntity();

        virtual void Update(float dt);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void OnGameModeChanged(bool bGameMode);

        void SetCurvedPath(bool isCurvedPath);
        void SetFixedTargetPosition(Vec3 targetPosition);
        void SetTargetEntity(CEntity* targetEntity);
        void SetLifetime(float lifetime) { m_currentLifetime = lifetime; }
        void SetSpeed(float speed) { m_Speed = speed; }
        void SetAgility(float agility) { m_Agility = agility; }
        void AddSecondaryTarget(CEntity* pEntity) { m_secondaryTargets.push_back(pEntity); }

    protected:
        void SetupSounds() override;
        float m_Speed;

        int m_Damage;

        bool m_bTriggered;

        bool m_bInitial;
        bool m_IsCurvedPath;
        CEntity* m_TargetEntity;

        std::vector<CEntity*> m_secondaryTargets;

        bool m_UseFixedTargetPosition;
        Vec3 m_FixedTargetPosition;

        void Explode();

        CExplosionEntity* m_pExplosion;

        float m_Agility;
        float m_currentLifetime;
        float m_currentKilltime;

    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
        }
    };


    class CProjectileGunEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            PROJECTILE_GUN_EVENT_SET_ENABLED = MESH_EVENT_END,
            PROJECTILE_GUN_EVENT_SET_FIRE_RATE,
            PROJECTILE_GUN_EVENT_SET_LIFETIME,
            PROJECTILE_GUN_EVENT_SET_SPEED,
            PROJECTILE_GUN_EVENT_SET_AGILITY
        };

        CProjectileGunEntity();
        virtual ~CProjectileGunEntity();

        virtual void Update(float dt);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        inline bool GetDisabled() const { return m_bDisabled; }
        inline void SetDisabled(bool bDisabled) { m_bDisabled = bDisabled; if (!m_bDisabled) m_fireAccumTime = 1000.0f; }

        void FireProjectiles(int num);
        inline int GetNumProjectilesToFire() const { return m_numProjectiles; }

        inline float GetProjectileRemainingLifetime() const { return m_projectileTime; }

    private:
        void Initialize();

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
                ar & m_pFan;
        }

        bool m_bDisabled;

        float m_fireRate;
        float m_fireAccumTime;
        float m_lifeTime;
        float m_speed;
        float m_agility;

        float m_fireAnimTime;

        float m_projectileTime;


        CEntity* m_pTarget;
        CMeshEntity* m_pFan;

        std::list<CEntity*> m_secondaryTargets;

        bool m_bDoDamage;

        int m_numProjectiles;
    };

} // env


#endif // PROJECTILE_ENTITY_H 