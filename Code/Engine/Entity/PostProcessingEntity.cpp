#include <Engine/Entity/PostProcessingEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Scene/Light.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    ENTITY_TYPEID CPostProcessingEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_POSTPROCESSING;

    CPostProcessingEntity::CPostProcessingEntity()
        : m_targetBloomThreshold(10.0f),
        m_targetBloomScale(1.0f),
        m_startBloomScale(1.0f),
        m_startBloomThreshold(10.0f),
        m_fadeInTime(0.0f),
        m_currentFadeInTime(-1.0f),
        m_currentSaturationTime(-1.0f),
        m_saturationDuration(-1.0f),
        m_saturationBegin(1.0f),
        m_saturationEnd(1.0f),
        m_fadeInExponent(1.0f),
        m_currentSaturation(1.0f)
    {
        SetMetaTypeID(CPostProcessingEntity::ENTITY_META_TYPEID);

        RegisterEvent("[PostProcessing] Start", POSTPROCESSING_EVENT_START);
        RegisterEvent("[PostProcessing] Distortion light", POSTPROCESSING_EVENT_DISTORTIONLIGHT);
        RegisterEvent("[PostProcessing] Distortion strong", POSTPROCESSING_EVENT_DISTORTIONSTRONG);
        RegisterEvent("[PostProcessing] SetSaturation", POSTPROCESSING_EVENT_SET_SATURATION);
        RegisterEventParameter(POSTPROCESSING_EVENT_SET_SATURATION, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f);
        RegisterEventParameter(POSTPROCESSING_EVENT_SET_SATURATION, "Begin", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 1.0f);
        RegisterEventParameter(POSTPROCESSING_EVENT_SET_SATURATION, "End", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 1.0f);

        m_bGhost = true;
    }

    CPostProcessingEntity::~CPostProcessingEntity()
    {
    }

    void CPostProcessingEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        SetRenderGame(false);
    }

    void CPostProcessingEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        SetRenderGame(false);
    }

    void CPostProcessingEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_currentSaturationTime = -1.0f;
        m_currentFadeInTime = -1.0f;
        m_currentSaturation = 1.0f;
        //m_currentEnergy = m_pLight->GetEnergy();
    }


    void CPostProcessingEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (m_currentFadeInTime >= 0.0f)
        {
            float alpha = pow((m_currentFadeInTime / m_fadeInTime), m_fadeInExponent);

            float bloomThreshold = alpha * m_startBloomThreshold +
                (1.0f - alpha) * m_targetBloomThreshold;

            float bloomScale = alpha * m_startBloomScale +
                (1.0f - alpha) * m_targetBloomScale;

            gEngine->GetWorldMgr()->GetScenario()->SetCurrentBloomScale(bloomScale);
            gEngine->GetWorldMgr()->GetScenario()->SetCurrentBloomThreshold(bloomThreshold);

            m_currentFadeInTime -= dt;
        }

        if (m_currentSaturationTime >= 0.0f)
        {
            float alpha = Equals(m_saturationDuration, 0.0f) ? 0.0f : m_currentSaturationTime / m_saturationDuration;

            m_currentSaturation = Lerp(m_saturationEnd, m_saturationBegin, alpha);

            if (Equals(m_currentSaturationTime, 0.0f))
                m_currentSaturationTime = -1.0f;
            else
                m_currentSaturationTime = std::max(m_currentSaturationTime - dt, 0.0f);
        }

        queue.m_postProcessRenderObject.m_saturation *= m_currentSaturation;

        float m_distortTime = 0.2f;

        if (m_distortionTime >= 0.0f)
        {
            float alpha = 0.0f;
            if (m_distortionTime > (m_distortTime * 0.5f))
            {
                alpha = 1.0f - (m_distortionTime - m_distortTime * 0.5f) / (m_distortTime * 0.5f);
            }
            else
            {
                alpha = (m_distortionTime) / (m_distortTime * 0.5f);
            }


            queue.m_postProcessRenderObject.m_imageDistortion.alpha = alpha;
            queue.m_postProcessRenderObject.m_imageDistortion.amount = m_distortionAmount;
            queue.m_postProcessRenderObject.m_imageDistortion.scaling = m_distortionScaling;
            queue.m_postProcessRenderObject.m_imageDistortion.dir = m_distortionDirection;

            m_distortionTime -= dt;
        }
        else
        {
            /*m_distortionTime = 0.2f;

            m_distortionAmount = GetRandomFloat(0.3f, 0.6f);
            m_distortionOffset = GetRandomFloat(0.0f, 5.0f);

            if(sgn(GetRandomFloat(-1, 1)) >= 0)
            {
            // x
            m_distortionDirection = VECTOR2(0.1f, 5.0f);
            m_distortionScaling = VECTOR2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f),
            sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f));
            }
            else
            {
            // y
            m_distortionDirection = VECTOR2(5.0f, 0.1f);
            m_distortionScaling = VECTOR2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f),
            sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f));
            }*/
        }

    }

    void CPostProcessingEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);


    }

    void CPostProcessingEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {

        case POSTPROCESSING_EVENT_SET_SATURATION:

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_currentSaturationTime);

            if (eventParameter.IsParameterSet("Begin"))
                eventParameter.GetParameter("Begin", m_saturationBegin);

            if (eventParameter.IsParameterSet("End"))
                eventParameter.GetParameter("End", m_saturationEnd);

            m_saturationDuration = m_currentSaturationTime;

            break;

        case POSTPROCESSING_EVENT_START:
            m_currentFadeInTime = m_fadeInTime;

            m_startBloomScale = gEngine->GetWorldMgr()->GetScenario()->GetCurrentBloomScale();
            m_startBloomThreshold = gEngine->GetWorldMgr()->GetScenario()->GetCurrentBloomThreshold();
            break;

        case POSTPROCESSING_EVENT_DISTORTIONLIGHT:
        {
            m_distortionTime = 0.2f;
            m_distortionAmount = GetRandomFloat(0.3f, 0.6f);
            m_distortionOffset = GetRandomFloat(0.0f, 5.0f);

            //if(sgn(GetRandomFloat(-1, 1)) >= 0)
            {
                // x
                m_distortionDirection = Vec2(0.1f, 5.0f);
                m_distortionScaling = Vec2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f),
                                              sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f));
            }
            /*else
            {
            // y
            m_distortionDirection = VECTOR2(5.0f, 0.1f);
            m_distortionScaling = VECTOR2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f),
            sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f));
            }*/
        }
        break;

        case POSTPROCESSING_EVENT_DISTORTIONSTRONG:
        {
            m_distortionTime = 0.4f;
            m_distortionAmount = GetRandomFloat(0.6f, 1.2f);
            m_distortionOffset = GetRandomFloat(0.0f, 1.5f);

            //if(sgn(GetRandomFloat(-1, 1)) >= 0)
            {
                // x
                m_distortionDirection = Vec2(0.1f, 5.0f);
                m_distortionScaling = Vec2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f),
                                              sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f));
            }
            /*else
            {
            // y
            m_distortionDirection = VECTOR2(5.0f, 0.1f);
            m_distortionScaling = VECTOR2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f),
            sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f));
            }*/
        }
        break;

        default:
            CSpriteEntity::OnInput(eventId, eventParameter);
            break;
        }
    }


} /* End namespace */