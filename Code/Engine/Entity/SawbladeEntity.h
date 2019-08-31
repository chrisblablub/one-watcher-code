/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SIMPLESAWBLADE_H
#define SIMPLESAWBLADE_H


#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>


namespace env
{
    class CAudioCue;

    enum SAWBLADE_TYPE
    {
        SAWBLADE_TYPE_STANDARD = 0,
        SAWBLADE_TYPE_EXPANDING,
        SAWBLADE_TYPE_END
    };


    class CSawbladeEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { SAWBLADE_EVENT_START = MESH_EVENT_END, SAWBLADE_EVENT_STOP };

        CSawbladeEntity();
        virtual ~CSawbladeEntity();

        virtual void Update(float dt);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual bool HasCollision(CEntity& e);
        virtual void SetMeshFilename(const std::string& strName);
        virtual void SetRotationSpeedZ(float rot);

        void ScrollType(bool bUp);
    protected:
        void SetupSounds() override;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_currentType;

            if (version > 0)
                ar & m_initialRotationSpeedZ;
        }

    private:

        SAWBLADE_TYPE m_currentType;

        Vec3 m_prevPosition;

        float m_initialRotationSpeedZ;
    };

} // env



#endif // SIMPLESAWBLADE_H 