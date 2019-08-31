/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MISSILE_ENTITY_H
#define MISSILE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/ExplosionEntity.h>

namespace env
{

    class CMissileEntity : public CMeshEntity
    {
    public:
        enum BOMB_STATE
        {
            BOMB_STATE_INACTIVE,
            BOMB_STATE_LOADING,
            BOMB_STATE_EXPLODING
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CMissileEntity();

        virtual ~CMissileEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);


        void SetState(BOMB_STATE state);
        BOMB_STATE GetState() const { return m_state; }

        void SetEnabled(bool bEnable);

        void SetLifetime(float lifetime) { m_lifeTime = lifetime; }
        float GetLifeTime() const { return m_lifeTime; }

        void SetTargetingSpeed(float speed) { m_targetingSpeed = speed; }
        float GetTargetingSpeed() const { return m_targetingSpeed; }

        void SetTargeting(bool bTargeting) { m_bTargeting = bTargeting; }
        float GetTargeting() const { return m_bTargeting; }

        float GetCurrentLifetime() const { return m_currentLifeTime; }
        float GetCurrentStateTime() const { return m_stateTimer.GetTime(); }

        void SetVelcotiy(const Vec3& velo) { m_velocity = velo; }


    private:
        void UpdateBombLoading(float dt);
        void UpdateBombExploding(float dt);

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_pExplosion;
        }

    protected:
        bool m_bEnabled;

        Vec3 m_velocity;
        Vec3 m_currentVelocity;

        bool m_bTargeting;


        float m_targetingSpeed;
        float m_currentLifeTime;
        float m_lifeTime;

        BOMB_STATE m_state;

        CTimer m_stateTimer;

        CExplosionEntity* m_pExplosion;

        void SetupSounds() override;
    };


    class CMissileGunEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CMissileGunEntity();

        virtual ~CMissileGunEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnEntityCloned(CEntity& entity);

        void SetFireDelayMin(float del) { m_fireDelayMin = del; UpdateBombs(); }
        float GetFireDelayMin() const { return m_fireDelayMin; }

        void SetFireDelayMax(float del) { m_fireDelayMax = del; UpdateBombs(); }
        float GetFireDelayMax() const { return m_fireDelayMax; }

        void SetLifetimeMin(float del) { m_lifeTimeMin = del; UpdateBombs(); }
        float GetLifetimeMin() const { return m_lifeTimeMin; }

        void SetLifetimeMax(float del) { m_lifeTimeMax = del; UpdateBombs(); }
        float GetLifetimeMax() const { return m_lifeTimeMax; }

        void SetVelocityMin(float imp) { m_velocityMin = imp; UpdateBombs(); }
        float GetVelocityMin() const { return m_velocityMin; }

        void SetVelocityMax(float imp) { m_velocityMax = imp; UpdateBombs(); }
        float GetVelocityMax() const { return m_velocityMax; }

        void SetMissileTargetingSpeed(float sp) { m_missileTargetingSpeed = sp; UpdateBombs(); }
        float GetMissileTargetingSpeed() const { return m_missileTargetingSpeed; }

        void SetMissilesTargeting(bool bTargeting) { m_bMissilesTargeting = bTargeting; UpdateBombs(); }
        float GetMissilesTargeting() const { return m_bMissilesTargeting; }

        void SetVisibilityRadius(float rad) { m_visibilityRadius = rad; }
        float GetVisibilityRadius() const { return m_visibilityRadius; }

        void SetVisibility(bool bVis) { m_bVisibility = bVis; }
        float GetVisibility() const { return m_bVisibility; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_fireDelayMin;
            ar & m_fireAccum;
            ar & m_lifeTimeMin;
            ar & m_bombs;
            ar & m_velocityMin;
            ar & m_lifeTimeMax;
            ar & m_velocityMax;
            ar & m_fireDelayMax;

            if (version > 0)
            {
                ar & m_missileTargetingSpeed;
                ar & m_bMissilesTargeting;
                ar & m_visibilityRadius;
                ar & m_bVisibility;
            }
        }

    protected:
        void UpdateBombs();


        float m_fireDelayMin;
        float m_fireDelayMax;
        float m_fireAccum;
        float m_fireDelay;
        float m_lifeTimeMin;
        float m_lifeTimeMax;
        float m_velocityMin;
        float m_velocityMax;

        float m_missileTargetingSpeed;
        float m_visibilityRadius;

        bool m_bMissilesTargeting;
        bool m_bVisibility;


        float m_currentRotatationTime;

        std::vector< CMissileEntity* > m_bombs;

        unsigned int m_currentNumActiveBombs;

        CEntity* m_pCharacter;

        void SetupSounds() override;
    };

    class CMissileGunAimingEntity : public CMissileGunEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CMissileGunAimingEntity();

        virtual ~CMissileGunAimingEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);

        void SetOffsetRadius(float radius);
        float GetOffsetRadius() const { return m_offsetRadius; }

        void SetTargetingSpeed(float speed) { m_targetingSpeed = speed; }
        float GetTargetingSpeed() const { return m_targetingSpeed; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMissileGunEntity >(*this);

            if (version > 0)
            {
                ar & m_offsetRadius;
                ar & m_targetingSpeed;
            }
        }

    protected:

        Vec3 m_prevPosition;

        float m_offsetRadius;
        float m_targetingSpeed;

        float m_initialRoll;

        bool m_bCurrentlyTargeting;
        //float m_currentTargetingTime;

        CEntity* m_pCharacter;


    };
} // env
#endif // MISSILE_ENTITY_H