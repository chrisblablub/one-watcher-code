/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MINE_ENTITY_H
#define MINE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/RotatorEntity.h>
#include <Engine/Entity/ExplosionEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Core/PhysicsNode.h>

namespace env
{

    class CHunterBaseEntity : public CMeshEntity
    {
    public:
        enum HUNTER_STATE
        {
            HUNTER_STATE_PAUSED = 0,
            HUNTER_STATE_FLYINGTOTARGET,
            HUNTER_STATE_SEARCHING,
            HUNTER_STATE_ATTACKINGTARGET,
            HUNTER_STATE_INACTIVE
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { HUNTER_BASE_EVENT_END = MESH_EVENT_END + 10 };

        CHunterBaseEntity();
        virtual ~CHunterBaseEntity();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual bool HasCollision(CEntity& e);

        void SetTarget(CEntity* pEntity) { m_pTarget = pEntity; }
        CEntity* GetTarget() const { return m_pTarget; }

        void SetTargetPosition(const Vec3& targetPos) { m_pTarget = NULL; m_targetPos = targetPos; }

        void SetState(HUNTER_STATE state);

        void SetRadius(float radius) { m_radius = radius; }

        void SetAiming(float aiming) { m_aiming = aiming; }

        void SetTargetBraking(bool bTargetBraking) { m_bTargetBraking = bTargetBraking; }

    protected:
        virtual void UpdatePaused(float dt);
        virtual void UpdateFlyingToTarget(float dt);
        virtual void UpdateSearching(float dt);
        virtual void UpdateAttackingTarget(float dt);
        virtual void UpdateInactive(float dt);

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_startPos;
            ar & m_pTarget;
            ar & m_targetPos;
            ar & m_targetVelocity;
            ar & m_acceleration;

            if (version > 0)
                ar & m_radius;
        }

        void SetupSounds() override;


    protected:

        CPhysicsNode2D m_hunterPhysics;

        CTimer m_stateTimer;

        HUNTER_STATE m_currentState;

        Vec3 m_attackDir;
        Vec3 m_startPos;

        CCharacter2DEntity* m_pCharacter;

        CEntity* m_pTarget;
        Vec3 m_targetPos;

        float m_targetVelocity;
        float m_acceleration;
        float m_radius;
        float m_aiming;

        bool m_bTargetBraking;
        bool m_bCurrentTargetBraking;

        CollisionState m_collisionState;
    };

    const Vec3 HUNTINGHUNTER_SCALE = Vec3(1.2f, 1.2f, 1.2f);

    class CHuntingHunterEntity : public CHunterBaseEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            HUNTINGHUNTER_EVENT_ENABLE = HUNTER_BASE_EVENT_END,
            HUNTINGHUNTER_EVENT_DISABLE,
            HUNTINGHUNTER_EVENT_SPAWN,
            HUNTINGHUNTER_EVENT_VANISH,
            HUNTINGHUNTER_EVENT_END
        };

        enum HUNTINGHUNTER_STATE
        {
            HUNTINGHUNTER_STATE_ENABLED = 0,
            HUNTINGHUNTER_STATE_DISABLED,
            HUNTINGHUNTER_STATE_SPAWNING,
            HUNTINGHUNTER_STATE_VANISHING
        };

        CHuntingHunterEntity();
        virtual ~CHuntingHunterEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CHunterBaseEntity >(*this);
            ar & m_pParticleSys;

            if (version > 0)
                ar & m_pLight;
        }

    protected:
        CParticleSystemEntity* m_pParticleSys;
        CLightSpriteEntity* m_pLight;

        HUNTINGHUNTER_STATE m_state;

        float m_currentStateTime;
    };


    class CExplodingHunterEntity : public CHunterBaseEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CExplodingHunterEntity();
        virtual ~CExplodingHunterEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void SetPersistent(bool bSet);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void Update(float dt);
        void SetLifeTime(float lifeTime) { m_lifeTime = lifeTime; }

    protected:
        virtual void UpdateInactive(float dt);
        virtual void UpdateAttackingTarget(float dt);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CHunterBaseEntity >(*this);
            ar & m_pExplosion;

            if (version > 0)
                ar & m_pHunterCore;
        }

    protected:
        //CSprite m_sprite;
        //CSceneNode m_spriteNode;
        CSprite m_sprite;
        CSceneNode m_spriteNode;

        CExplosionEntity* m_pExplosion;
        CParticleSystemEntity* m_pHunterCore;
        CTimer m_lifeTimer;

        float m_lifeTime;
        float m_currentBlinkTime;
        bool m_bBlinkAppear;
    };

    class CExplodingHunterEmitterEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CExplodingHunterEmitterEntity();
        virtual ~CExplodingHunterEmitterEntity();

        virtual void Update(float dt);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);

        inline void SetEmitRate(float rate) { m_emitRate = rate; }
        inline float GetEmitRate() const { return m_emitRate; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_emitRate;
            ar & m_accumTime;
        }

    protected:
        virtual void OnEmitExplodingHunter();

    protected:
        float m_emitRate;
        float m_accumTime;
        float m_radius;

        bool m_bActive;

        CEntity* m_pCharacter;
    };

    class CExplodingHunterEmitterEntityT2 : public CExplodingHunterEmitterEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

    protected:
        virtual void InitializeAtCreation();
        virtual void OnEmitExplodingHunter();

    };
} // env

#endif // MINE_ENTITY_H 
