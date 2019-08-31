/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TIMELAPSE_ENTITY_H
#define TIMELAPSE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>

namespace env
{
    class CTimelapseEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            TIMELAPSE_EVENT_RESET = SPRITE_EVENT_END,
            TIMELAPSE_EVENT_EXECUTE
        };

        CTimelapseEntity();

        virtual ~CTimelapseEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_timeMultiplier;
        }

    protected:
        float m_timeMultiplier;
    };
} // env

#endif // TIMELAPSE_ENTITY_H 