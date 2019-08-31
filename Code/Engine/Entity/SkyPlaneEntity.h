/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SKY_PLANE_ENTITY_H
#define SKY_PLANE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{
    class CSkyPlaneEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CSkyPlaneEntity();
        virtual ~CSkyPlaneEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);


        void SetColor(const Vec3& clr) { m_color = clr; }
        const Vec3& GetColor() const { return m_color; }

        void SetAlpha(float a) { m_alpha = a; }
        float GetAlpha() const { return m_alpha; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
                ar & m_alpha;
        }

    protected:
        Vec3 m_color;
        float m_alpha;

        SkyPlaneRenderObject m_skyPlaneRenderObject;
    };


} // env

#endif // SKY_PLANE_ENTITY_H 