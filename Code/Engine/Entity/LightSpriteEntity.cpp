#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Scene/Light.h>
#include <Engine/Scene/SceneMgr.h>

namespace env
{
    ENTITY_TYPEID CLightSpriteEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_LIGHTSPRITE;

    //-----------------------------------------------------------------------------------
    CLightSpriteEntity::CLightSpriteEntity()
        : m_pLight(0)
        , m_bFlicker(false)
        , m_bFlickering(false)
        , m_bInitFromTemplate(false)
        , m_bLightraysSet(false)
        , m_bLightraysInitial(false)
        , m_flickerAmount(0.5f)
        , m_flickerTime(0.1f)
        , m_flickerPauseTime(0.5f)
        , m_flickerRandomness(0.5f)
        , m_lightOffTime(-1.0f)
        , m_lightOnTime(-1.0f)
        , m_diffuseDuration(0.0f)
        , m_diffuseTime(-1.0f)
        , m_specularDuration(0.0f)
        , m_specularTime(-1.0f)
        , m_energyDuration(0.0f)
        , m_energyTime(-1.0f)
    {
        SetMetaTypeID(CLightSpriteEntity::ENTITY_META_TYPEID);

        RegisterEvent("[Light] On", LIGHT_EVENT_ON);
        RegisterEvent("[Light] Off", LIGHT_EVENT_OFF);

        RegisterEvent("[Light] SetDiffuseColor", LIGHT_EVENT_SET_DIFFUSE_COLOR);
        RegisterEventParameter(LIGHT_EVENT_SET_DIFFUSE_COLOR, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(LIGHT_EVENT_SET_DIFFUSE_COLOR, "Color", CBaseParam::PARAMETER_TYPE_VECTOR3, "[Format] = RGB", Vec3(0, 0, 0), Vec3(0), Vec3(1, 1, 1));

        RegisterEvent("[Light] SetSpecularColor", LIGHT_EVENT_SET_SPECULAR_COLOR);
        RegisterEventParameter(LIGHT_EVENT_SET_SPECULAR_COLOR, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(LIGHT_EVENT_SET_SPECULAR_COLOR, "Color", CBaseParam::PARAMETER_TYPE_VECTOR3, "[Format] = RGB", Vec3(0, 0, 0), Vec3(0), Vec3(1, 1, 1));

        RegisterEvent("[Light] SetLightrays", LIGHT_EVENT_SET_LIGHTRAYS);
        RegisterEventParameter(LIGHT_EVENT_SET_LIGHTRAYS, "Lightrays", CBaseParam::PARAMETER_TYPE_BOOL);

        RegisterEvent("[Light] SetEnergy", LIGHT_EVENT_SET_ENERGY);
        RegisterEventParameter(LIGHT_EVENT_SET_ENERGY, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(LIGHT_EVENT_SET_ENERGY, "Energy", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100.0f);
        
        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    CLightSpriteEntity::~CLightSpriteEntity()
    {
        gEngine->GetSceneMgr()->RemoveLight(m_pLight->GetName());
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        m_pLight = gEngine->GetSceneMgr()->CreateLight();
        m_pLight->Serialize();
        m_pLight->AttachNode(m_pNode);
        m_pLight->SetType(LT_POINT);

        CSpriteEntity::SetMaterialFilename("Sprites/light.dds");
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        CSpriteEntity::SetMaterialFilename("Sprites/light.dds");

        m_sprite.InitializeAtLoad();
        m_sprite.AttachNode(m_pNode);

        if (m_bInitFromTemplate)
            LoadTemplate(m_strTemplate, false);

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::OnEntityCloned(CEntity& entity)
    {
        CSpriteEntity::OnEntityCloned(entity);

        CLightSpriteEntity& other = static_cast<CLightSpriteEntity&>(entity);
        m_pLight->SetRadius(other.GetLight()->GetRadius());
        m_pLight->SetInitialEnergy(other.GetLight()->GetInitialEnergy());
        m_pLight->SetType(other.GetLight()->GetType());
        m_pLight->SetInitialDiffuseColor(other.GetLight()->GetInitialDiffuseColor());
        m_pLight->SetInitialSpecularColor(other.GetLight()->GetInitialSpecularColor());
        m_pLight->SetSpecular(other.GetLight()->GetSpecular());
        m_pLight->SetSpotOuterAngle(other.GetLight()->GetSpotOuterAngle());
        m_pLight->SetSpotInnerAngle(other.GetLight()->GetSpotInnerAngle());
        m_pLight->SetStatic(other.GetLight()->GetStatic());
        m_pLight->SetAffectsMeshes(other.GetLight()->GetAffectsMeshes());

        m_bInitFromTemplate = other.m_bInitFromTemplate;
        m_strTemplate = other.m_strTemplate;

        SetFlicker(other.GetFlicker());
        SetFlickerAmount(other.GetFlickerAmount());
        SetFlickerPauseTime(other.GetFlickerPauseTime());
        SetFlickerRandomness(other.GetFlickerRandomness());
        SetFlickerTime(other.GetFlickerTime());
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_bFlickering = false;

        m_flickerTargetTime = 1.0f;
        m_flickerCurrentTime = -1.0f;
        m_lightOffTime = -1.0f;
        m_lightOnTime = -1.0f;

        m_diffuseTime = -1.0f;
        m_diffuseDuration = 0.0f;

        m_specularTime = -1.0f;
        m_specularDuration = 0.0f;

        m_energyTime = -1.0f;
        m_energyDuration = 0.0f;

        if (m_bLightraysSet)
        {
            m_pLight->SetLightRays(m_bLightraysInitial);
            m_bLightraysSet = false;
        }
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case LIGHT_EVENT_ON:
            if (m_pLight)
            {
                m_lightOffTime = -1.0f;
                m_lightOnTime = 1.5f;
            }

            break;
        case LIGHT_EVENT_OFF:
            if (m_pLight)
            {
                m_lightOffTime = 1.5f;
                m_lightOnTime = -1.0f;
            }
            break;

        case LIGHT_EVENT_SET_DIFFUSE_COLOR:
        {
            float duration = 0.0f;
            Vec3 color = Vec3(0);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", duration);

            if (eventParameter.IsParameterSet("Color"))
                eventParameter.GetParameter("Color", color);

            m_diffuseColorBegin = m_pLight->GetDiffuseColor();
            m_diffuseColorEnd = color;

            m_diffuseDuration = duration;
            m_diffuseTime = duration;
        }
            break;

        case LIGHT_EVENT_SET_SPECULAR_COLOR:
        {
            float duration = 0.0f;
            Vec3 color = Vec3(0);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", duration);

            if (eventParameter.IsParameterSet("Color"))
                eventParameter.GetParameter("Color", color);

            m_specularColorBegin = m_pLight->GetSpecularColor();
            m_specularColorEnd = color;

            m_specularDuration = duration;
            m_specularTime = duration;
        }
        break;

        case LIGHT_EVENT_SET_LIGHTRAYS:
        {
            bool bLightRays = false;
            if (eventParameter.IsParameterSet("Lightrays"))
                eventParameter.GetParameter("Lightrays", bLightRays);

            m_bLightraysInitial = m_pLight->GetLightRays();
            m_bLightraysSet = true;

            m_pLight->SetLightRays(bLightRays);
        }
        break;

        case LIGHT_EVENT_SET_ENERGY:
        {
            float duration = 0.0f;
            float energy = 1.0f;

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", duration);

            if (eventParameter.IsParameterSet("Energy"))
                eventParameter.GetParameter("Energy", energy);

            m_energyBegin = m_pLight->GetEnergy();
            m_energyEnd = energy;

            m_energyDuration = duration;
            m_energyTime = duration;
        }
        break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (m_pLight->GetRenderInGame() && m_pLight->GetLightRays())
        {
            m_lightRaysRenderObject.m_bLightRays = true;
            m_lightRaysRenderObject.m_lightRaysDecay = m_pLight->GetLightRaysDecay();
            m_lightRaysRenderObject.m_lightRaysIllumDecay = m_pLight->GetLightRaysIllumDecay();
            m_lightRaysRenderObject.m_bLightRaysOcclusionMask = m_pLight->GetLightRaysUseOcclusionMask();
            m_lightRaysRenderObject.m_bLightRaysOrigin = m_pLight->GetLightRaysUseWorldOrigin();
            m_lightRaysRenderObject.m_lightRaysOrigin = m_pNode->GetDerivedPosition();
            m_lightRaysRenderObject.m_lightRaysColor = m_pLight->GetEnergy() * Vec3(m_pLight->GetDiffuseColor().x,
                                                                                           m_pLight->GetDiffuseColor().y,
                                                                                           m_pLight->GetDiffuseColor().z);

            queue.m_pLightRaysRenderObject = &m_lightRaysRenderObject;
        }
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);

        if (m_lightOnTime >= 0.0f)
        {
            m_pLight->SetEnergy(m_pLight->GetInitialEnergy() * (1.0f - m_lightOnTime));
            m_lightOnTime -= dt;
        }
        else if (m_lightOffTime >= 0.0f)
        {
            m_pLight->SetEnergy(m_pLight->GetInitialEnergy() * m_lightOffTime);
            m_lightOffTime -= dt;

        }

        if (m_diffuseTime >= 0.0f)
        {
            float alpha = 0.0f;

            if (m_diffuseDuration > 0.00f)
                alpha = m_diffuseTime / m_diffuseDuration;

            Vec3 color = Lerp(m_diffuseColorEnd, m_diffuseColorBegin, alpha);

            m_pLight->SetDiffuseColor(Vec4(color, 0));
            
            m_diffuseTime -= dt;
        }

        if (m_specularTime >= 0.0f)
        {
            float alpha = 0.0f;

            if (m_specularDuration > 0.00f)
                alpha = m_specularTime / m_specularDuration;

            Vec3 color = Lerp(m_specularColorEnd, m_specularColorBegin, alpha);

            m_pLight->SetSpecularColor(Vec4(color, 0));

            m_specularTime -= dt;
        }

        if (m_energyTime >= 0.0f)
        {
            float alpha = 0.0f;

            if (m_energyDuration > 0.00f)
                alpha = m_energyTime / m_energyDuration;

            float energy = Lerp(m_energyEnd, m_energyBegin, alpha);

            m_pLight->SetEnergy(energy);

            m_energyTime -= dt;
        }

        if (m_bFlicker)
        {
            m_flickerCurrentTime -= dt;

            if (m_bFlickering)
            {
                if (m_flickerCurrentTime >= m_flickerTargetTime * 0.5f)
                {
                    float flickerParam = m_flickerAmount * (m_flickerCurrentTime - m_flickerTargetTime * 0.5f) / (m_flickerTargetTime * 0.5f);

                    m_sprite.SetAlpha(std::min((1.0f - m_flickerAmount) + flickerParam, 1.0f));

                    m_pLight->SetEnergy((1.0f - m_flickerAmount) * m_pLight->GetInitialEnergy() + m_pLight->GetInitialEnergy() * flickerParam);
                }
                else
                {
                    float flickerParam = m_flickerAmount * (1.0f - m_flickerCurrentTime / (m_flickerTargetTime * 0.5f));

                    m_sprite.SetAlpha(std::min((1.0f - m_flickerAmount) + flickerParam, 1.0f));

                    m_pLight->SetEnergy((1.0f - m_flickerAmount) * m_pLight->GetInitialEnergy() + m_pLight->GetInitialEnergy() * flickerParam);
                }
            }
            else
            {
                m_pLight->SetEnergy(m_pLight->GetInitialEnergy());
                Vec4 clr = m_sprite.GetColor();
                clr.w = 1.0f;
                m_sprite.SetColor(clr);
            }

            if (m_flickerCurrentTime < 0.0f)
            {
                m_bFlickering = !m_bFlickering;

                if (m_bFlickering)
                {
                    m_flickerCurrentTime = m_flickerTime + m_flickerTime * GetRandomFloat(-m_flickerRandomness, m_flickerRandomness);
                }
                else
                {
                    m_flickerCurrentTime = m_flickerPauseTime + m_flickerPauseTime * GetRandomFloat(-m_flickerRandomness, m_flickerRandomness);
                }
                m_flickerTargetTime = m_flickerCurrentTime;

            }
        }

    }

    //-----------------------------------------------------------------------------------
    CLight* CLightSpriteEntity::GetLight()
    {
        return m_pLight;
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::SaveTemplate(const std::string& strFilename, const std::string& strName)
    {
        CScript script;

        CScript& generalScript = *script.AddSubscript("general");

        generalScript["type"] = (int)m_pLight->GetType();
        generalScript["diffuseColor"] = m_pLight->GetInitialDiffuseColor();
        generalScript["specularColor"] = m_pLight->GetInitialSpecularColor();
        generalScript["specular"] = m_pLight->GetSpecular();
        generalScript["static"] = m_pLight->GetStatic();
        generalScript["affectsTerrain"] = m_pLight->GetAffectsTerrain();
        generalScript["affectsMeshes"] = m_pLight->GetAffectsMeshes();
        generalScript["radius"] = m_pLight->GetRadius();
        generalScript["energy"] = m_pLight->GetInitialEnergy();
        generalScript["initTemplate"] = m_bInitFromTemplate;

        if (m_pLight->GetLightRays())
        {
            CScript& lightraysScript = *script.AddSubscript("lightrays");

            lightraysScript["lightrays"] = m_pLight->GetLightRays();
            lightraysScript["intensity"] = m_pLight->GetLightRaysDecay();
            lightraysScript["falloff"] = m_pLight->GetLightRaysIllumDecay();
            lightraysScript["occlusionMask"] = m_pLight->GetLightRaysUseOcclusionMask();
            lightraysScript["worldPos"] = m_pLight->GetLightRaysUseWorldOrigin();
        }

        if (GetFlicker())
        {
            CScript& flickerScript = *script.AddSubscript("flicker");

            flickerScript["time"] = GetFlickerTime();
            flickerScript["pauseTime"] = GetFlickerPauseTime();
            flickerScript["intensity"] = GetFlickerAmount();
            flickerScript["randomness"] = GetFlickerRandomness();
        }

        script.SaveScript(strFilename);

        m_strTemplate = strName;
    }

    //-----------------------------------------------------------------------------------
    void CLightSpriteEntity::LoadTemplate(const std::string& strFilename, bool bInitMaterial)
    {
        CScript* pScript = gScriptMgr->AddGetPtr(strFilename);

        if (!pScript)
            return;

        if (pScript->SubscriptExists("general"))
        {
            CScript& generalScript = *pScript->GetSubscript("general");

            m_pLight->SetType((LIGHTTYPE)(int)generalScript["type"]);
            m_pLight->SetInitialDiffuseColor(generalScript["diffuseColor"]);
            m_pLight->SetInitialSpecularColor(generalScript["specularColor"]);
            m_pLight->SetSpecular(generalScript["specular"]);;
            m_pLight->SetStatic(generalScript["static"]);;
            m_pLight->SetAffectsTerrain(generalScript["affectsTerrain"]);
            m_pLight->SetAffectsMeshes(generalScript["affectsMeshes"]);
            m_pLight->SetRadius(generalScript["radius"]);
            m_pLight->SetInitialEnergy(generalScript["energy"]);

            GetSprite().SetColor(m_pLight->GetInitialDiffuseColor());

            m_bInitFromTemplate = generalScript["initTemplate"];
        }

        if (pScript->SubscriptExists("lightrays"))
        {
            CScript& lightraysScript = *pScript->GetSubscript("lightrays");

            m_pLight->SetLightRays(lightraysScript["lightrays"]);;
            m_pLight->SetLightRaysDecay(lightraysScript["intensity"]);
            m_pLight->SetLightRaysIllumDecay(lightraysScript["falloff"]);
            m_pLight->SetLightRaysUseOcclusionMask(lightraysScript["occlusionMask"]);
            m_pLight->SetLightRaysUseWorldOrigin(lightraysScript["worldPos"]);
        }
        else
        {
            m_pLight->SetLightRays(false);
        }

        if (pScript->SubscriptExists("flicker"))
        {
            CScript& flickerScript = *pScript->AddSubscript("flicker");

            SetFlickerTime(flickerScript["time"]);
            SetFlickerPauseTime(flickerScript["pauseTime"]);
            SetFlickerAmount(flickerScript["intensity"]);
            SetFlickerRandomness(flickerScript["randomness"]);
        }
        else
        {
            SetFlicker(false);
        }

        gScriptMgr->Release(strFilename);

        m_strTemplate = strFilename;
    }
} // env