/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Core/Particle.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/BoundingBoxDataCache.h>
#include <Common/Core/BoundingVolume.h>

namespace env
{
    class CParticleSystemEntity;
    //class CBoundingBoxDataCache;

    enum PARTICLE_ORIENTATION
    {
        PARTICLE_ORIENTATION_TO_VELOCITY,
        PARTICLE_ORIENTATION_RANDOM
    };

    enum PARTICLE_FACING
    {
        PARTICLE_FACING_EMITTER,
        PARTICLE_FACING_CAMERA,
        PARTICLE_FACING_HORIZONTAL,
        PARTICLE_FACING_SCREEN
    };

    class CParticleEmitterEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CParticleEmitterEntity();

        virtual ~CParticleEmitterEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);

        void Unload();
        void Reset();
        void EmitParticles();

        virtual void LoadFromTemplate(CScript& script);
        virtual void SaveAsTemplate(CScript& script);


        virtual void Update(float dt);

        void SetTime(float time) { m_time = time; }

        void ComputeBoundingBox(Vec3& vMin, Vec3& vMax);

        void SetColor(const Vec4& vColor) { m_vColorBegin = m_vColorEnd = vColor; }
        void SetColorBegin(const Vec4& vColorStart) { m_vColorBegin = vColorStart; }
        const Vec4& GetColorBegin() { return m_vColorBegin; }

        void SetColorEnd(const Vec4& vColorEnd) { m_vColorEnd = vColorEnd; }
        const Vec4& GetColorEnd() { return m_vColorEnd; }

        void SetAcceleration(float vAcceleration) { m_accelerationMin = m_accelerationMax = vAcceleration; }
        void SetAccelerationMin(float vAccelerationMin) { m_accelerationMin = vAccelerationMin; }
        float GetAccelerationMin() { return m_accelerationMin; }

        void SetAccelerationMax(float vAccelerationMax) { m_accelerationMax = vAccelerationMax; }
        float GetAccelerationMax() { return m_accelerationMax; }

        void SetVelocity(float speed) { m_velocityBegin = m_velocityEnd = speed; }
        void SetVelocityBegin(float min) { m_velocityBegin = min; }
        float GetVelocityBegin() { return m_velocityBegin; }

        void SetVelocityEnd(float max) { m_velocityEnd = max; }
        float GetVelocityEnd() { return m_velocityEnd; }

        void SetVelocityExp(float exp) { m_velocityExp = exp; }
        float GetVelocityExp() const { return m_velocityExp; }

        void SetVelocityRange(float range) { m_velocityRange = range; }
        float GetVelocityRange() const { return m_velocityRange; }


        void SetLifeTime(float lifeTime) { m_lifetimeMin = m_lifetimeMax = lifeTime; }
        void SetLifeTimeMin(float min) { m_lifetimeMin = min; }
        float GetLifeTimeMin() { return m_lifetimeMin; }

        void SetLifeTimeMax(float max) { m_lifetimeMax = max; }
        float GetLifeTimeMax() { return m_lifetimeMax; }

        void SetVelocityAngle(float rad) { m_velocityAngle = rad; }
        float GetVelocityAngle() { return m_velocityAngle; }

        void SetEmitRate(float rate) { m_emitRate = rate; }
        float GetEmitRate() { return m_emitRate; }

        void SetEmitDuration(float duration) { m_emitDuration = duration; m_currentEmitDuration = duration; }
        float GetEmitDuration() { return m_emitDuration; }

        void SetParticleSystem(CParticleSystemEntity* pSys);
        CParticleSystemEntity* GetParticleSystem();

        unsigned int GetNumParticlesAlive() { return m_numParticlesAlive; }

        void SetMaterialSequence(unsigned int seq) { m_sequence = seq; }
        unsigned int GetMaterialSequence() const { return m_sequence; }

        void SetParticleSize(float size) { m_particleSizeBegin = m_particleSizeEnd = m_particleSize = size; }
        float GetParticleSize() { return m_particleSize; }

        void SetParticleSizeBegin(float size) { m_particleSizeBegin = size; }
        float GetParticleSizeBegin() { return m_particleSizeBegin; }

        void SetParticleSizeRange(float size) { m_particleSizeRange = size; }
        float GetParticleSizeRange() { return m_particleSizeRange; }

        void SetParticleSizeEnd(float size) { m_particleSizeEnd = size; }
        float GetParticleSizeEnd() { return m_particleSizeEnd; }

        void SetNumParticles(unsigned int numParticles);
        unsigned int GetNumParticles() { return m_numParticles; }

        void SetFadeInTime(float time) { m_fadeInTime = time; }
        float GetFadeInTime() { return m_fadeInTime; }

        void SetFadeOutTime(float time) { m_fadeOutTime = time; }
        float GetFadeOutTime() { return m_fadeOutTime; }

        void SetGlobalSpaceParticles(bool bSet) { m_bGlobalSpaceParticles = bSet; }
        bool GetGlobalSpaceParticles() const { return m_bGlobalSpaceParticles; }

        void SetEmitNewParticles(bool bSet) { m_bEmitNewParticles = bSet; }
        void SetPauseParticles(bool bSet) { m_bPauseParticles = bSet; }

        void SetClipping(bool bClip) { m_bClipping = bClip; }
        bool GetClipping() const { return m_bClipping; }

        std::vector< CParticleCPU* >& GetParticles();

        const CBoundingVolume& GetBoundingBox() { return m_boundingBox; }

        void SetWarmUpTime(float time) { m_warmUpTime = time; }
        float GetWarmUpTime() const { return m_warmUpTime; }

        void SetBoundingBoxMin(const Vec3& size);
        Vec3 GetBoundingBoxMin() const { return m_vBBoxMin; }

        void SetBoundingBoxMax(const Vec3& size);
        Vec3 GetBoundingBoxMax() const { return m_vBBoxMax; }

        void SetDynamicBoundingBox(bool bSet);
        bool GetDynamicBoundingBox() const { return m_bDynamicBBox; }

        void SetVelocityFromCenter(bool bSet) { m_bVelocityFromCenter = bSet; }
        bool GetVelocityFromCenter() const { return m_bVelocityFromCenter; }

        void SetEmitDelay(float delay) { m_emitDelay = delay; }
        float GetEmitDelay() const { return m_emitDelay; }

        void SetBlendMode(float blendMode) { m_blendMode = blendMode; }
        float GetBlendMode() const { return m_blendMode; }

        void SetOrientation(PARTICLE_ORIENTATION orient) { m_orientation = orient; }
        PARTICLE_ORIENTATION GetOrientation() const { return m_orientation; }

        void SetFacing(PARTICLE_FACING facing) { m_facing = facing; }
        PARTICLE_FACING GetFacing() const { return m_facing; }

        void SetOrientationSpeedMin(float orientSpeed) { m_orientationSpeedMin = orientSpeed; }
        float GetOrientationSpeedMin() const { return m_orientationSpeedMin; }

        void SetOrientationSpeedMax(float orientSpeed) { m_orientationSpeedMax = orientSpeed; }
        float GetOrientationSpeedMax() const { return m_orientationSpeedMax; }

        void SetDistortionMin(float distortion) { m_distortionMin = distortion; }
        float GetDistortionMin() const { return m_distortionMin; }

        void SetDistortionMax(float distortion) { m_distortionMax = distortion; }
        float GetDistortionMax() const { return m_distortionMax; }

        void SetEmitAllParticlesOnce(bool bOnce) { m_bEmitAllParticlesOnce = bOnce; }
        bool GetEmitAllParticlesOnce() const { return m_bEmitAllParticlesOnce; }

        void SetStartDisabled(bool bOnce) { m_bStartDisabled = bOnce; }
        bool GetStartDisabled() const { return m_bStartDisabled; }

    protected:
        virtual void InitParticle(CParticleCPU& particle);
        virtual void InitParticlePosition(CParticleCPU& particle);
        virtual void InitParticleVelocityAcceleration(CParticleCPU& particle);

    protected:
        std::vector< CParticleCPU* > m_particles;
        unsigned int m_numParticlesAlive;
        unsigned int m_numParticles;

        float m_particleSize;
        float m_particleSizeBegin;
        float m_particleSizeEnd;
        float m_particleSizeRange;

        float m_blendMode;

        float m_fadeInTime;
        float m_fadeOutTime;

        CParticleSystemEntity* m_pParticleSystem;

        float m_currentEmitDuration;
        float m_currentEmitTime;
        float m_time;
        float m_particlesToEmit;

        Vec4 m_vColorBegin;
        Vec4 m_vColorEnd;

        Vec3 m_prevPosition;

        bool m_bEmitNewParticles;
        bool m_bPauseParticles;
        bool m_bGlobalSpaceParticles;
        bool m_bClipping;
        bool m_bVelocityFromCenter;
        bool m_bEmitAllParticlesOnce;
        bool m_bStartDisabled;

        PARTICLE_ORIENTATION m_orientation;
        PARTICLE_FACING m_facing;

        float m_accelerationMin;
        float m_accelerationMax;
        float m_velocityBegin;
        float m_velocityEnd;
        float m_velocityExp;
        float m_velocityRange;
        float m_orientationSpeedMin;
        float m_orientationSpeedMax;
        float m_distortionMin;
        float m_distortionMax;

        float m_velocityAngle;
        float m_lifetimeMin;
        float m_lifetimeMax;
        float m_emitRate;
        float m_emitDuration;
        float m_emitDelay;
        float m_warmUpTime;

        unsigned int m_sequence;
        unsigned int m_numParticlesEmitted;

        bool m_bDynamicBBox;
        Vec3 m_vBBoxMin;
        Vec3 m_vBBoxMax;

        //CBoundingBoxDataCache* m_pEmitterBBox;
        CBoundingVolume m_boundingBox;
        CBoundingBoxDataCache m_boundingBoxDataCache;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & std::string();
            ar & std::string();
            ar & std::string();
            ar & m_vColorBegin;
            ar & m_vColorEnd;
            ar & Vec3(0.0f, 0.0f, 0.0f);
            ar & m_velocityBegin;
            ar & m_velocityEnd;
            ar & m_velocityAngle;
            ar & m_lifetimeMin;
            ar & m_lifetimeMax;
            ar & m_emitRate;
            ar & m_emitDuration;
            ar & m_numParticles;
            ar & m_particleSizeBegin;
            ar & m_particleSizeEnd;
            ar & m_fadeInTime;
            ar & m_fadeOutTime;

            if (version > 0)
            {
                ar & m_warmUpTime;
                ar & m_bDynamicBBox;
                ar & m_vBBoxMin;
                ar & m_vBBoxMax;
            }

            if (version > 1)
            {
                ar & m_bGlobalSpaceParticles;
            }

            if (version > 2)
            {
                ar & m_emitDelay;
                ar & m_sequence;
                ar & m_bVelocityFromCenter;
            }

            if (version > 3)
                ar & m_particleSizeRange;

            if (version > 4)
                ar & m_blendMode;

            if (version > 5)
            {
                ar & m_velocityExp;
                ar & m_velocityRange;
            }

            if (version > 6)
            {
                ar & m_orientation;
                ar & m_orientationSpeedMin;
            }

            if (version > 7)
            {
                ar & m_orientationSpeedMax;
                ar & m_facing;
            }

            if (version > 8)
            {
                ar & m_distortionMin;
                ar & m_distortionMax;
            }

            if (version > 9)
                ar & m_bEmitAllParticlesOnce;

            if (version > 10)
                ar & m_bStartDisabled;
        }
    };

    class CPlanarPointParticleEmitterEntity : public CParticleEmitterEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CPlanarPointParticleEmitterEntity();

    private:
        virtual void InitParticleVelocityAcceleration(CParticleCPU& particle);
        virtual void InitParticlePosition(CParticleCPU& particle);

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CParticleEmitterEntity >(*this);
        }
    };

    class CSphereParticleEmitterEntity : public CParticleEmitterEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CSphereParticleEmitterEntity();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);

        virtual void LoadFromTemplate(CScript& script);
        virtual void SaveAsTemplate(CScript& script);

    protected:
        virtual void InitParticlePosition(CParticleCPU& particle);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CParticleEmitterEntity >(*this);
            ar & m_radius;
        }

    protected:
        float m_radius;

        CBoundingBoxDataCache m_emitterBoxDataCache;
    };

    class CBoxParticleEmitterEntity : public CParticleEmitterEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CBoxParticleEmitterEntity();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void LoadFromTemplate(CScript& script);
        virtual void SaveAsTemplate(CScript& script);

        void SetEmitterBoxMin(const Vec3& emitterBoxMin) { m_emitterBoxMin = emitterBoxMin; }
        void SetEmitterBoxMax(const Vec3& emitterBoxMax) { m_emitterBoxMax = emitterBoxMax; }

        const Vec3& GetEmitterBoxMin() const { return m_emitterBoxMin; }
        const Vec3& GetEmitterBoxMax() const { return m_emitterBoxMax; }

        void SetAlignScreenX(bool bAlign) { m_bAlignScreenX = bAlign; }
        void SetAlignScreenY(bool bAlign) { m_bAlignScreenY = bAlign; }

        bool GetAlignScreenX() const { return m_bAlignScreenX; }
        bool GetAlignScreenY() const { return m_bAlignScreenY; }


    protected:
        virtual void InitParticlePosition(CParticleCPU& particle);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CParticleEmitterEntity >(*this);
            ar & m_emitterBoxMin;
            ar & m_emitterBoxMax;

            if (version > 0)
            {
                ar & m_bAlignScreenX;
                ar & m_bAlignScreenY;
            }
        }

    protected:
        Vec3 m_emitterBoxMin;
        Vec3 m_emitterBoxMax;

        bool m_bAlignScreenX;
        bool m_bAlignScreenY;

        CBoundingBoxDataCache m_emitterBoxDataCache;
    };

} // env
#endif // PARTICLE_EMITTER_H