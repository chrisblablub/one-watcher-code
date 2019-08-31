#include <Engine/Scene/Light.h>
#include <Engine/Scene/SceneMgr.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CLight::CLight()
        : m_initialSpecularColor(Vec4(0.4f, 0.4f, 0.4f, 0.4f))
        , m_currentSpecularColor(Vec4(0.4f, 0.4f, 0.4f, 0.4f))
        , m_initialDiffColor(Vec4(1.0f, 0.8f, 0.0f, 1.0f))
        , m_currentDiffColor(Vec4(1.0f, 0.8f, 0.0f, 1.0f))
        , m_vAttenuation(Vec3(0.0f, 500.0f, 0.0f))
        , m_softness(8.0f)
        , m_jitter(1.0f)
        , m_bShadows(false)
        , m_bSoftShadows(false)
        , m_bSerialize(false)
        , m_bLightCone(false)
        , m_bRenderInEditor(true)
        , m_bRenderInGame(true)
        , m_bSharpEdges(false)
        , m_bSpecular(false)
        , m_bLightRays(false)
        , m_bLightRaysUseOcclusionMask(true)
        , m_bLightRaysUseWorldOrigin(false)
        , m_bStatic(false)
        , m_bAffectsMeshes(true)
        , m_bAffectsTerrain(true)
        , m_shadowSamples(1)
        , m_shadowMapSize(1024)
        , m_radius(500.0f)
        , m_coneLength(250.0f)
        , m_currentEnergy(4.0f)
        , m_initialEnergy(4.0f)
        , m_spotInnerAngle(PI * 0.15f)
        , m_spotOuterAngle(PI * 0.17f)
        , m_lightRaysDecay(0.94f)
        , m_lightRaysIllumDecay(0.02f)
        , m_type(LT_UNSPECIFIED)
        , m_pShadowCamNode(nullptr) {}

    //-----------------------------------------------------------------------------------
    CLight::CLight(const std::string& strName, const Vec4& vDiffColor, const Vec4& vSpecColor, float SpotRange)
        : m_strName(strName)
        , m_initialDiffColor(vDiffColor)
        , m_currentDiffColor(vDiffColor)
        , m_initialSpecularColor(vSpecColor)
        , m_currentSpecularColor(vSpecColor)
        , m_vAttenuation(Vec3(0.0f, 500.0f, 0.0f))
        , m_softness(8.0f)
        , m_jitter(1.0f)
        , m_bShadows(false)
        , m_bSoftShadows(false)
        , m_bLightCone(false)
        , m_bSerialize(false)
        , m_bRenderInEditor(true)
        , m_bRenderInGame(true)
        , m_bSharpEdges(false)
        , m_bSpecular(false)
        , m_bLightRays(false)
        , m_bLightRaysUseOcclusionMask(true)
        , m_bLightRaysUseWorldOrigin(false)
        , m_bStatic(false)
        , m_bAffectsMeshes(true)
        , m_bAffectsTerrain(true)
        , m_shadowSamples(1)
        , m_shadowMapSize(1024)
        , m_radius(500.0f)
        , m_coneLength(250.0f)
        , m_currentEnergy(4.0f)
        , m_initialEnergy(4.0f)
        , m_spotInnerAngle(PI * 0.15f)
        , m_spotOuterAngle(PI * 0.17f)
        , m_lightRaysDecay(0.94f)
        , m_lightRaysIllumDecay(0.02f)
        , m_type(LT_SPOT)
        , m_pShadowCamNode(nullptr)
    {
        CFrustum& frustum = m_shadowCamera.GetFrustum();
        frustum.SetNearClipping(1.0f);
        frustum.SetFarClipping(400.0f);
        frustum.SetFieldOfView(m_spotOuterAngle * 2);
    }

    //-----------------------------------------------------------------------------------
    CLight::~CLight()
    {
        
    }

    //-----------------------------------------------------------------------------------
    void CLight::InitializeAtCreation()
    {
    }


    //-----------------------------------------------------------------------------------
    void CLight::InitializeAtLoad()
    {
        m_currentEnergy = m_initialEnergy;

        if (m_pShadowCamNode)
        {
            gEngine->GetSceneMgr()->RemoveNode(m_pShadowCamNode);
            m_pShadowCamNode = nullptr;
        }

        m_shadowCamera.AttachNode(nullptr);

        if (m_type == LT_POINT)
            m_bShadows = false;
    }

    //-----------------------------------------------------------------------------------
    void CLight::OnGameModeChanged()
    {
        SetDiffuseColor(m_initialDiffColor);
        SetSpecularColor(m_initialSpecularColor);
        SetEnergy(m_initialEnergy);
    }

    //-----------------------------------------------------------------------------------
    void CLight::SetSpotInnerAngle(float rad)
    {
        ENV_ASSERT_DBG(rad <= PI);

        m_spotInnerAngle = rad;
        if (m_spotInnerAngle > m_spotOuterAngle)
            m_spotOuterAngle = m_spotInnerAngle;

        m_shadowCamera.GetFrustum().SetFieldOfView(m_spotOuterAngle * 2);
    }

    //-----------------------------------------------------------------------------------
    void CLight::SetSpotOuterAngle(float rad)
    {
        ENV_ASSERT_DBG(rad <= PI);

        m_spotOuterAngle = rad;
        if (m_spotOuterAngle < m_spotInnerAngle)
            m_spotInnerAngle = m_spotOuterAngle;

        m_shadowCamera.GetFrustum().SetFieldOfView(m_spotOuterAngle * 2);
    }

    //-----------------------------------------------------------------------------------
    void CLight::AddToRenderQueue(RenderQueue& renderQueue)
    {
        m_lightRenderObject.m_position = m_pNode->GetDerivedPosition();
        m_lightRenderObject.m_direction = m_pNode->GetDerivedDirection();
        m_lightRenderObject.m_mtxTransform = m_pNode->GetDerivedRST();
        m_lightRenderObject.m_scale = m_pNode->GetDerivedScale();
        m_lightRenderObject.m_diffuseColor = GetDiffuseColor();
        m_lightRenderObject.m_specularColor = GetSpecularColor();
        m_lightRenderObject.m_energy = GetEnergy();
        m_lightRenderObject.m_radius = GetRadius();
        m_lightRenderObject.m_bSpecular = GetSpecular();
        m_lightRenderObject.m_bStatic = GetStatic();
        m_lightRenderObject.m_bAffectsMeshes = m_bAffectsMeshes;
        m_lightRenderObject.m_bAffectsTerrain = m_bAffectsTerrain;
        m_lightRenderObject.m_type = GetType();

        renderQueue.m_lightRenderObjects.push_back(&m_lightRenderObject);
    }

}