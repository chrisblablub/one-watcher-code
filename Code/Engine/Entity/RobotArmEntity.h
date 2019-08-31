/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ROBOT_ARM_ENTITY_H
#define ROBOT_ARM_ENTITY_H


#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/SawbladeEntity.h>
#include <Engine/Entity/LightSpriteEntity.h>
#include <Common/Core/Timer.h>

namespace env
{

    class CRobotArmEntity : public CMeshEntity
    {
    public:
        enum ROBOT_ARM_STATE
        {
            ROBOT_ARM_STATE_IDLE,
            ROBOT_ARM_STATE_AIM,
            ROBOT_ARM_STATE_PREPAREHIT,
            ROBOT_ARM_STATE_HIT,
            ROBOT_ARM_STATE_REVERSEHIT
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CRobotArmEntity();
        virtual ~CRobotArmEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);

        void SetState(ROBOT_ARM_STATE state);

        void SetDelay(float delay) { m_delay = delay; }
        float GetDelay() const { return m_delay; }

        void SetHitTime(float time) { m_hitTime = time; }
        float GetHitTime() const { return m_hitTime; }

        void SetHitPauseTime(float time) { m_hitPauseTime = time; }
        float GetHitPauseTime() const { return m_hitPauseTime; }

        void SetReverseHitTime(float time) { m_reverseHitTime = time; }
        float GetReverseHitTime() const { return m_reverseHitTime; }

        void SetReverseHitPauseTime(float time) { m_reverseHitPauseTime = time; }
        float GetReverseHitPauseTime() const { return m_reverseHitPauseTime; }

        void SetMirror(bool bMirror) { if (bMirror) m_directionFactor = -1.0f; else m_directionFactor = 1.0f; }
        bool GetMirror() const { return m_directionFactor < 0.0f; }

    private:
        //void ComputeArmAngles(float& distance, float rootRotationSpeed, float upperArmRotationSpeed, float lowerArmRotationSpeed);
        //void RotateToAngles(float rootRoll, float upperArmRoll, float lowerArmRoll, float rootRotationSpeed, float upperArmRotationSpeed, float lowerArmRotationSpeed);

        float ComputeRootAngle();
        float UpdateRootAngle(float rootAngle, float rotationSpeed);

        float ComputeUpperArmAngle();

        float ComputeLowerArmAngle(float upperArmRollAngle);
        float UpdateLowerArmAngle(float lowerArmAngle, float rotationSpeed);

        void UpdateAim(float dt);
        void UpdateHit(float dt);
        void UpdateReverseHit(float dt);

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            //ar & m_pUpperArm;
            ar & m_pLowerArm;
            ar & m_entities;
            ar & m_pTarget;
            ar & m_upperArmLength;
            ar & m_lowerArmLength;
            ar & m_aimPauseTime;
            ar & m_hitPauseTime;
            ar & m_hitTime;

            if (version > 0)
                ar & m_delay;

            if (version > 1)
                ar & m_directionFactor;

            if (version > 2)
            {
                ar & m_reverseHitTime;
                ar & m_reverseHitPauseTime;
            }
        }

    protected:
        CTimer m_stateTimer;

        ROBOT_ARM_STATE m_state;

        //CEntity* m_pUpperArm;
        CEntity* m_pLowerArm;
        std::vector< CEntity* > m_entities;
        CEntity* m_pTarget;
        CEntity* m_pCharacter;
        CCameraEntity* m_pPlayerCamera;

        Vec3 m_attackPos;

        float m_upperArmLength;
        float m_lowerArmLength;

        float m_upperArmAimRoll;
        float m_upperArmHitRoll;

        float m_aimPauseTime;
        float m_hitPauseTime;
        float m_reverseHitTime;
        float m_reverseHitPauseTime;
        float m_hitTime;

        float m_directionFactor;
        //float m_currentPauseTime;
        //float m_currentHitTime;
        float m_delay;

        void SetupSounds() override;
    };

    class CRobotArmT2Entity : public CRobotArmEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CRobotArmT2Entity();
        virtual ~CRobotArmT2Entity();

        virtual void InitializeAtCreation();
        virtual void Update(float dt);
    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CRobotArmEntity >(*this);
        }
    };

    class CRobotArmT3Entity : public CRobotArmEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CRobotArmT3Entity();
        virtual ~CRobotArmT3Entity();

        virtual void InitializeAtCreation();
        virtual void Update(float dt);
    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CRobotArmEntity >(*this);

            if (version > 0)
                ar & m_pLight;
        }

    private:
        CLightSpriteEntity* m_pLight;
    };

} // env



#endif // ROBOT_ARM_ENTITY_H 