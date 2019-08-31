/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ROTATORENTITY_H
#define ROTATORENTITY_H


#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{
    class CPathEntity;
    class CMeshInstance;

    class CRotatorEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS 
        { 
            ROTATOR_EVENT_START = SPRITE_EVENT_END, 
            ROTATOR_EVENT_STOP, 
            ROTATOR_EVENT_END 
        };

        CRotatorEntity();
        virtual ~CRotatorEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void OnEntityCloned(CEntity& entity);

        void AttachNode(CSceneNode* pNode);
        void DetachNode();
        CSceneNode* GetAttachedNode();

        void SetSpeed(float speed);
        float GetSpeed();

        void SetReverse(bool bReverse);
        bool GetReverse();

        void SetAngle(float angle);
        float GetAngle();

        void SetRadius(float radius);
        float GetRadius();

        void SetStartAngle(float angle);
        float GetStartAngle();

        void SetStartPause(float pause);
        float GetStartPause();

        void SetEndPause(float pause);
        float GetEndPause();

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_speed;
            ar & m_angle;
            ar & m_startPause;
            ar & m_startAngle;
            ar & m_radius;
            ar & m_bReverse;
            ar & m_pAttachedNode;
            ar & m_endPause;
            ar & m_pRetainer;
        }

    private:
        float m_speed;
        float m_angle;
        float m_startPause;
        float m_endPause;
        float m_startAngle;

        float m_currentSpeed;
        float m_currentStartAngle;
        float m_currentEndAngle;
        float m_currentRoll;
        float m_currentPause;
        float m_radius;

        bool m_bReverse;
        bool m_bPaused;
        bool m_bPausedTriggered;
        bool m_bAtStart;

        CMeshEntity* m_pRetainer;

        CSceneNode* m_pAttachedNode;

        std::vector< std::string > m_retainerMeshNames;
    };

} // env


#endif // ROTATORENTITY_H 