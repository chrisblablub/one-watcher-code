/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENERGY_FIELD_ENTITY_H
#define ENERGY_FIELD_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Graphic/RenderObjects/EnergyFieldRenderObject.h>

namespace env
{
    class CEnergyFieldEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CEnergyFieldEntity();
        virtual ~CEnergyFieldEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);
        virtual void SetMeshFilename(const std::string& strName);

        //void SetEnergyColor(const VECTOR3& color) { m_energyColor = color; }
        //const VECTOR3& GetEnergyColor() const { return m_energyColor; }
    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            //if(version > 0)
            //	ar & m_energyColor;
        }

    protected:

        Vec3 m_energyColor;

        EnergyFieldRenderObject m_energyFieldRenderObject;
    };


} // env

#endif // ENERGY_FIELD_ENTITY_H 