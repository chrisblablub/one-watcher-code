/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PLATFORM_ENTITY_H
#define PLATFORM_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/ExplosionEntity.h>

namespace env
{
    enum PLATFORM_TYPE
    {
        PLATFORM_TYPE_RACKWHEEL_TOP,
        PLATFORM_TYPE_RACKWHEEL_BOTTOM,
        PLATFORM_TYPE_RACKWHEEL_TOPBOTTOM,
        PLATFORM_TYPE_GATE,
        PLATFORM_TYPE_SMALLRACKWHEEL_BOTTOM,
        PLATFORM_TYPE_SMALLRACKWHEEL_TOP,
    };

    class CPlatformBaseEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CPlatformBaseEntity();
        virtual ~CPlatformBaseEntity();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void SetMeshFilename(const std::string& strName);
        virtual void OnPlayerOnPlatform() {}

        void SetWidth(float width) { m_halfSize.x = width; }
        void SetHeight(float height) { m_halfSize.y = height; }

        void SetSize(float width, float height);
        const Vec2& GetSize() const { return m_halfSize; }

        virtual void SetEnabled() { m_bDisabled = false; }
        virtual void SetDisabled() { m_bDisabled = true; }
        bool GetDisabled() const { return m_bDisabled; }


    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
            ar & m_halfSize;

            if (version > 0)
                ar & m_pRackWheel;
        }

        void InitPlatform(const Vec2& halfSize, const Vec3& rackWheelScale, const Vec3& rackWheelPos);

    protected:

        Vec2 m_halfSize;
        Vec2 m_prevPosition;

        CMeshEntity* m_pRackWheel;
        void SetupSounds() override;

        bool m_bDisabled;
    };



    class CPlatformEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CPlatformEntity();
        virtual ~CPlatformEntity();

        virtual void Update(float dt);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);

        virtual void OnPlayerOnPlatform() {}

        void SetWidth(float width) { m_halfSize.x = width; }
        void SetHeight(float height) { m_halfSize.y = height; }

        void SetSize(float width, float height);
        const Vec2& GetSize() const { return m_halfSize; }


        void SetType(PLATFORM_TYPE type);
        PLATFORM_TYPE GetType() const { return m_type; }

        virtual void SetEnabled() { m_bDisabled = false; }
        virtual void SetDisabled() { m_bDisabled = true; }
        bool GetDisabled() const { return m_bDisabled; }

    protected:
        virtual void OnCreatePlatform();

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
                ar & m_halfSize;

            if (version > 1)
            {
                ar & m_type;
            }

            if (version > 2)
            {
                ar & m_attachedEntities;
            }
        }

        Vec2 m_halfSize;
        Vec2 m_prevPosition;

        PLATFORM_TYPE m_type;

        std::vector< CEntity* > m_attachedEntities;

        bool m_bDisabled;
    };

    class CPlatformDrone : public CPlatformEntity
    {
    public:
        static const float BLINK_TIME;

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CPlatformDrone();
        virtual ~CPlatformDrone();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual bool HasCollision(CEntity& e);

        virtual void OnPlayerOnPlatform();

        virtual void SetEnabled();
        virtual void SetDisabled();

        void SetDelay(float delay) { m_delay = delay; }
        float GetDelay() const { return m_delay; }

    protected:
        virtual void OnCreatePlatform();
        void SetupSounds() override;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CPlatformEntity >(*this);
            ar & m_pExplosion;

            if (version > 0)
                ar & m_pFan;

            if (version > 1)
                ar & m_delay;
        }

        CExplosionEntity* m_pExplosion;
        CMeshEntity* m_pFan;

        float m_currentSpawnTime;
        float m_currentBlinkTime;
        float m_accumExplosionTime;
        bool m_bBlinkAppear;
        bool m_bExploding;

        float m_delay;
        float m_currentDelay;

        CSprite m_sprite;
        CSceneNode m_spriteNode;


    };


    /*class CPlatformSawbladeTopEntity : public CPlatformEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CPlatformSawbladeTopEntity();

        virtual void InitializeAtCreation();

    };*/

} // env

#endif // PLATFORM_ENTITY_H