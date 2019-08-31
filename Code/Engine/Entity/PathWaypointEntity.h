/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PATHWAYPOINTENTITY_H
#define PATHWAYPOINTENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>

namespace env
{
    class CPathEntity;

    class CPathWaypointEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CPathWaypointEntity();
        virtual ~CPathWaypointEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnFired(unsigned int eventId);
        virtual void OnJustPicked();
        virtual void OnJustUnpicked();

        void UnsetPath() { m_pNode->SetPosition(m_pNode->GetDerivedPosition()); m_pPath = nullptr; }
        void SetPath(CPathEntity* pPath);
        CPathEntity* GetPath();

        void SetSpeed(float speed);
        float GetSpeed();

        void SetAcceleration(float acceleration);
        float GetAcceleration();

        void SetBreakAcceleartion(float acc) { m_breakAcceleration = acc; }
        float GetBreakAcceleartion() const { return m_breakAcceleration; }

        void SetPause(float pause);
        float GetPause();

    private:
        friend class boost::serialization::access;

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_speed;
            ar & m_acceleration;
            ar & m_pause;
            ar & m_pPath;

            if (version > 0)
                ar & m_breakAcceleration;
            else
                m_breakAcceleration = m_acceleration;
        }

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_speed;
            ar & m_acceleration;
            ar & m_pause;
            ar & m_pPath;

            if (version > 0)
                ar & m_breakAcceleration;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    private:
        float m_speed;
        float m_acceleration;
        float m_breakAcceleration;
        float m_pause;
        CPathEntity* m_pPath;
    };

} // env


#endif /* PATHWAYPOINTENTITY */