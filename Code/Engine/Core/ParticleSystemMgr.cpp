#include <Engine/Core/ParticleSystemMgr.h>
#include <Engine/Entity/EntityMgr.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CParticleSystemMgr::CParticleSystemMgr()
        : m_pParticleScript(NULL)
    {
    }

    //-----------------------------------------------------------------------------------
    CParticleSystemMgr::~CParticleSystemMgr()
    {
        //gScriptMgr->Release("particleSystems.txt");

        Reset();
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemMgr::Reset()
    {
        for (mapAffectorFactory::iterator it = m_affectorFactory.begin(); it != m_affectorFactory.end(); ++it)
            ENV_DELETE(it->second);

        m_affectorFactory.clear();

        gScriptMgr->Release("ParticleSystems.txt");
        m_pParticleScript = NULL;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemMgr::Initialize()
    {
        m_pParticleScript = gScriptMgr->AddGetPtr("ParticleSystems.txt");

        InitializeParticleSystemScripts();
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemMgr::InitializeParticleSystemScripts()
    {
        m_particleSystemScripts.clear();

        CScript::mapSubscripts& particleSystemTypes = m_pParticleScript->GetSubscripts();
        for (CScript::mapSubscripts::iterator it = particleSystemTypes.begin(); it != particleSystemTypes.end(); ++it)
            m_particleSystemScripts[it->first] = it->second;
    }

    //-----------------------------------------------------------------------------------
    CScript* CParticleSystemMgr::GetParticleSystemTemplate(const std::string& strType)
    {
        mapParticleSystemScripts::iterator it = m_particleSystemScripts.find(strType);
        if (it == m_particleSystemScripts.end())
            return NULL;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    CScript* CParticleSystemMgr::CreateParticleSystemTemplate(const std::string& strName)
    {
        mapParticleSystemScripts::iterator it = m_particleSystemScripts.find(strName);
        if (it == m_particleSystemScripts.end())
        {
            CScript* pScript = m_pParticleScript->AddSubscript(strName);
            m_particleSystemScripts[strName] = pScript;
            return pScript;
        }
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemMgr::SaveTemplates()
    {
        m_pParticleScript->SaveScript();
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemMgr::GetParticleSystemTemplates(std::set< std::string >& particleSystems)
    {
        for (mapParticleSystemScripts::iterator it = m_particleSystemScripts.begin(); it != m_particleSystemScripts.end(); ++it)
            particleSystems.insert(it->first);
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemMgr::GetParticleSystemAffectors(std::set< std::string >& affectors)
    {
        for (mapAffectorFactory::iterator it = m_affectorFactory.begin(); it != m_affectorFactory.end(); ++it)
            affectors.insert(it->first);
    }

    //-----------------------------------------------------------------------------------
    IParticleAffector* CParticleSystemMgr::CreateParticleAffector(const std::string& strAffectorType)
    {
        return m_affectorFactory[strAffectorType]->Create();
    }
}