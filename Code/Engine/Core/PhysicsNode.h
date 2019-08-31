/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PHYSICS_NODE_H
#define PHYSICS_NODE_H

#include <Engine/EngineCommon.h>

namespace env
{
    class CPhysicsNode2D
    {
    public:
        CPhysicsNode2D()
            : m_vVelocity(0.0f, 0.0f),
            m_vMaxVelocity(900.0f, 900.0f),
            m_vForce(0.0f, 0.0f),
            m_forceTime(0.0f),
            m_vGravity(0.0f, 0.0f),
            m_bGravity(false)
        {}

        void Update(float dt)
        {

            if (m_forceTime <= 0.0f)
            {
                m_vForce = Vec2(0.0f, 0.0f);
            }
            else
                m_forceTime -= dt;

            //VECTOR2 accumAccel = 0.5f * (m_vGravity + m_vForce)* dt * dt;
            Vec2 accumAccel = (m_vGravity + m_vForce)* dt;
            if ((abs(m_vVelocity.x) <= m_vMaxVelocity.x) || (m_vVelocity.x > 0.0f && accumAccel.x < 0.0f) || (m_vVelocity.x < 0.0f && accumAccel.x > 0.0f))
                m_vVelocity.x += accumAccel.x;

            if ((abs(m_vVelocity.y) <= m_vMaxVelocity.y) || (m_vVelocity.y > 0.0f && accumAccel.y < 0.0f) || (m_vVelocity.y < 0.0f && accumAccel.y > 0.0f))
                m_vVelocity.y += accumAccel.y;
        }

        void Reset()
        {
            m_vForce = Vec2(0.0f, 0.0f);
            m_vVelocity = Vec2(0.0f, 0.0f);
            m_vGravity = Vec2(0.0f, 0.0f);

            m_forceTime = 0.0f;
        }


        void ApplyImpulse(const Vec2& force, float time)
        {
            m_forceTime = time;
            m_vForce = force;
        }

        float GetVelocityScalar() { return glm::length(m_vVelocity); }

        float GetImpulseTime() const { return m_forceTime; }

        void SetVelocity(const Vec2& velo) { m_vVelocity = velo; }
        const Vec2& GetVelocity() const { return m_vVelocity; }

        void SetGravity(bool bSet, const Vec2& gravity) { m_bGravity = bSet; m_vGravity = gravity; }
        const Vec2& GetGravity() const { return m_vGravity; }

    private:
        Vec2 m_vVelocity;

        Vec2 m_vMaxVelocity;

        Vec2 m_vForce;
        float m_forceTime;

        Vec2 m_vGravity;
        bool m_bGravity;


    };
} // env

#endif // PHYSICS_NODE_H