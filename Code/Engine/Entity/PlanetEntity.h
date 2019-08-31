/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PLANET_ENTITY_H
#define PLANET_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Scene/Sprite.h>
#include <Graphic/RenderObjects/SpriteRenderObject.h>

namespace env 
{
    class CPlanetEntity : public CMeshEntity
    {
    public: 
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CPlanetEntity();
        virtual ~CPlanetEntity();


        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);
        virtual void SetMeshFilename(const std::string& strName);
    
        void SetEnergyColor(const Vec3& color) { m_energyColor = color; }
        const Vec3& GetEnergyColor() const { return m_energyColor; }

        void SetHalo(bool bHalo);
        bool GetHalo() const { return m_bHalo; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if(version > 0)
                ar & m_energyColor;

            if (version > 1)
                ar & m_bHalo;
        }

    protected:

        Vec3 m_energyColor;

        bool m_bHalo;

        CSprite m_sprite;
        CSceneNode m_spriteNode;

        SpriteRenderObject m_spriteRenderObject;

        PlanetRenderObject m_planetRenderObject;
    };	


    class CUniversePlanetEntity : public CMeshEntity
    {
    public: 
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CUniversePlanetEntity();
        virtual ~CUniversePlanetEntity();

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

        //VECTOR3 m_energyColor;
    };	

    
} // env

#endif // PLANET_ENTITY_H 