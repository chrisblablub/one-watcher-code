/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SKY_BOX_ENTITY_H
#define SKY_BOX_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Graphic/RenderObjects/SkyBoxRenderObject.h>

namespace env
{
    class CSkyBoxEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CSkyBoxEntity();
        virtual ~CSkyBoxEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);
        virtual bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode) { return false; }

        CTextureManaged* GetSkyBoxTexture() const { return m_pTexture; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
        }

    protected:
        CTextureManaged* m_pTexture;

        SkyBoxRenderObject m_skyBoxRenderObject;

    };



} // env

#endif // SKY_BOX_ENTITY_H 