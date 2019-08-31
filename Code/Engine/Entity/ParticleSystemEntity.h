/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PARTICLE_SYSTEM_ENTITY_H
#define PARTICLE_SYSTEM_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/ParticleEmitterEntity.h>
#include <Engine/Core/ParticleAffector.h>
//#include <Graphic/RenderSystemConfig.h>
#include <Graphic/Core/Material.h>
#include <Graphic/RenderObjects/ParticleSystemRenderObject.h>
#include <Common/Core/Observer.h>


namespace env
{
    struct CParticleCPU;
    class IParticleAffector;
    class CParticleEmitterEntity;
    class CParticleSystemController;

    enum PARTICLESYSTEM_ALIGNMENT
    {
        PARTICLESYSTEM_ALIGNMENT_NONE = 0,
        PARTICLESYSTEM_ALIGNMENT_LEFT,
        PARTICLESYSTEM_ALIGNMENT_TOP,
        PARTICLESYSTEM_ALIGNMENT_RIGHT,
        PARTICLESYSTEM_ALIGNMENT_BOTTOM,
        PARTICLESYSTEM_ALIGNMENT_CENTER
    };

    class CParticleSystemEntity : public CSpriteEntity, public CObserver
    {
    public:
        typedef std::map< std::string, CParticleEmitterEntity* > mapEmitters;
        typedef std::map< std::string, IParticleAffector* > mapAffectors;

        enum TRIGGERABLE_EVENTS 
        { 
            PARTICLESYSTEM_EVENT_ENABLE = SPRITE_EVENT_END, 
            PARTICLESYSTEM_EVENT_DISABLE, 
            PARTICLESYSTEM_EVENT_STARTEMIT, 
            PARTICLESYSTEM_EVENT_STOPEMIT, 
            PARTICLESYSTEM_EVENT_END 
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CParticleSystemEntity();

        virtual ~CParticleSystemEntity();
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void Update(float dt);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void SetPickable(bool bPickable);

        virtual void OnResetDevice();
        virtual void OnLostDevice();

        virtual void OnEntityCloned(CEntity& entity);

        virtual void SetRenderEditor(bool bEditor);
        virtual void SetRenderGame(bool bGame);
        virtual void SetRender(bool bRender);

        const std::string& GetParticleSystemTemplate();

        void SetAlignment(PARTICLESYSTEM_ALIGNMENT alignment);
        PARTICLESYSTEM_ALIGNMENT GetAlignment();

        void SetParticleRotationAngle(float angle);
        float GetParticleRotationAngle();


        bool LoadFromTemplate(const std::string& strType);
        void SaveAsTemplate(const std::string& strName);

        void EmitAllParticlesOnce();


        const std::string& GetParticleSystemType();
        const std::vector< CParticleCPU* >& GetParticles();
        CVertexBuffer* GetParticleVB();

        CManagedMaterial* GetMaterialPtr() { return m_pMaterial; }

        CParticleEmitterEntity* AddEmitter(const std::string& strEmitter);
        void RemoveEmitter(const std::string& strName);
        CParticleEmitterEntity* GetEmitter(const std::string& str);

        IParticleAffector* AddAffector(const std::string& strName);
        void RemoveAffector(const std::string& strName);

        mapEmitters& GetEmitters() { return m_emitters; }
        const mapAffectors& GetAffectors() { return m_affectors; }


        //! Sets the emittance of new particles
        void SetEmitNewParticles(bool bSet);

        //! Pauses the simulation of particles
        void SetPauseParticles(bool bSet);

        void SetStartDisabled(bool bSet);

        //! Sets rendering of particles
        void SetRenderParticles(bool bEnable) { m_bRenderParticles = bEnable; }

        void Reset();

        int GetNumTotalParticlesAlive() const { return m_numTotalParticlesAlive; }

        void SetParticleMaterialFilename(const std::string& strMaterial);
        std::string GetParticleMaterialFilename() { return m_strMaterial; }


        void SetRenderer(const std::string& strRenderer) { m_strRenderer = strRenderer; }
        std::string& GetRenderer() { return m_strRenderer; }

        inline void SetLoadFromTemplate(bool bLoad) { m_bLoadFromTemplate = bLoad; }
        inline bool GetLoadFromTemplate() const { return m_bLoadFromTemplate; }

    private:
        void Unload();

        bool Initialize(CRenderDevice* pDevice);

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_strParticleSystemTemplate;
            ar & m_alignment;
            ar & m_strRenderer;
            ar & m_strMaterial;
            ar & m_emitters;
            ar & m_affectors;

            if (version > 1)
                ar & m_bLoadFromTemplate;
        }

        template< class Particle >
        void SetupParticlesColor(Particle* pParticle,
                                 CParticleCPU* pCurParticle,
                                 const Vec4& particlePos,
                                 const Vec4& texCoords,
                                 const Vec3 particleVertices[4],
                                 const Matrix4x4& mtxInvView,
                                 const PARTICLE_FACING particleEmitterFacing);

        void SetupParticleDistort(CParticleDistortGPU* pParticle,
                                  CParticleCPU* pCurParticle);

    protected:
        CRenderDevice* m_pDevice;
        CVertexBuffer* m_pParticleVB;

        std::string m_strParticleSystemTemplate;

        bool m_bLoadFromTemplate;

        float m_currentAngle;
        float m_time;

        bool m_bEmitNewParticles;
        bool m_bRenderParticles;

        

        int m_numTotalParticles;
        int m_numTotalParticlesAlive;

        PARTICLESYSTEM_ALIGNMENT m_alignment;

        std::string m_strRenderer;
        std::string m_strMaterial;

        mapEmitters m_emitters;
        mapAffectors m_affectors;

        CManagedMaterial* m_pMaterial;

        ParticleSystemRenderObject m_renderObject;
    };

    

} // env 
#endif // PARTICLE_SYSTEM_H