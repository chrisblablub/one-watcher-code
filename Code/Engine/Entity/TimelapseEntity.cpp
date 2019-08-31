#include <Engine/Entity/TimelapseEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CTimelapseEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_TIMELAPSE;

    const std::string PARAM_TIMELAPSE_EXECUTE = "Execute";

    //-----------------------------------------------------------------------------------
    CTimelapseEntity::CTimelapseEntity()
        : m_timeMultiplier(1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Timelapse] Execute", TIMELAPSE_EVENT_EXECUTE);
        RegisterEventParameter(TIMELAPSE_EVENT_EXECUTE, PARAM_TIMELAPSE_EXECUTE, CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f);
    }

    //-----------------------------------------------------------------------------------
    CTimelapseEntity::~CTimelapseEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CTimelapseEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CTimelapseEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        m_bGhost = false;
    }
 
    //-----------------------------------------------------------------------------------
    void CTimelapseEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    //-----------------------------------------------------------------------------------
    void CTimelapseEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_timeMultiplier = 1.0f;
    }

    //-----------------------------------------------------------------------------------
    void CTimelapseEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case TIMELAPSE_EVENT_EXECUTE:
        {
            if (eventParameter.IsParameterSet(PARAM_TIMELAPSE_EXECUTE))
                eventParameter.GetParameter(PARAM_TIMELAPSE_EXECUTE, m_timeMultiplier);

            gEngine->GetWorldMgr()->SetTimeMultiplier(m_timeMultiplier);
        }
        break;

        default:
        {
            CSpriteEntity::OnInput(eventId, eventParameter);
        }
        }
    }
} // env