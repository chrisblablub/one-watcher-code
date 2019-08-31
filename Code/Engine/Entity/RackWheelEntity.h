/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef RACK_WHEEL_ENTITY_H
#define RACK_WHEEL_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{
    class CRackWheelEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CRackWheelEntity();
        virtual ~CRackWheelEntity();

        virtual void Update(float dt);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void SetMeshFilename(const std::string& strName);
    protected:
        void SetupSounds() override;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
            {
                ar & m_pMeshInner;
                ar & m_pMeshFan;
                ar & m_pMeshShield;
            }
        }

        void InitMeshes();

        CMeshEntity* m_pMeshInner;
        CMeshEntity* m_pMeshFan;
        CMeshEntity* m_pMeshShield;
    };
} // env
#endif // RACK_WHEEL_ENTITY_H 