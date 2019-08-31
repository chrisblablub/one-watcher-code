#include <Engine/Core/Scenario.h>
#include <Engine/Scene/SceneMgr.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    CScenario::CScenario()
        : m_contrast(1.0f)
        , m_saturation(1.0f)
        , m_contrastSlowMo(1.0f)
        , m_saturationSlowMo(0.5f)
        , m_sepiaSlowMo(0.5f)
        , m_hueRotationSlowMo(0.0f)
        , m_sepia(0.0f)
        , m_hueRotation(0.0f)
        , m_silhouetteColor(0.1f, 0.1f, 0.1f, 1.0f)
        , m_silhouetteIntensity(0.8f)
        , m_colorBrightness(0.0f, 0.0f, 0.0f)
        , m_colorBalance(1.0f, 1.0f, 1.0f)
        , m_colorStandardWeights(0.299f, 0.587f, 0.114f)
        , m_borderColor(1.0f, 1.0f, 1.0f)
        , m_bloomScale(1.0f)
        , m_bloomBrightpassOffset(2.0f)
        , m_bloomBrightpassThreshold(1.0f)
        , m_currentBloomScale(1.0f)
        , m_currentBloomThreshold(10.0f)
        , m_pSkyColorTexture(nullptr)
        , m_pSkyGradientTexture(nullptr)
        , m_cloudIntensity(1.0f)
        , m_initialEnergyFieldIntensity(1.0f)
        , m_currentEnergyFieldIntensity(1.0f)
        , m_weather(SCENARIO_WEATHER_NORMAL)
        , m_bSilhouettes(false)

    {
        srand((unsigned int)time(NULL));

        m_terrainDiffuseColors.resize(10);
        m_terrainDiffuseColors[0] = UndoGamma(Vec3(1.00f, 1.00f, 1.00f)); // 255
        m_terrainDiffuseColors[1] = UndoGamma(Vec3(0.80f, 0.80f, 0.80f)); // 200
        m_terrainDiffuseColors[2] = UndoGamma(Vec3(0.39f, 0.39f, 0.39f)); // 100
        m_terrainDiffuseColors[3] = UndoGamma(Vec3(0.31f, 0.31f, 0.31f)); // 80
        m_terrainDiffuseColors[4] = UndoGamma(Vec3(0.31f, 0.31f, 0.31f)); // 80
        m_terrainDiffuseColors[5] = UndoGamma(Vec3(0.23f, 0.23f, 0.23f)); // 60
        m_terrainDiffuseColors[6] = UndoGamma(Vec3(0.23f, 0.23f, 0.23f)); // 60
        m_terrainDiffuseColors[7] = UndoGamma(Vec3(0.23f, 0.23f, 0.23f)); // 60
        m_terrainDiffuseColors[8] = UndoGamma(Vec3(0.23f, 0.23f, 0.23f)); // 60
        m_terrainDiffuseColors[9] = UndoGamma(Vec3(0.23f, 0.23f, 0.23f)); // 60

        m_terrainEmissiveColors.resize(10);
        m_terrainEmissiveColors[0] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[1] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[2] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[3] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[4] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[5] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[6] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[7] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[8] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255
        m_terrainEmissiveColors[9] = Vec4(1.00f, 1.00f, 1.00f, 1.00f); // 255

        m_borderColor = UndoGamma(Vec3(0.31f, 0.31f, 0.31f));
    }

    //-----------------------------------------------------------------------------------
    CScenario::~CScenario()
    {
        for (int i = 0; i < m_skylines.size(); ++i)
        {
            if (m_skylines[i].pTexture)
                gTextureMgr->Release(m_skylines[i].pTexture->GetName());
        }

        if (m_pSkyColorTexture)
            gTextureMgr->Release(m_pSkyColorTexture->GetName());

        if (m_pSkyGradientTexture)
            gTextureMgr->Release(m_pSkyGradientTexture->GetName());

    }

    //-----------------------------------------------------------------------------------
    void CScenario::InitializeAtCreation()
    {
    }

    //-----------------------------------------------------------------------------------
    void CScenario::InitializeAtLoad()
    {

        m_skylines.clear();
        for (int i = 0; i < m_skylines.size(); ++i)
        {
            SetSkyline(i, m_skylines[i]);
        }

        if (m_pSkyColorTexture)
            gTextureMgr->Release(m_pSkyColorTexture->GetName());

        if (m_pSkyGradientTexture)
            gTextureMgr->Release(m_pSkyGradientTexture->GetName());

        m_pSkyColorTexture = NULL;
        m_pSkyGradientTexture = NULL;
    }

    //-----------------------------------------------------------------------------------
    void CScenario::AddToRenderQueue(RenderQueue& renderQueue)
    {
        m_bloomRenderObject.m_bloomThreshold = m_currentBloomThreshold;
        m_bloomRenderObject.m_bloomScale = m_currentBloomScale;

        m_environmentRenderObject.m_cloudIntensity = GetCloudIntensity();
        m_environmentRenderObject.m_energyFieldIntensity = GetEnergyFieldIntensity();
        m_environmentRenderObject.m_fogColor = Vec3(GetFog().fogColor);
        m_environmentRenderObject.m_fogStart = GetFog().fogStart;
        m_environmentRenderObject.m_fogRange = GetFog().fogRange;
        m_environmentRenderObject.m_skyColor = GetFog().skyColor;
        m_environmentRenderObject.m_weather =  m_weather;
        m_environmentRenderObject.m_borderColor = m_borderColor;
        m_environmentRenderObject.m_strBorderTexture = GetBorderTexture();

        renderQueue.m_postProcessRenderObject.m_silhouetteIntensity = GetSilhouetteIntensity();
        renderQueue.m_postProcessRenderObject.m_silhouetteColor = Vec3(GetSilhouetteColor());
        renderQueue.m_postProcessRenderObject.m_colorScaling = GetColorBalance();
        renderQueue.m_postProcessRenderObject.m_colorOffset = GetColorBrightness();
        renderQueue.m_postProcessRenderObject.m_contrast = GetContrast();
        renderQueue.m_postProcessRenderObject.m_sepia = GetSepia();
        renderQueue.m_postProcessRenderObject.m_hueRotation = GetHueRotation();
        renderQueue.m_postProcessRenderObject.m_saturation *= GetSaturation();

        renderQueue.m_pBloomRenderObject = &m_bloomRenderObject;
        renderQueue.m_pEnvironmentRenderObject = &m_environmentRenderObject;
    }

    //-----------------------------------------------------------------------------------
    void CScenario::OnGameModeChanged(bool bGameMode)
    {
        m_currentBloomScale = m_bloomScale;
        m_currentBloomThreshold = m_bloomBrightpassThreshold;

        m_currentEnergyFieldIntensity = m_initialEnergyFieldIntensity;

        m_fog.fogColor = m_fog.initialFogColor;
        m_fog.skyColor = m_fog.initialSkyColor;
    }

    //-----------------------------------------------------------------------------------
    void CScenario::SetSkyline(unsigned int layer, const Skyline& skyline)
    {
        if (layer >= m_skylines.size())
        {
            m_skylines.resize(layer + 1);
        }

        if (m_skylines[layer].pTexture)
        {
            gTextureMgr->Release(m_skylines[layer].pTexture->GetName());
            m_skylines[layer].pTexture = NULL;
        }

        m_skylines[layer] = skyline;

        m_skylines[layer].pTexture = gTextureMgr->AddGetPtr(m_skylines[layer].strTexture);
    }

    //-----------------------------------------------------------------------------------
    Skyline& CScenario::GetSkyline(unsigned int layer)
    {
        if (layer >= m_skylines.size())
        {
            m_skylines.resize(layer + 1);
        }
        return m_skylines[layer];
    }

    //-----------------------------------------------------------------------------------
    void CScenario::SetTerrainDiffuseColor(unsigned int layer, Vec3 vColor)
    {
        m_terrainDiffuseColors[layer] = vColor;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CScenario::GetTerrainDiffuseColor(unsigned int layer)
    {
        return m_terrainDiffuseColors[layer];
    }

    //-----------------------------------------------------------------------------------
    void CScenario::SetTerrainEmissiveColor(unsigned int layer, const Vec4& vColor)
    {
        m_terrainEmissiveColors[layer] = vColor;
    }

    //-----------------------------------------------------------------------------------
    const Vec4& CScenario::GetTerrainEmissiveColor(unsigned int layer)
    {
        return m_terrainEmissiveColors[layer];
    }

}