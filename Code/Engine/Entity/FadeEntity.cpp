#include <Engine/Entity/FadeEntity.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    ENTITY_TYPEID CFadeEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_FADE;
    
    const std::string PARAM_FADE_DURATION = "Fade Duration";
    const std::string PARAM_FADE_COLOR_BEGIN_TOP = "Color Begin [Top]";
    const std::string PARAM_FADE_COLOR_BEGIN_BOT = "Color Begin [Bottom]";
    const std::string PARAM_FADE_COLOR_END_TOP = "Color End [Top]";
    const std::string PARAM_FADE_COLOR_END_BOT = "Color End [Bottom]";

    const std::string PARAM_FADE_FRAME_DURATION = "Duration";
    const std::string PARAM_FADE_FRAME_DELAY = "Delay";
    const std::string PARAM_FADE_FRAME_TOP = "Top";
    const std::string PARAM_FADE_FRAME_LEFT = "Left";
    const std::string PARAM_FADE_FRAME_BOTTOM = "Bottom";
    const std::string PARAM_FADE_FRAME_RIGHT = "Right";

    const std::string PARAM_FADE_BARS_DURATION = "Duration";
    const std::string PARAM_FADE_BARS_TYPE = "Type";
    const std::string PARAM_FADE_BARS_AMOUNT = "Amount";

    //-----------------------------------------------------------------------------------
    CFadeEntity::CFadeEntity()
        : m_screenDuration(2.0f)
        , m_fadeScreenTime(1.0f)
        , m_currentFadeScreenTime(-1.0f)

        , m_fadeBeginColorTop(0.0f, 0.0f, 0.0f, 0.0f)
        , m_fadeEndColorTop(0.0f, 0.0f, 0.0f, 0.0f)
        , m_fadeBeginColorBot(0.0f, 0.0f, 0.0f, 0.0f)
        , m_fadeEndColorBot(0.0f, 0.0f, 0.0f, 0.0f)
        , m_currentFadeColorTop(0.0f)
        , m_currentFadeColorBot(0.0f)
        
        , m_FadeBarsAmount(0.0f, 0.0f)

        , m_currentScreenTime(0.0f)
        , m_currentFadeBarsTime(-1.0f)
        , m_currentFadeFrameTime(-1.0f)
        , m_currentFadeFrameDelay(0.0f)

        , m_bInitTemplate(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Fade] Reset", FADE_EVENT_RESET);

        // FadeScreen
        RegisterEvent("[FadeScreen] Execute", FADE_EVENT_SCREEN);
        RegisterEventParameter(FADE_EVENT_SCREEN, PARAM_FADE_DURATION, CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(FADE_EVENT_SCREEN, PARAM_FADE_COLOR_BEGIN_TOP, CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 0), Vec4(0), Vec4(1));
        RegisterEventParameter(FADE_EVENT_SCREEN, PARAM_FADE_COLOR_BEGIN_BOT, CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 0), Vec4(0), Vec4(1));
        RegisterEventParameter(FADE_EVENT_SCREEN, PARAM_FADE_COLOR_END_TOP, CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 1), Vec4(0), Vec4(1));
        RegisterEventParameter(FADE_EVENT_SCREEN, PARAM_FADE_COLOR_END_BOT, CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 1), Vec4(0), Vec4(1));


        // FadeFrame
        RegisterEvent("[FadeFrame] SetParameter", FADE_EVENT_FRAME_SET_PARAMS);
        RegisterEventParameter(FADE_EVENT_FRAME_SET_PARAMS, PARAM_FADE_FRAME_DURATION, CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f);
        RegisterEventParameter(FADE_EVENT_FRAME_SET_PARAMS, PARAM_FADE_FRAME_DELAY, CBaseParam::PARAMETER_TYPE_FLOAT, 0.0f);
        RegisterEventParameter(FADE_EVENT_FRAME_SET_PARAMS, PARAM_FADE_FRAME_TOP, CBaseParam::PARAMETER_TYPE_VECTOR2, Vec2(0.0f), Vec2(0.0f), Vec2(1.0f));
        RegisterEventParameter(FADE_EVENT_FRAME_SET_PARAMS, PARAM_FADE_FRAME_LEFT, CBaseParam::PARAMETER_TYPE_VECTOR2, Vec2(0.0f), Vec2(0.0f), Vec2(1.0f));
        RegisterEventParameter(FADE_EVENT_FRAME_SET_PARAMS, PARAM_FADE_FRAME_BOTTOM, CBaseParam::PARAMETER_TYPE_VECTOR2, Vec2(0.0f), Vec2(0.0f), Vec2(1.0f));
        RegisterEventParameter(FADE_EVENT_FRAME_SET_PARAMS, PARAM_FADE_FRAME_RIGHT, CBaseParam::PARAMETER_TYPE_VECTOR2, Vec2(0.0f), Vec2(0.0f), Vec2(1.0f));

        RegisterEvent("[FadeFrame] Execute", FADE_EVENT_FRAME_EXECUTE);

        std::string strFadeTooltip;
        strFadeTooltip += "[1] = Top + Bottom, ";
        strFadeTooltip += "[2] = Right -> Left, ";
        strFadeTooltip += "[3] = Left -> Right, ";
        strFadeTooltip += "[4] = Top -> Bottom, ";
        strFadeTooltip += "[5] = Bottom -> Top";

        // FadeBars
        RegisterEvent("[FadeBars] Execute", FADE_EVENT_BARS);
        RegisterEventParameter(FADE_EVENT_BARS, PARAM_FADE_BARS_DURATION, CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f);
        RegisterEventParameter(FADE_EVENT_BARS, PARAM_FADE_BARS_TYPE, CBaseParam::PARAMETER_TYPE_INT, strFadeTooltip, 1, (int)FADE_NONE, (int)FADE_BARS_COUNT);
        RegisterEventParameter(FADE_EVENT_BARS, PARAM_FADE_BARS_AMOUNT, CBaseParam::PARAMETER_TYPE_VECTOR2, "[Begin, End]", Vec2(0.0f), Vec2(0.0f), Vec2(1.0f));
        RegisterEventParameter(FADE_EVENT_BARS, "Color Begin", CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 1), Vec4(0), Vec4(1));
        RegisterEventParameter(FADE_EVENT_BARS, "Color End", CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 1), Vec4(0), Vec4(1));


    }

    //-----------------------------------------------------------------------------------
    CFadeEntity::~CFadeEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CFadeEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        SetMaterialFilename("Sprites/screenfader.png");
        SetRenderGame(false);
        SetRenderEditor(true);
    }

    //-----------------------------------------------------------------------------------
    void CFadeEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        SetRenderGame(false);
        SetRenderEditor(true);

        if (m_bInitTemplate)
            LoadTemplate(m_strTemplate);
    }

    //-----------------------------------------------------------------------------------
    void CFadeEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (!GetGameMode())
            return;

        // Screen
        if (m_currentFadeScreenTime >= 0.0f)
        {
            float alpha = Equals(m_fadeScreenTime, 0.0f) ? 0.0f : m_currentFadeScreenTime / m_fadeScreenTime;

            m_currentFadeColorTop = Lerp(m_fadeEndColorTop, m_fadeBeginColorTop, alpha);
            m_currentFadeColorBot = Lerp(m_fadeEndColorBot, m_fadeBeginColorBot, alpha);

            if (Equals(m_currentFadeScreenTime, 0.0f))
                m_currentFadeScreenTime = -1.0f;
            else
                m_currentFadeScreenTime = std::max(m_currentFadeScreenTime - dt, 0.0f);
        }

        if (m_currentFadeColorTop.a > 0.000001f ||
            m_currentFadeColorBot.a > 0.000001f)
        {
            queue.m_fadeScreenRenderObject.m_fadeScreenColorTop = m_currentFadeColorTop;
            queue.m_fadeScreenRenderObject.m_fadeScreenColorBot = m_currentFadeColorBot;
        }

        // Frame
        if (m_currentFadeFrameTime >= 0.0f)
        {
            float alpha = Clamp(m_currentFadeFrameTime / m_currentFadeFrameParams.m_fadeFrameDuration, 0.0f, 1.0f);
            float alphaSquared = alpha * alpha;
            float alphaInvSquared = 1.0f - alpha;
            alphaInvSquared *= alphaInvSquared;
            alphaInvSquared = 1.0f - alphaInvSquared;
            
            Vec4 fadeFrame;
            fadeFrame.x = Lerp(m_currentFadeFrameParams.m_fadeFrameParamsEnd.x, m_currentFadeFrameParams.m_fadeFrameParamsBegin.x, /*m_currentFadeFrameParams.m_fadeFrameParamsEnd.x > m_currentFadeFrameParams.m_fadeFrameParamsBegin.x ? alphaInvSquared :*/ alphaSquared);
            fadeFrame.y = Lerp(m_currentFadeFrameParams.m_fadeFrameParamsEnd.y, m_currentFadeFrameParams.m_fadeFrameParamsBegin.y, /*m_currentFadeFrameParams.m_fadeFrameParamsEnd.y > m_currentFadeFrameParams.m_fadeFrameParamsBegin.y ? alphaInvSquared :*/ alphaSquared);
            fadeFrame.z = Lerp(m_currentFadeFrameParams.m_fadeFrameParamsEnd.z, m_currentFadeFrameParams.m_fadeFrameParamsBegin.z, /*m_currentFadeFrameParams.m_fadeFrameParamsEnd.z > m_currentFadeFrameParams.m_fadeFrameParamsBegin.z ? alphaInvSquared :*/ alphaSquared);
            fadeFrame.w = Lerp(m_currentFadeFrameParams.m_fadeFrameParamsEnd.w, m_currentFadeFrameParams.m_fadeFrameParamsBegin.w, /*m_currentFadeFrameParams.m_fadeFrameParamsEnd.w > m_currentFadeFrameParams.m_fadeFrameParamsBegin.w ? alphaInvSquared :*/ alphaSquared);

            Vec4 fadeFrameColorTop = Lerp(m_currentFadeFrameParams.m_fadeFrameEndColorTop, m_currentFadeFrameParams.m_fadeFrameBeginColorTop, alpha);
            Vec4 fadeFrameColorBot = Lerp(m_currentFadeFrameParams.m_fadeFrameEndColorBot, m_currentFadeFrameParams.m_fadeFrameBeginColorBot, alpha);

            if (fadeFrame.x > 0.0f ||
                fadeFrame.y > 0.0f ||
                fadeFrame.z > 0.0f ||
                fadeFrame.w > 0.0f)
            {
                queue.m_fadeFrameRenderObject.m_fadeFrame = fadeFrame;
                queue.m_fadeFrameRenderObject.m_fadeFrameColorBot = fadeFrameColorBot;
                queue.m_fadeFrameRenderObject.m_fadeFrameColorTop = fadeFrameColorTop;
            }

            m_currentFadeFrameDelay -= dt;

            if (m_currentFadeFrameDelay < 0.0f)
                m_currentFadeFrameTime = std::max(m_currentFadeFrameTime - dt, 0.0f);
        }

        // Bars
        if (m_currentFadeBarsTime >= 0.0f)
        {
            float alpha = Clamp(m_currentFadeBarsTime / m_fadeBarsTime, 0.0f, 1.0f);
            alpha *= alpha;

            float fadeFrame = Lerp(m_FadeBarsAmount.y, m_FadeBarsAmount.x, alpha);
            Vec4 clr = Lerp(m_fadeBarsEndColor, m_fadeBarsBeginColor, alpha);

            if (fadeFrame > 0.0f)
            {
                m_currentFade.fadeBarsType = (FADE_BARS_TYPE)m_fadeBarsType;
                m_currentFade.fadeBarsAmount = fadeFrame;
                m_currentFade.fadeColor = clr;

                queue.m_pFadeBarsRenderObject = &m_currentFade;
            }

            m_currentFadeBarsTime = std::max(m_currentFadeBarsTime - dt, 0.0f);
        }
    }

    //-----------------------------------------------------------------------------------
    void CFadeEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_currentFadeScreenTime = -1.0f;
        m_currentScreenTime = -1.0f;

        m_fadeBeginColorTop = Vec4(0.0f);
        m_fadeBeginColorBot = Vec4(0.0f);
        m_fadeEndColorTop = Vec4(0.0f);
        m_fadeEndColorBot = Vec4(0.0f);

        m_currentFadeColorTop = m_fadeBeginColorTop;
        m_currentFadeColorBot = m_fadeBeginColorBot;

        m_currentFade.fadeBarsAmount = 0.0f;
        m_currentFade.fadeBarsType = FADE_NONE;

        m_currentFadeBarsTime = -1.0f;
        m_currentFadeScreenTime = -1.0f;
        m_currentFadeFrameTime = -1.0f;
        m_currentFadeFrameDelay = 0.0f;

        m_currentFadeFrameParams = m_defaultFadeFrameParams;
    }

    //-----------------------------------------------------------------------------------
    void CFadeEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case FADE_EVENT_RESET:
        {
            m_currentFadeFrameTime = -1.0f;
            m_currentFadeFrameDelay = 0.0f;
            m_currentFadeBarsTime = -1.0f;
            m_currentFadeScreenTime = -1.0f;
            m_currentFade.fadeBarsAmount = 0.0f;
        }
        break;

        case FADE_EVENT_FRAME_SET_PARAMS:
        {
            Vec2 top, left, bottom, right;
            if (eventParameter.IsParameterSet(PARAM_FADE_FRAME_TOP))
                eventParameter.GetParameter(PARAM_FADE_FRAME_TOP, top);

            if (eventParameter.IsParameterSet(PARAM_FADE_FRAME_LEFT))
                eventParameter.GetParameter(PARAM_FADE_FRAME_LEFT, left);

            if (eventParameter.IsParameterSet(PARAM_FADE_FRAME_BOTTOM))
                eventParameter.GetParameter(PARAM_FADE_FRAME_BOTTOM, bottom);

            if (eventParameter.IsParameterSet(PARAM_FADE_FRAME_RIGHT))
                eventParameter.GetParameter(PARAM_FADE_FRAME_RIGHT, right);

            if (eventParameter.IsParameterSet(PARAM_FADE_FRAME_DURATION))
                eventParameter.GetParameter(PARAM_FADE_FRAME_DURATION, m_currentFadeFrameParams.m_fadeFrameDuration);

            if (eventParameter.IsParameterSet(PARAM_FADE_FRAME_DELAY))
                eventParameter.GetParameter(PARAM_FADE_FRAME_DELAY, m_currentFadeFrameParams.m_fadeFrameDelay);

            m_currentFadeFrameParams.m_fadeFrameParamsBegin.x = left.x;
            m_currentFadeFrameParams.m_fadeFrameParamsBegin.y = right.x;
            m_currentFadeFrameParams.m_fadeFrameParamsBegin.z = top.x;
            m_currentFadeFrameParams.m_fadeFrameParamsBegin.w = bottom.x;

            m_currentFadeFrameParams.m_fadeFrameParamsEnd.x = left.y;
            m_currentFadeFrameParams.m_fadeFrameParamsEnd.y = right.y;
            m_currentFadeFrameParams.m_fadeFrameParamsEnd.z = top.y;
            m_currentFadeFrameParams.m_fadeFrameParamsEnd.w = bottom.y;
        } 
        break; 

        case FADE_EVENT_FRAME_EXECUTE:
        {
            m_currentFadeFrameTime = m_currentFadeFrameParams.m_fadeFrameDuration;
            m_currentFadeFrameDelay = m_currentFadeFrameParams.m_fadeFrameDelay;
        } break;

        case FADE_EVENT_SCREEN:
        {
            if (eventParameter.IsParameterSet(PARAM_FADE_DURATION))
                eventParameter.GetParameter(PARAM_FADE_DURATION, m_fadeScreenTime);

            if (eventParameter.IsParameterSet(PARAM_FADE_COLOR_BEGIN_TOP))
                eventParameter.GetParameter(PARAM_FADE_COLOR_BEGIN_TOP, m_fadeBeginColorTop);

            if (eventParameter.IsParameterSet(PARAM_FADE_COLOR_BEGIN_BOT))
                eventParameter.GetParameter(PARAM_FADE_COLOR_BEGIN_BOT, m_fadeBeginColorBot);

            if (eventParameter.IsParameterSet(PARAM_FADE_COLOR_END_TOP))
                eventParameter.GetParameter(PARAM_FADE_COLOR_END_TOP, m_fadeEndColorTop);

            if (eventParameter.IsParameterSet(PARAM_FADE_COLOR_END_BOT))
                eventParameter.GetParameter(PARAM_FADE_COLOR_END_BOT, m_fadeEndColorBot);

            m_currentFadeScreenTime = m_fadeScreenTime;
        } 
        break;

        case FADE_EVENT_BARS:
        {
            m_fadeBarsBeginColor = Vec4(0);
            m_fadeBarsEndColor = Vec4(0);

            if (eventParameter.IsParameterSet(PARAM_FADE_BARS_DURATION))
                eventParameter.GetParameter(PARAM_FADE_BARS_DURATION, m_fadeBarsTime);

            if (eventParameter.IsParameterSet(PARAM_FADE_BARS_TYPE))
                eventParameter.GetParameter(PARAM_FADE_BARS_TYPE, m_fadeBarsType);

            if (eventParameter.IsParameterSet(PARAM_FADE_BARS_AMOUNT))
                eventParameter.GetParameter(PARAM_FADE_BARS_AMOUNT, m_FadeBarsAmount);

            if (eventParameter.IsParameterSet("Color Begin"))
                eventParameter.GetParameter("Color Begin", m_fadeBarsBeginColor);

            if (eventParameter.IsParameterSet("Color End"))
                eventParameter.GetParameter("Color End", m_fadeBarsEndColor);

            m_currentFadeBarsTime = m_fadeBarsTime;
        }
        break;

        default:
        {
            CSpriteEntity::OnInput(eventId, eventParameter);
        }
        break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CFadeEntity::SaveTemplate(const std::string& strFilename, const std::string& strName)
    {
        m_strTemplate = strName;

        CScript script;

        script["templateInit"] = m_bInitTemplate;
        CScript& frameScript = *script.AddSubscript("frame");

        m_defaultFadeFrameParams.SaveTemplate(frameScript);

        script.SaveScript(strFilename);
    }

    //-----------------------------------------------------------------------------------
    void CFadeEntity::LoadTemplate(const std::string& strName)
    {
        m_strTemplate = strName;

        CScript* pScript = gScriptMgr->AddGetPtr(strName);

        if (!pScript)
            return;

        if (pScript->Exists("templateInit"))
            m_bInitTemplate = (*pScript)["templateInit"];

        if (pScript->SubscriptExists("frame"))
            m_defaultFadeFrameParams.LoadTemplate(*pScript->GetSubscript("frame"));

        gScriptMgr->Release(strName);
    }
} // env