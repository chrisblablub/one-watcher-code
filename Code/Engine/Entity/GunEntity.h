/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/SmallRocketEntity.h>

namespace env
{
    const unsigned int LASER_NUM_BOUNCES = 3;

    enum LASER_TYPE
    {
        LASER_TYPE_STANDARD = 0,
        LASER_TYPE_BOUNCING
    };

    class CGunEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { GUN_EVENT_ENABLE = MESH_EVENT_END, GUN_EVENT_DISABLE, GUN_EVENT_END };

        CGunEntity();
        virtual ~CGunEntity();

        virtual void Update(float dt);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnSerialization(bool bLoad);

        void SetLaserType(LASER_TYPE type);
        LASER_TYPE GetLaserType() const { return m_laserType; }

        bool GetLaser() const { return m_bLaser; }

        void SetBulletType(BULLET_TYPE type);
        BULLET_TYPE GetBulletType() const { return m_bulletType; }

        void SetTarget(CEntity* pTarget) { m_pTarget = pTarget; }
        CEntity* GetTarget() const { return m_pTarget; }

        void SetRadius(float radius);
        float GetRadius();

        void SetFireRate(float rate);
        float GetFireRate();

        void SetRotationSpeed(float speed);
        float GetRotationSpeed();

    private:
        void CreateLaserSprite();

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_radius;
            ar & m_fireRate;
            ar & m_rotationSpeed;
            ar & m_pTarget;
            ar & m_bLaser;
            ar & m_laserType;
            ar & m_bulletType;
        }

    private:
        bool m_bGunAlwaysEnabled;
        bool m_bGunEnabled;
        bool m_bLaserActive;

        CEntity* m_pTarget;

        std::vector< CSpriteEntity* > m_laserSprites;

        float m_radius;
        float m_fireRate;
        float m_rotationSpeed;

        float m_currentLaserLifetime;
        float m_timeRotationLocked;

        bool m_bLaser;

        LASER_TYPE m_laserType;
        BULLET_TYPE m_bulletType;

    };
} // env

#endif // GUN_ENTITY_H 