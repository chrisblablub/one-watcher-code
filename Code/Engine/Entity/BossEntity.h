/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BOSS_ENTITY_H
#define BOSS_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Animation/SceneNodeAnimation.h>
#include <Common/Core/StateMachine.h>
#include <queue>

namespace env
{
    class CProjectileGunEntity;

    class CBossEntity : public CMeshEntity
    {
        enum TRIGGERABLE_EVENTS
        {
            BOSS_EVENT_ACTIVATE = MESH_EVENT_END,
            BOSS_EVENT_SET_DEAD,
            BOSS_EVENT_ENABLE_GUN_LEFT,
            BOSS_EVENT_ENABLE_GUN_RIGHT,
            BOSS_EVENT_WAKE,
            BOSS_EVENT_ENABLE_FINAL_PHASE,
            BOSS_EVENT_END
        };

    public:
        enum BOSS_STATE
        {
            BOSS_STATE_NORMAL,
            BOSS_STATE_HIT,
            BOSS_STATE_CHARACTER_JUMP,
            BOSS_STATE_CHARACTER_HIT,
            BOSS_STATE_DYING
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CBossEntity();
        virtual ~CBossEntity();

        virtual void Update(float dt);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);


        void SetDead(bool bDead) { m_bIsDead = bDead; }

        void SetState(BOSS_STATE state)
        {
            m_stateMachine.SetNextStateID(state);
        }

    protected:
        typedef CStateMachineUpdate<CBossEntity> StateMachine;

        void InitStateMachine();
        void NormalUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void HitEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void HitUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void CharacterJumpUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void CharacterHitEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void CharacterHitUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void DyingEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void DyingUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);



        float m_FireDelay;
        float m_ReloadTime;
        float m_InitialExplosionTimer;
        float m_ExplosionTimer;


        float m_gunLeftEnableTime;
        float m_gunLeftEnableDuration;
        float m_gunRightEnableTime;
        float m_gunRightEnableDuration;

        bool m_bGunLeftEnabled;
        bool m_bGunRightEnabled;

        bool m_bFinalPhase;

        int m_numProjectiles;

        float m_laserTime;
        float m_laserDuration;

        bool m_IsActivated;
        bool m_bIsAwake;
        bool m_bIsDead;

        bool m_SoundWasPlayed;

        float m_wakeTime;
        float m_wakeDuration;

        StateMachine m_stateMachine;


        BOSS_STATE m_state;

        CMeshEntity* m_pEye;
        CMeshEntity* m_pInnerEye;
        CMeshEntity* m_pBody;
        CParticleSystemEntity* m_pInnerEyeParticles;

        CEntity* m_pCharacter;

        CProjectileGunEntity* m_pGunRight;
        CProjectileGunEntity* m_pGunLeft;

        std::vector<CEntity*> m_ExplosionEffects;
        std::queue<CSceneNodeAnimation> m_Animations;

        CLaserRadiatorAimingEntity* m_pLaserEntity;

        Vec4 m_innerEyeParticlesColor;

        void SetupSounds() override;
        void RemoveExplosionEffects();
        void SetupMeshes();
        void SetupWakeUpAnimation();
        void AddHitAnimation();
        void AddIdleAnimation();

        void AddExplosionEffects(float dt, int maxNumberOfEffects);
        void UpdateAnimations(float dt);

        bool IsDead();


    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_pEye;
            ar & m_pInnerEye;

            if (version > 1)
                ar & m_pBody;

            if (version > 2)
            {
                ar & m_pGunLeft;
                ar & m_pGunRight;
            }

            if (version > 3)
                ar & m_pLaserEntity;

            if (version > 4)
                ar & m_pInnerEyeParticles;
        }
    };

} // env


#endif // BOSS_ENTITY_H 