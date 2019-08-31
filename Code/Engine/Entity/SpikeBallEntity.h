/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SPIKE_BALL_ENTITY_H
#define SPIKE_BALL_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{
    class CSpikeBallEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { SPIKEBALL_EVENT_START = MESH_EVENT_END, SPIKEBALL_EVENT_STOP, SPIKEBALL_EVENT_END };

        CSpikeBallEntity();
        virtual ~CSpikeBallEntity();

        virtual void Update(float dt);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
    protected:
        void SetupSounds() override;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
                ar & m_pMeshInner;
        }

        void InitMeshes();

    private:

        CMeshEntity* m_pMeshInner;
    };

} // env


#endif // SPIKE_BALL_ENTITY_H 