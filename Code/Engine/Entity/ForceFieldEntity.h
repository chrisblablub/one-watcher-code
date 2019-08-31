/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FORCE_FIELD_ENTITY_H
#define FORCE_FIELD_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Graphic/RenderObjects/ForceFieldRenderObject.h>

namespace env
{
    class CForceFieldEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CForceFieldEntity();

        virtual ~CForceFieldEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnEntityCloned(CEntity& entity);

        void SetAcceleration(float strength) { m_acceleration = strength; }
        float GetAcceleration() const { return m_acceleration; }

        void SetTargetVelocity(float targetVelo) { m_targetVelocity = targetVelo; }
        float GetTargetVelocity() const { return m_targetVelocity; }

        void SetColor(const Vec3& clr) { m_color = clr; }
        const Vec3& GetColor() const { return m_color; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_acceleration;

            if (version > 0)
                ar & m_targetVelocity;

            if (version > 1)
                ar & m_color;
        }

    protected:
        float m_acceleration;
        float m_targetVelocity;

        Vec3 m_color;

        ForceFieldRenderObject m_forceFieldRenderObject;
    };


} // env

#endif // FORCE_FIELD_ENTITY_H 