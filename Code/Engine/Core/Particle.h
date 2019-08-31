/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <Graphic/Core/VertexDeclarations.h>

namespace env
{
    struct CParticleCPU : CParticleColorGPU
    {
        CParticleCPU()
            : CParticleColorGPU()
            , m_initialAcceleration(0.0f, 0.0f, 0.0f)
            , m_velocity(0.0f, 0.0f, 0.0f)
            , m_spawnTime(0.0f)
            , m_lifeTime(0.0f)
            , m_curFrame(0)
            , m_startColor(1.0f, 1.0f, 1.0f, 1.0f)
            , m_endColor(1.0f, 1.0f, 1.0f, 1.0f)
            , m_initialPosition(0.0f, 0.0f, 0.0f, 1.0f)
            , m_startSize(1.0f)
            , m_size(1.0f)
            , m_endSize(1.0f)
            , m_angle(0.0f)
            , m_orientSpeed(0.0f)
            , m_sequence(0)
            , m_distortion(0.0f) {}

        Vec4 m_startColor;
        Vec4 m_endColor;
        Vec3 m_initialAcceleration;
        Vec4 m_initialPosition;
        Vec3 m_velocity;
        Vec3 m_velocityBegin;
        Vec3 m_velocityEnd;

        float m_spawnTime;
        float m_lifeTime;
        float m_startSize;
        float m_endSize;
        float m_size;
        float m_angle;
        float m_orientSpeed;
        float m_distortion;

        int m_curFrame;
        int m_sequence;
    };

} // env
#endif // PARTICLE_H