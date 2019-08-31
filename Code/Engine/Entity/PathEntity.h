/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PATHENTITY_H
#define PATHENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Scene/SceneNode.h>

namespace env
{
    class CPathWaypointEntity;

    struct Train
    {
        Train()
            : pAttachedNode(nullptr)
            , pPathNode(nullptr)
            , m_version(0)
            , startCornerInitial(0)
            , startCorner(0)
            , currentCorner(0) {}

        std::string strEntityName;
        CSceneNode* pAttachedNode;
        CSceneNode* pPathNode;

        float offset;
        int currentCorner;
        int startCornerInitial;
        int startCorner;
        bool bReverse;
        bool bFollowPlayer;
        float currentPause;
        float accumPause;
        float trackTime;
        float currentSpeed;

        unsigned int m_version;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & strEntityName;
            ar & pAttachedNode;
            ar & offset;
            ar & currentCorner;
            ar & startCornerInitial;
            ar & bReverse;
            ar & currentPause;
            ar & accumPause;
            ar & trackTime;

            if(version > 0)
                ar & bFollowPlayer;

            if(version > 1)
                ar & pPathNode;

            m_version = version;
        }

    };

    class CPathEntity : public CSpriteEntity
    {
    public:
        enum TRIGGERABLE_EVENTS 
        { 
            PATH_EVENT_START = SPRITE_EVENT_END, 
            PATH_EVENT_STOP, 
            PATH_EVENT_RESET, 
            PATH_EVENT_SET_START_CORNER,
            PATH_EVENT_END };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CPathEntity();
        virtual ~CPathEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnJustUnpicked();
        virtual void OnJustPicked();
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        

        void AddWaypoint(CPathWaypointEntity* pCorner);
        void RemoveWaypoint(const std::string& strCorner);

        Train* AttachNode(CSceneNode* pNode, const std::string& strEntityName);
        void DetachNode(CSceneNode* pNode);
        void DetachNode(const std::string& strEntityName);

        void SetLoop(bool bLoop);
        bool GetLoop();

        void SetReverse(bool bReverse);
        bool GetReverse();

        void SetPaused(bool bPaused);
        bool GetPaused();

        int GetNumWaypoints();

        CPathWaypointEntity* GetWaypoint(unsigned int index);

        const std::list< Train >& GetTrains();
        Train* GetTrain(const std::string& strEntityName);

        inline void SetRenderDebugWireframe(bool bSet) { m_bRenderDebugWireframe = bSet; }
        inline bool GetRenderDebugWireframe() const { return m_bRenderDebugWireframe; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_waypoints;
            ar & m_trains;
            ar & m_bReverse;
            ar & m_bLoop;
        }

    private:
        typedef std::pair< CPathWaypointEntity*, CMeshEntity* > pairWP;
        std::vector< pairWP > m_waypoints;
        
        std::list< Train > m_trains;

        //float m_trackTime;
        float m_levelTime;
        
        bool m_bReverse;
        bool m_bLoop;
        bool m_bPaused;
        
        bool m_bRenderDebugWireframe;

        CVector3Cache m_debugWireframe;

        CEntity* m_pCharacter;
    };
} // env


#endif // PATHENTITY_H 