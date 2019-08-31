#include <Engine/Core/ParticleAffector.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Entity/ParticleEmitterEntity.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    void CParticleSunlightAffector::AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt)
    {
    }

    //-----------------------------------------------------------------------------------
    void CParticleTextureVelocityRotator::AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt)
    {
        Vec3 derivedVelocity(particle.m_velocity);
        if (!pEmitter->GetGlobalSpaceParticles())
            derivedVelocity = Vec3(Vec4(derivedVelocity, 0.0f) * pEmitter->GetNode()->GetDerivedRST());

        Vec2 vXAxis(1.0f, 0.0f);
        Vec2 vVelo(glm::normalize(derivedVelocity));
    }

    //-----------------------------------------------------------------------------------
    void CParticleCharacterTrace::AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt)
    {
    }

    //-----------------------------------------------------------------------------------
    void CParticleTextureRandomRotator::AffectParticle(CParticleCPU& particle, CParticleEmitterEntity* pEmitter, float dt)
    {
    }
} // env