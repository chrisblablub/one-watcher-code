#include <Engine/Core/State.h>

namespace env
{

    //-----------------------------------------------------------------------------------
    CState::CState(const std::string& strName)
        : m_strName(strName)
        , m_bUseExclusively(false)
    {
        ENV_LOG_INFO("CREATING STATE " + strName);
    }

    //-----------------------------------------------------------------------------------
    CState::~CState()
    {

    }

    //-----------------------------------------------------------------------------------
    void CState::SetUsedExclusively(bool bExclusively)
    {
        m_bUseExclusively = bExclusively;
    }

    //-----------------------------------------------------------------------------------
    bool CState::Load(const std::string& strPreviousState)
    {
        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CState::Unload()
    {
        return true;
    }

    //-----------------------------------------------------------------------------------
    void CState::Update(float time, float dt, RenderQueue& renderQueue)
    {

    }
} // env