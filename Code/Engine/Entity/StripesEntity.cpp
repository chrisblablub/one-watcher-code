
#include <Engine/Entity/StripesEntity.h>

namespace env
{
    ENTITY_TYPEID CStripesEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_STRIPES;

    const std::string STRIPE_EVENT_PARAM_SPAWN_DURATION = "Spawn Duration";
    const std::string STRIPE_EVENT_PARAM_SPAWN_OFFSET = "Spawn Offset";
    const std::string STRIPE_EVENT_PARAM_FADE_DURATION = "Fade Duration";
    const std::string STRIPE_EVENT_PARAM_WIDTH_START = "Width Start";
    const std::string STRIPE_EVENT_PARAM_WIDTH_END = "Width End";
    const std::string STRIPE_EVENT_PARAM_GROW_VERTICALLY = "Grow Vertically";
    const std::string STRIPE_EVENT_PARAM_GROW_ALTERNATINGLY = "Grow Alternatingly";
    const std::string STRIPE_EVENT_PARAM_FADE_OUT = "Fade Out";

    //-----------------------------------------------------------------------------------
    CStripesEntity::CStripesEntity()
        : m_currentStripeTime(-1.0f)
        , m_stripeTime(-1.0f)
        , m_bInitTemplate(false)
    {
        SetMetaTypeID(CStripesEntity::ENTITY_META_TYPEID);

        // Reset
        RegisterEvent("[Stripes] Reset", STRIPE_EVENT_RESET);

        // Execute
        RegisterEvent("[Stripes] Execute", STRIPE_EVENT_EXECUTE);

        // SetParameter
        RegisterEvent("[Stripes] SetParameter", STRIPE_EVENT_SET_PARAMETER);
        RegisterEventParameter(STRIPE_EVENT_SET_PARAMETER, STRIPE_EVENT_PARAM_SPAWN_DURATION, CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(STRIPE_EVENT_SET_PARAMETER, STRIPE_EVENT_PARAM_SPAWN_OFFSET, CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(STRIPE_EVENT_SET_PARAMETER, STRIPE_EVENT_PARAM_FADE_DURATION, CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(STRIPE_EVENT_SET_PARAMETER, STRIPE_EVENT_PARAM_WIDTH_START, CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(STRIPE_EVENT_SET_PARAMETER, STRIPE_EVENT_PARAM_WIDTH_END, CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(STRIPE_EVENT_SET_PARAMETER, STRIPE_EVENT_PARAM_GROW_VERTICALLY, CBaseParam::PARAMETER_TYPE_BOOL);
        RegisterEventParameter(STRIPE_EVENT_SET_PARAMETER, STRIPE_EVENT_PARAM_GROW_ALTERNATINGLY, CBaseParam::PARAMETER_TYPE_BOOL);

        // SetSpawnDuration
        RegisterEvent("[Stripes] SetSpawnDuration", STRIPE_EVENT_SET_SPAWN_DURATION);
        RegisterEventParameter(STRIPE_EVENT_SET_SPAWN_DURATION, STRIPE_EVENT_PARAM_SPAWN_DURATION, CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f);

        // SetSpawnOffset
        RegisterEvent("[Stripes] SetSpawnOffset", STRIPE_EVENT_SET_SPAWN_OFFSET);
        RegisterEventParameter(STRIPE_EVENT_SET_SPAWN_OFFSET,  STRIPE_EVENT_PARAM_SPAWN_OFFSET, CBaseParam::PARAMETER_TYPE_FLOAT, 0.1f, 0.0f);

        // SetFadeDuration
        RegisterEvent("[Stripes] SetFadeDuration", STRIPE_EVENT_SET_FADE_DURATION);
        RegisterEventParameter(STRIPE_EVENT_SET_FADE_DURATION, STRIPE_EVENT_PARAM_FADE_DURATION, CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f);

        // SetWidthStart
        RegisterEvent("[Stripes] SetWidthStart", STRIPE_EVENT_SET_WIDTH_START);
        RegisterEventParameter(STRIPE_EVENT_SET_WIDTH_START, STRIPE_EVENT_PARAM_WIDTH_START, CBaseParam::PARAMETER_TYPE_FLOAT, 0.0f, 0.0f, 1.0f);

        // SetWidthEnd
        RegisterEvent("[Stripes] SetWidthEnd", STRIPE_EVENT_SET_WIDTH_END);
        RegisterEventParameter(STRIPE_EVENT_SET_WIDTH_END, STRIPE_EVENT_PARAM_WIDTH_END, CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 1.0f);

        // SetGrowVertically
        RegisterEvent("[Stripes] SetGrowVertically", STRIPE_EVENT_SET_GROW_VERTICALLY);
        RegisterEventParameter(STRIPE_EVENT_SET_GROW_VERTICALLY, STRIPE_EVENT_PARAM_GROW_VERTICALLY, CBaseParam::PARAMETER_TYPE_BOOL);

        // SetGrowAlternatingly
        RegisterEvent("[Stripes] SetGrowAlternatingly", STRIPE_EVENT_SET_GROW_ALTERNATINGLY);
        RegisterEventParameter(STRIPE_EVENT_SET_GROW_ALTERNATINGLY, STRIPE_EVENT_PARAM_GROW_ALTERNATINGLY, CBaseParam::PARAMETER_TYPE_BOOL);

        // SetFadeOut
        RegisterEvent("[Stripes] SetFadeOut", STRIPE_EVENT_SET_FADE_OUT);
        RegisterEventParameter(STRIPE_EVENT_SET_FADE_OUT, STRIPE_EVENT_PARAM_FADE_OUT, CBaseParam::PARAMETER_TYPE_BOOL);

        RegisterEvent("[Stripes] SetLoadingScreen", STRIPE_EVENT_SET_LOADING_SCREEN);
    }
    
    //-----------------------------------------------------------------------------------
    CStripesEntity::~CStripesEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CStripesEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();
        
        SetMaterialFilename("Sprites/stripes.png");
        SetRenderGame(false);
        SetRenderEditor(true);

        GenerateStripes();
    }
    
    //-----------------------------------------------------------------------------------
    void CStripesEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();
        
        if (!m_strTemplate.empty() && m_bInitTemplate)
            LoadTemplate(m_strTemplate);

        GenerateStripes();
    }
    
    //-----------------------------------------------------------------------------------
    void CStripesEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (m_currentStripeTime >= 0.0f)
            m_currentStripeTime -= dt;

        if (m_currentStripeTime >= 0.0f)
        {
            m_stripeManager.AddToRenderQueue(queue);

            if (m_stripeParams.m_bFadeOut)
            {
                if (m_currentStripeTime >= (m_stripeTime - m_stripeParams.m_timeFadeDuration))
                    queue.m_stripeRenderObject.m_stripesAmount = 0.0f;
                else
                    queue.m_stripeRenderObject.m_stripesAmount = Clamp(1.0f - (m_currentStripeTime) / m_stripeTime, 0.0f, 1.0f);
            }
            else
            {
                if (m_currentStripeTime < m_stripeParams.m_timeFadeDuration)
                    queue.m_stripeRenderObject.m_stripesAmount = 1.0f;
                else
                    queue.m_stripeRenderObject.m_stripesAmount = Clamp(1.0f - (m_currentStripeTime - m_stripeParams.m_timeFadeDuration) / m_stripeTime, 0.0f, 1.0f);
            }

            queue.m_stripeRenderObject.m_stripeTime = m_stripeTime;
        }
    }

    //-----------------------------------------------------------------------------------
    void CStripesEntity::OnEntityCloned(CEntity& entity)
    {
        CSpriteEntity::OnEntityCloned(entity);

        CStripesEntity& other = static_cast<CStripesEntity&>(entity);
        m_defaultStripeParams = other.m_defaultStripeParams;
    }
    
    //-----------------------------------------------------------------------------------
    void CStripesEntity::OnInput(unsigned int eventID, const EventParameter& eventParameter)
    {
        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_SPAWN_DURATION))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_SPAWN_DURATION, m_stripeParams.m_timeSpawnDuration);

        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_SPAWN_OFFSET))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_SPAWN_OFFSET, m_stripeParams.m_timeSpawnOffset);

        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_FADE_DURATION))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_FADE_DURATION, m_stripeParams.m_timeFadeDuration);

        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_WIDTH_START))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_WIDTH_START, m_stripeParams.m_startWidth);

        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_WIDTH_END))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_WIDTH_END, m_stripeParams.m_endWidth);

        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_GROW_VERTICALLY))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_GROW_VERTICALLY, m_stripeParams.m_bGrowVertically);

        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_GROW_ALTERNATINGLY))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_GROW_ALTERNATINGLY, m_stripeParams.m_bGrowAlternatingly);

        if (eventParameter.IsParameterSet(STRIPE_EVENT_PARAM_FADE_OUT))
            eventParameter.GetParameter(STRIPE_EVENT_PARAM_FADE_OUT, m_stripeParams.m_bFadeOut);

        switch (eventID)
        {
        case ENTITY_EVENT_PLAYER_FIRE:
        case STRIPE_EVENT_EXECUTE:
        {
            GenerateStripes();
            ExecStripes();
        } break;
        
        case STRIPE_EVENT_RESET:
        {
            m_stripeParams = m_defaultStripeParams;
            m_currentStripeTime = -1.0f;
        } break;

        case STRIPE_EVENT_SET_LOADING_SCREEN:
        {
            StripeRenderObject obj = m_stripeManager.GetStripeRenderObject();
            obj.m_stripesAmount = m_stripeParams.m_bFadeOut ? 0.0f : 1.0f;
            obj.m_stripeTime = m_stripeTime;

            gEngine->GetLevelLoader().SetStripeRenderObject(obj);

        } break;

        case STRIPE_EVENT_SET_PARAMETER:            break; 
        case STRIPE_EVENT_SET_SPAWN_DURATION:       break;
        case STRIPE_EVENT_SET_FADE_DURATION:        break;
        case STRIPE_EVENT_SET_SPAWN_OFFSET:         break;
        case STRIPE_EVENT_SET_WIDTH_START:          break;
        case STRIPE_EVENT_SET_WIDTH_END:            break;
        case STRIPE_EVENT_SET_GROW_VERTICALLY:      break;
        case STRIPE_EVENT_SET_GROW_ALTERNATINGLY:   break;
        default:
            CSpriteEntity::OnInput(eventID, eventParameter);
        }
    }
    
    //-----------------------------------------------------------------------------------
    void CStripesEntity::GenerateStripes()
    {
        const RenderQualityParams& renderParams = gRenderSystem->GetRenderQualityParams();

        float aspectRatio = static_cast<float>(renderParams.screenHeight) /
            static_cast<float>(renderParams.screenWidth);

        m_stripeParams.m_aspectRatio = aspectRatio;
        m_stripeTime = m_stripeManager.Generate(CDefaultStripeGenerator(m_stripeParams));
    }

    //-----------------------------------------------------------------------------------
    void CStripesEntity::ExecStripes()
    {
        if (!GetGameMode())
            return;

        m_currentStripeTime = m_stripeTime + m_stripeParams.m_timeFadeDuration;
    }

    //-----------------------------------------------------------------------------------
    void CStripesEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_stripeParams = m_defaultStripeParams;

        GenerateStripes();

        m_currentStripeTime = -1.0f;
    }

    //-----------------------------------------------------------------------------------
    void CStripesEntity::SaveTemplate(const std::string& strFilename, const std::string& strName)
    {
        m_strTemplate = strName;

        CScript script;

        script["templateInit"] = m_bInitTemplate;

        m_defaultStripeParams.SaveTemplate(script);

        script.SaveScript(strFilename);
    }

    //-----------------------------------------------------------------------------------
    void CStripesEntity::LoadTemplate(const std::string& strName)
    {
        m_strTemplate = strName;

        CScript* pScript = gScriptMgr->AddGetPtr(strName);

        if (!pScript)
            return;

        if (pScript->Exists("templateInit"))
            m_bInitTemplate = (*pScript)["templateInit"];

        m_defaultStripeParams.LoadTemplate(*pScript);

        gScriptMgr->Release(strName);
    }

} // env