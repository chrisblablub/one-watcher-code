#include <Engine/Entity/EnvMapEntity.h>

namespace env
{
    ENTITY_TYPEID CEnvMapEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ENVMAP;

    //-----------------------------------------------------------------------------------
    CEnvMapEntity::CEnvMapEntity()
        : m_pTexture(nullptr)
        , m_bEnabled(true)
    {
        SetMetaTypeID(CEnvMapEntity::ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CEnvMapEntity::~CEnvMapEntity()
    {
        gTextureMgr->Release(m_pTexture);
    }

    //-----------------------------------------------------------------------------------
    void CEnvMapEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        SetMaterialFilename("Sprites/envmap.png");
        SetRenderGame(false);
    }

    //-----------------------------------------------------------------------------------
    void CEnvMapEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        SetEnvironmentMap(m_strEnvMap);
        SetRenderGame(false);
    }

    //-----------------------------------------------------------------------------------
    void CEnvMapEntity::SetEnvironmentMap(const std::string& strEnvMap)
    {
        if (m_pTexture)
        {
            gTextureMgr->Release(m_pTexture);
            m_pTexture = nullptr;
            m_strEnvMap.clear();
        }

        m_pTexture = gTextureMgr->AddGetPtr(strEnvMap);

        if (m_pTexture)
        {
            CTextureSampler textureSampler(TEXTUREADDRESS_TYPE_WRAP, TEXTUREADDRESS_TYPE_WRAP,
                TEXTUREFILTER_TYPE_ANISOTROPIC, TEXTUREFILTER_TYPE_LINEAR, TEXTUREFILTER_TYPE_LINEAR, 4);

            m_pTexture->SetSRGB(true);
            m_pTexture->SetTextureSampler(textureSampler);

            m_strEnvMap = strEnvMap;
        }
    }

    //-----------------------------------------------------------------------------------
    void CEnvMapEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    //-----------------------------------------------------------------------------------
    void CEnvMapEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        CSpriteEntity::AddToRenderQueue(dt, renderQueue);

        m_envMapRenderObject.m_pEnvMap = m_pTexture;

        if (m_bEnabled)
        {
            renderQueue.m_pEnvMapRenderObject = &m_envMapRenderObject;

            if (m_envMapRenderObject.m_pEnvMap)
                m_envMapRenderObject.m_pEnvMap->SetAlphaChannel(true);
        }
    }

} // env
