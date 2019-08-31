#include <Engine/Entity/SkyDomeEntity.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CSkyDomeEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SKYDOME;

    //-----------------------------------------------------------------------------------
    CSkyDomeEntity::CSkyDomeEntity()
        : m_gradientColor1(1.0f, 1.0f, 1.0f),
        m_gradientColor2(1.0f, 1.0f, 1.0f),
        m_energyFieldColor(1.0f, 0.5f, 0.0f),
        m_currentEnergyFieldParam(1.0f),
        m_currentEnergyFieldTime(0.0f),
        m_flickerTime(8.0f),
        m_flickerTargetTime(0.0f),
        m_flickerAmount(1.0f),
        m_flickerRandomness(0.5f),
        m_flickerPauseTime(15.5f),
        m_pSkyTexture(NULL)
    {
        SetMetaTypeID(CSkyDomeEntity::ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CSkyDomeEntity::~CSkyDomeEntity()
    {
        if (m_pSkyTexture)
            gTextureMgr->Release(m_pSkyTexture->GetName());
    }

    //-----------------------------------------------------------------------------------
    void CSkyDomeEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Skydome.x");
    }

    //-----------------------------------------------------------------------------------
    void CSkyDomeEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    //-----------------------------------------------------------------------------------
    void CSkyDomeEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        if (!m_strSkyTexture.empty())
        {
            m_strSkyTexture = "SkyDome01.dds";

            m_pSkyTexture = gTextureMgr->AddGetPtr("Skies/" + m_strSkyTexture);

            if (m_pSkyTexture)
            {
                CTextureSampler textureSampler(TEXTUREADDRESS_TYPE_WRAP, TEXTUREADDRESS_TYPE_WRAP,
                                               TEXTUREFILTER_TYPE_ANISOTROPIC, TEXTUREFILTER_TYPE_LINEAR, TEXTUREFILTER_TYPE_LINEAR);

                textureSampler.SetMaxAnisotropy(4);

                m_pSkyTexture->SetTextureSampler(textureSampler);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CSkyDomeEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        m_pNode->SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition() + Vec3(0, 0, 2500));

        m_currentEnergyFieldTime -= dt;

        if (m_bEnergyField)
        {
            if (m_currentEnergyFieldTime >= m_flickerTargetTime * 0.8f)
            {
                float flickerParam = m_flickerAmount * (m_currentEnergyFieldTime - m_flickerTargetTime * 0.8f) / (m_flickerTargetTime * 0.2f);

                m_currentEnergyFieldParam = 1.0f - std::min((1.0f - m_flickerAmount) + flickerParam, 1.0f);
                //m_sprite.SetAlpha(min((1.0f - m_flickerAmount) + flickerParam, 1.0f));

                //m_pLight->SetEnergy((1.0f - m_flickerAmount) * m_pLight->GetInitialEnergy() +  m_pLight->GetInitialEnergy() * flickerParam);
            }
            else if (m_currentEnergyFieldTime <= m_flickerTargetTime * 0.2f)
            {
                float flickerParam = m_flickerAmount * (1.0f - m_currentEnergyFieldTime / (m_flickerTargetTime * 0.2f));

                m_currentEnergyFieldParam = 1.0f - std::min((1.0f - m_flickerAmount) + flickerParam, 1.0f);
                //m_sprite.SetAlpha(min((1.0f - m_flickerAmount) + flickerParam, 1.0f));

                //m_pLight->SetEnergy((1.0f - m_flickerAmount) * m_pLight->GetInitialEnergy() + m_pLight->GetInitialEnergy() * flickerParam);
            }
            else
            {
                m_currentEnergyFieldParam = 1.0f;
            }
        }
        else
        {
            m_currentEnergyFieldParam = 0.0f;
            //m_pLight->SetEnergy(m_pLight->GetInitialEnergy());
            //VECTOR4 clr = m_sprite.GetVec4Color();
            //clr.w = 1.0f;
            //m_sprite.SetColor(clr);
        }

        if (m_currentEnergyFieldTime < 0.0f)
        {
            m_bEnergyField = !m_bEnergyField;

            if (m_bEnergyField)
            {
                m_currentEnergyFieldTime = m_flickerTime + m_flickerTime * GetRandomFloat(-m_flickerRandomness, m_flickerRandomness);
            }
            else
            {
                m_currentEnergyFieldTime = m_flickerPauseTime + m_flickerPauseTime * GetRandomFloat(-m_flickerRandomness, m_flickerRandomness);
            }
            m_flickerTargetTime = m_currentEnergyFieldTime;

        }
    }

    //-----------------------------------------------------------------------------------
    void CSkyDomeEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        m_skyDomeRenderObject.m_meshInstances.clear();
        GetMeshInstanceContainer().GetMeshInstanceRenderObjects(m_skyDomeRenderObject.m_meshInstances);

        m_skyDomeRenderObject.m_pSkyTexture = m_pSkyTexture;

        renderQueue.m_pSkyDomeRenderObject = &m_skyDomeRenderObject;
    }

    //-----------------------------------------------------------------------------------
    void CSkyDomeEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    void CSkyDomeEntity::SetSkyTextureName(const std::string& strTex)
    {
        if (m_pSkyTexture)
            gTextureMgr->Release(m_pSkyTexture->GetName());

        m_strSkyTexture = strTex;

        if (!m_strSkyTexture.empty())
            m_pSkyTexture = gTextureMgr->AddGetPtr("Skies/" + m_strSkyTexture);
    }







    ENTITY_TYPEID CHaloEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_HALO;

    //-----------------------------------------------------------------------------------
    CHaloEntity::CHaloEntity()
        : m_startShowTime(-1.0f)
        , m_startHideTime(-1.0f)
        , m_intensityLeftTime(-1.0f)
        , m_intensityRightTime(-1.0f)
        , m_intensityLeftDuration(1.0f)
        , m_intensityRightDuration(1.0f)
        , m_beginIntensityLeft(0.0f)
        , m_endIntensityLeft(0.0f)
        , m_beginIntensityRight(0.0f)
        , m_endIntensityRight(0.0f)
    {
        SetMetaTypeID(CHaloEntity::ENTITY_META_TYPEID);

        RegisterEvent("[Halo] Show", HALO_EVENT_SHOW);
        RegisterEvent("[Halo] Hide", HALO_EVENT_HIDE);
        RegisterEvent("[Halo] Start Show", HALO_EVENT_STARTSHOW);
        RegisterEvent("[Halo] Start Hide", HALO_EVENT_STARTHIDE);

        // SetIntensity
        RegisterEvent("[Halo] SetIntensity", HALO_EVENT_SET_INTENSITY);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100000.0f);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY, "Begin", CBaseParam::PARAMETER_TYPE_FLOAT, 0.0f, 0.0f, 1.0f);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY, "End", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 1.0f);

        RegisterEvent("[Halo] SetIntensityLeft", HALO_EVENT_SET_INTENSITY_LEFT);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY_LEFT, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100000.0f);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY_LEFT, "Begin", CBaseParam::PARAMETER_TYPE_FLOAT, 0.0f, 0.0f, 1.0f);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY_LEFT, "End", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 1.0f);

        RegisterEvent("[Halo] SetIntensityRight", HALO_EVENT_SET_INTENSITY_RIGHT);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY_RIGHT, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100000.0f);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY_RIGHT, "Begin", CBaseParam::PARAMETER_TYPE_FLOAT, 0.0f, 0.0f, 1.0f);
        RegisterEventParameter(HALO_EVENT_SET_INTENSITY_RIGHT, "End", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 1.0f);
    }

    //-----------------------------------------------------------------------------------
    CHaloEntity::~CHaloEntity()
    {
    }

    const float HALO_SHOW_TIME = 4.5f;
    const float HALO_HIDE_TIME = 4.5f;

    //-----------------------------------------------------------------------------------
    void CHaloEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case HALO_EVENT_SHOW:
            m_haloRenderObject.m_haloIntensityLeft = 0.0f;
            m_haloRenderObject.m_haloIntensityRight = 0.0f;
            break;

        case HALO_EVENT_HIDE:
            m_haloRenderObject.m_haloIntensityLeft = 1.0f;
            m_haloRenderObject.m_haloIntensityRight = 1.0f;
            break;

        case HALO_EVENT_STARTSHOW:
            m_startShowTime = HALO_SHOW_TIME;
            break;

        case HALO_EVENT_STARTHIDE:
            m_startHideTime = HALO_HIDE_TIME;
            break;

        case HALO_EVENT_SET_INTENSITY:

            if(eventParameter.IsParameterSet("Begin"))
                eventParameter.GetParameter("Begin", m_beginIntensityLeft);

            if (eventParameter.IsParameterSet("End"))
                eventParameter.GetParameter("End", m_endIntensityLeft);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_intensityLeftDuration);

            m_intensityLeftTime = m_intensityLeftDuration;
            
            m_intensityRightDuration = m_intensityLeftDuration;
            m_intensityRightTime = m_intensityLeftTime;
            m_beginIntensityRight = m_beginIntensityLeft;
            m_endIntensityRight = m_endIntensityLeft;
      
            break;

        case HALO_EVENT_SET_INTENSITY_LEFT:

            if (eventParameter.IsParameterSet("Begin"))
                eventParameter.GetParameter("Begin", m_beginIntensityLeft);

            if (eventParameter.IsParameterSet("End"))
                eventParameter.GetParameter("End", m_endIntensityLeft);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_intensityLeftDuration);

            m_intensityLeftTime = m_intensityLeftDuration;

            break;

        case HALO_EVENT_SET_INTENSITY_RIGHT:

            if (eventParameter.IsParameterSet("Begin"))
                eventParameter.GetParameter("Begin", m_beginIntensityRight);

            if (eventParameter.IsParameterSet("End"))
                eventParameter.GetParameter("End", m_endIntensityRight);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_intensityRightDuration);

            m_intensityRightTime = m_intensityRightDuration;

            break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
        }
    }

    //-----------------------------------------------------------------------------------
    void CHaloEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("halo.x");

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    //-----------------------------------------------------------------------------------
    void CHaloEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
    }

    //-----------------------------------------------------------------------------------
    void CHaloEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        if (m_startShowTime >= 0.0f)
        {
            m_startShowTime -= dt;

            m_haloRenderObject.m_haloIntensityLeft = m_startShowTime / HALO_SHOW_TIME;
        }

        if (m_startHideTime >= 0.0f)
        {
            m_startHideTime -= dt;

            m_haloRenderObject.m_haloIntensityLeft = m_startHideTime / HALO_HIDE_TIME;
        }

        if (m_intensityLeftTime >= 0.0f)
        {
            m_intensityLeftTime -= dt;

            float alpha = 0.0f;

            if (m_intensityLeftDuration > 0.0f)
                alpha = Clamp(m_intensityLeftTime / m_intensityLeftDuration, 0.0f, 1.0f);

            m_haloRenderObject.m_haloIntensityLeft = Lerp(m_endIntensityLeft, m_beginIntensityLeft, alpha);
        }

        if (m_intensityRightTime >= 0.0f)
        {
            m_intensityRightTime -= dt;

            float alpha = 0.0f;

            if (m_intensityRightDuration > 0.0f)
                alpha = Clamp(m_intensityRightTime / m_intensityRightDuration, 0.0f, 1.0f);

            m_haloRenderObject.m_haloIntensityRight = Lerp(m_endIntensityRight, m_beginIntensityRight, alpha);
        }
    }

    //-----------------------------------------------------------------------------------
    void CHaloEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_haloRenderObject.m_haloIntensityLeft = 1.0f;
        m_haloRenderObject.m_haloIntensityRight = 1.0f;

        m_startShowTime = -1.0f;
        m_startHideTime = -1.0f;
        m_intensityLeftDuration = -1.0f;
        m_intensityRightDuration = -1.0f;
    }

    //-----------------------------------------------------------------------------------
    void CHaloEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        m_haloRenderObject.m_meshInstances.clear();
        GetMeshInstanceContainer().GetMeshInstanceRenderObjects(m_haloRenderObject.m_meshInstances);

        renderQueue.m_pHaloRenderObject = &m_haloRenderObject;
    }
} // env
