/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef ENVIRONMENT_MAP_ENTITY_H
#define ENVIRONMENT_MAP_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Graphic/RenderObjects/SkyBoxRenderObject.h>

namespace env
{
    class CEnvMapEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CEnvMapEntity();
        virtual ~CEnvMapEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);

        void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }
        bool GetEnabled() const { return m_bEnabled; }

        void SetEnvironmentMap(const std::string& strEnvMap);
        CTextureManaged* GetEnvironmentMap() const { return m_pTexture; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);

            if (version > 0)
                ar & m_strEnvMap;
        }

    protected:
        CTextureManaged* m_pTexture;

        std::string m_strEnvMap;

        bool m_bEnabled;

        EnvMapRenderObject m_envMapRenderObject;

    };

} // env

#endif // ENVIRONMENT_MAP_ENTITY_H
