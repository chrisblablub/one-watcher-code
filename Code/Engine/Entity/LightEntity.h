/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LIGHTENTITY_H
#define LIGHTENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Scene/Sprite.h>

namespace env
{
    class CLight;

    class CLightEntity : public CMeshEntity
    {
    public: 
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS 
        { 
            LIGHT_EVENT_ON = MESH_EVENT_END, 
            LIGHT_EVENT_OFF, 
            LIGHT_EVENT_FOLLOWTARGET, 
            LIGHT_EVENT_FOLLOWTARGETWHILEINSPOT,
            LIGHT_EVENT_END
        };

        CLightEntity();
        virtual ~CLightEntity();

        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual bool HasCollision(CEntity& e);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);

        void SetTarget(CEntity* pTarget) { m_pTargetEntity = pTarget; m_bFollowTarget = true; }
        CEntity* GetTarget() { return m_pTargetEntity; }

        CLight* GetLight();

        void SetMaxRotationSpeed(float rotSpeed) { m_maxRotationSpeed = rotSpeed; }
        float GetMaxRotationSpeed() { return m_maxRotationSpeed; }

        void SetFlicker(bool bFlicker) { m_bFlicker = bFlicker; }
        bool GetFlicker() const { return m_bFlicker; }

        void SetSprite(bool bSet) { m_bSprite = bSet; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_pLight;
            ar & m_pTargetEntity;
            ar & m_sprite;
            ar & m_maxRotationSpeed;

            if(version > 3)
                ar & m_bFlicker;

            if(version > 4)
                ar & m_bSprite;

            if(version > 5)
            {
                ar & m_pTargetEntity;
                ar & m_bFollowTarget;
                ar & m_bFollowPlayer;
            }
        }

    protected:
        CEntity* m_pTargetEntity;

        CLight* m_pLight;

        
        bool m_bFollowTarget;
        bool m_bFollowPlayer;
        bool m_bFollowPlayerWhileInSpot;
        bool m_bFlicker;
        bool m_bFlickering;
        bool m_bSprite;

        float m_maxRotationSpeed;
        float m_curTime;
        float m_targetTime;

        CSprite m_sprite;
    };





    class CSimpleLightEntity : public CSpriteEntity
    {
    public: 
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CSimpleLightEntity();
        virtual ~CSimpleLightEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);

        CLight* GetLight() const { return m_pLight; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_pLight;
        }

    private:
        CLight* m_pLight;
    };

} /* End namespace */

#endif /* LIGHTENTITY_H */