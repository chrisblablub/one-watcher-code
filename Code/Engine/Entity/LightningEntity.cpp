#include <Engine/Entity/LightningEntity.h>
#include <Engine/Core/Engine.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Scene/Light.h>


namespace env
{
    ENTITY_TYPEID CLightningEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_LIGHTNING;

    CLightningEntity::CLightningEntity()
        : m_pLightningLight(NULL),
        m_lightningTime(0.0f),
        m_accumLightningTime(0.0f),
        m_accumPauseTime(0.0f),
        //m_fogColor(1.0f, 1.0f, 1.0f, 1.0f),
        m_currentLightningIteration(0),
        m_energyFieldIntensity(0.0f),
        m_numLightningIterations(3)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CLightningEntity::~CLightningEntity()
    {
        if (m_pLightningLight)
            gEngine->GetEntityMgr()->RemoveEntity(m_pLightningLight->GetName());
    }

    void CLightningEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        m_pLightningLight = static_cast<CLightSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LIGHTSPRITE));
        m_pLightningLight->SetAutoDelete(false);
        m_pLightningLight->SetRenderGame(false);
        m_pLightningLight->GetLight()->SetCastingShadows(false);
        m_pLightningLight->GetLight()->SetType(LT_DIRECTIONAL);
        m_pLightningLight->GetLight()->SetInitialDiffuseColor(Vec4(0.3f, 0.3f, 0.3f, 0.3f));
        m_pLightningLight->GetLight()->SetSpecular(true);
        m_pLightningLight->GetLight()->SetInitialSpecularColor(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_pLightningLight->GetLight()->SetInitialEnergy(12.0f);
        m_pLightningLight->GetNode()->SetPosition(Vec3(0.0f, 50.0f, 0.0f));

        m_pNode->AttachChildNode(m_pLightningLight->GetNode());
    }

    void CLightningEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();
    }

    void CLightningEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);


        /*if(gEngine->GetEntityMgr()->GetActiveEntity() == this)
        {
        m_initialEnergy = m_pLightningLight->GetLight()->GetEnergy();
        return;
        }*/

        if (!GetGameMode())
            return;

        const float ENERGY_FIELD_INTENSITY = 0.75f;


        if (m_accumLightningTime >= 0.0f)
        {
            if (m_accumLightningTime < m_lightningTime * 0.5f)
            {
                // switch off
                /*float lightningTime = m_lightningTime;
                if(m_currentLightningIteration < 2)
                lightningTime *= 0.3f;*/

                float alpha = std::pow(m_accumLightningTime / (m_lightningTime * 0.5f), 1.5f);

                gEngine->GetWorldMgr()->GetScenario()->SetEnergyFieldIntensity(Lerp(gEngine->GetWorldMgr()->GetScenario()->GetInitialEnergyFieldIntensity(), m_energyFieldIntensity, alpha));
                m_pLightningLight->GetLight()->SetEnergy(m_pLightningLight->GetLight()->GetInitialEnergy() * alpha);

                Fog& fog = gEngine->GetWorldMgr()->GetScenario()->GetFog();
                Vec4 fogLightningColor = fog.initialFogColor + Vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.3f;
                fog.fogColor = alpha * fogLightningColor + (1.0f - alpha) * fog.initialFogColor;

                Vec3 skyLightningColor = fog.initialSkyColor + Vec3(1.0f, 1.0f, 1.0f) * 0.3f;
                fog.skyColor = alpha * skyLightningColor + (1.0f - alpha) * fog.initialSkyColor;
            }
            else
            {
                // on
                float alpha = std::pow((1.0f - ((m_accumLightningTime - m_lightningTime * 0.5f) / (m_lightningTime * 0.5f))), 0.8f);

                gEngine->GetWorldMgr()->GetScenario()->SetEnergyFieldIntensity(Lerp(gEngine->GetWorldMgr()->GetScenario()->GetInitialEnergyFieldIntensity(), m_energyFieldIntensity, alpha));
                m_pLightningLight->GetLight()->SetEnergy(m_pLightningLight->GetLight()->GetInitialEnergy() * alpha);

                Fog& fog = gEngine->GetWorldMgr()->GetScenario()->GetFog();
                Vec4 fogLightningColor = fog.initialFogColor + Vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.3f;
                fog.fogColor = alpha * fogLightningColor + (1.0f - alpha) * fog.initialFogColor;

                Vec3 skyLightningColor = fog.initialSkyColor + Vec3(1.0f, 1.0f, 1.0f) * 0.3f;
                fog.skyColor = alpha * skyLightningColor + (1.0f - alpha) * fog.initialSkyColor;
            }

            m_accumLightningTime -= dt;

            /*if(m_accumLightningTime < 0.0f)
            {
            ++m_currentLightningIteration;
            }*/
        }
        else if (m_accumPauseTime <= 0.0f)
        {
            m_pLightningLight->GetLight()->SetEnergy(0.0f);

            float randVal = GetRandomFloat(0.0f, 1.0f);
            float probability = 0.2f;

            if (m_currentLightningIteration < m_numLightningIterations)
                probability = 0.0f;

            if (randVal >= probability)
            {

                m_accumLightningTime = GetRandomFloat(0.70f, 0.8f);
                m_lightningTime = m_accumLightningTime;
                m_accumPauseTime = GetRandomFloat(0.0f, 4.0f);
                m_energyFieldIntensity = ENERGY_FIELD_INTENSITY;

                if (m_currentLightningIteration == 0)
                {
                    m_numLightningIterations = (int)GetRandomFloat(1, 5);
                }

                if (m_currentLightningIteration < m_numLightningIterations - 1)
                {
                    m_accumLightningTime *= GetRandomFloat(0.4f, 0.6f);
                    m_accumPauseTime = GetRandomFloat(0.0f, 0.05f);
                    m_energyFieldIntensity *= GetRandomFloat(0.3f, 0.65f);
                }
                else
                    m_accumPauseTime = GetRandomFloat(1.5f, 5.0f);


                CQuaternion quat = m_pLightningLight->GetNode()->GetOrientation();
                quat.SetYaw(GetRandomFloat(-1.0f, 1.0f));
                m_pLightningLight->GetNode()->SetOrientation(quat);

                ++m_currentLightningIteration;
                if (m_currentLightningIteration >= m_numLightningIterations)
                    m_currentLightningIteration = 0;
            }
        }
        else
            m_accumPauseTime -= dt;
    }

    void CLightningEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_accumLightningTime = 0.0f;
        m_accumPauseTime = GetRandomFloat(0.6f, 2.5f);
        m_pLightningLight->GetLight()->SetEnergy(0.0f);

        //m_fogColor = gEngine->GetWorldMgr()->GetScenario()->GetFog().initialFogColor;
        //m_skyColor = gEngine->GetWorldMgr()->GetScenario()->GetFog().initialSkyColor;
    }

} // env 
