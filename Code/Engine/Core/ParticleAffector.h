/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PARTICLE_AFFECTOR_H
#define PARTICLE_AFFECTOR_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/Particle.h>

namespace env
{
    class CParticleEmitterEntity;

    class IParticleAffector
    {
    public:
        virtual ~IParticleAffector() {}
        virtual void AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt) = 0;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
        }
    };

    class CParticleSunlightAffector : public IParticleAffector
    {
    public:
        virtual ~CParticleSunlightAffector() {}
        virtual void AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< IParticleAffector >(*this);
        }
    };

    class CParticleTextureVelocityRotator : public IParticleAffector
    {
    public:
        virtual ~CParticleTextureVelocityRotator() {}
        virtual void AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< IParticleAffector >(*this);
        }
    };

    class CParticleCharacterTrace : public IParticleAffector
    {
    public:
        virtual ~CParticleCharacterTrace() {}
        virtual void AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< IParticleAffector >(*this);
        }
    };

    class CParticleTextureRandomRotator : public IParticleAffector
    {
    public:
        virtual ~CParticleTextureRandomRotator() {}
        virtual void AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< IParticleAffector >(*this);
        }
    };

} // env
#endif // PARTICLE_AFFECTOR_H