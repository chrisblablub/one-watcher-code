/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PARTICLE_SYSTEM_MGR
#define PARTICLE_SYSTEM_MGR


#include <Engine/EngineCommon.h>
#include <Engine/Core/ParticleAffector.h>
#include <Graphic/Renderer/ParticleSystemRenderer.h>


namespace env
{
    class CParticleSystemMgr
    {
    public:

        typedef std::map< std::string, CParticleSystemRenderer* > mapRenderers;
        typedef std::map< std::string, IFactory< IParticleAffector >* > mapAffectorFactory;
        typedef std::map< std::string, CScript* > mapParticleSystemScripts;

        enum PARTICLESYSTEM_EVENTS { PARTICLESYSTEM_RELOADED = 0 };

        CParticleSystemMgr();
        CParticleSystemMgr(const CParticleSystemMgr&) = delete;
        CParticleSystemMgr& operator=(const CParticleSystemMgr&) = delete;
        ~CParticleSystemMgr();

        void Reset();

        void Initialize();
        CScript* GetParticleSystemTemplate(const std::string& strType);
        CScript* CreateParticleSystemTemplate(const std::string& strName);

        void SaveTemplates();

        void GetParticleSystemTemplates(std::set< std::string >& particleSystems);
        void GetParticleSystemAffectors(std::set< std::string >& affectors);

        IParticleAffector* CreateParticleAffector(const std::string& strAffectorType);

        template< class T > void RegisterAffector(const std::string& strAffectorName)
        {
            m_affectorFactory[strAffectorName] = new CFactory< IParticleAffector, T >();
        }

    private:

        void InitializeParticleSystemScripts();

        mapRenderers m_renderers;
        mapAffectorFactory m_affectorFactory;
        mapParticleSystemScripts m_particleSystemScripts;

        CScript* m_pParticleScript;
    };


} // env
#endif // PARTICLE_SYSTEM_MGR